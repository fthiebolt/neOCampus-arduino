/*
 * neOCampus operation
 * 
 * Display module to manage all kinds of displays
 * 
 * ---
 * Notes:
 * ---
 * TODO:
 * ---
 * F.Thiebolt   aug.21  initial release
 * 
 */


/*
 * Includes
 */
#include <FS.h>
#if defined(ESP32)
  #include "SPIFFS.h"
#endif

#include "neocampus.h"
#include "neocampus_debug.h"

#include "display.h"


/* 
 * Definitions
 */
#define MQTT_MODULE_NAME        "display"               // used to build module's base topic
#define DATA_JSON_SIZE          (JSON_OBJECT_SIZE(20))
#define CONFIG_JSON_SIZE        (JSON_OBJECT_SIZE(3))   // config file contains: frequency
// [nov.20] set FLOAT resolution data to send over MQTT
// #define FLOAT_RESOLUTION        3


// constructors
display::display( void ): base() {
  // call low-level constructor
  _constructor();
}

// low-level constructor
void display::_constructor( void ) {
  _freq = DEFL_DISPLAY_COOLDOWN;    // [aug.21] useless since we're now following a cooldown approach for sensors
  for( uint8_t i=0; i < _MAX_DISPLAYS; i++ )
    _display[i] = nullptr;

  // initialize total count of registered displays
  _displays_count = 0;

  // load json config file (if any)
  loadConfig( );
}

// destructor
display::~display( void ) {
  for( uint8_t i=0; i < _displays_count; i++ ) {
    if( _display[i] == nullptr ) continue;
    free( _display[i] );
    _display[i] = nullptr;
  }
  _displays_count = 0;
}



/*
 * add_device method
 */
boolean display::add_sensor( uint8_t adr ) {
  // check if it is possible to add a sensor
  if( _displays_count>=_MAX_DISPLAYS ) return false;

  bool _display_added=false;

  // check for OLED 1.3 inch (SH1106)
  if( oled1.3inches::is_device( adr ) == true ) {
    oled1.3inches *cur_display = new oled1.3inches();
    if( cur_display->begin( adr ) != true ) {
      log_debug(F("\n[display] ###ERROR at OLED1.3INCHES startup ... removing instance ..."));log_flush();
      free(cur_display);
      cur_display = nullptr;
    }
    else {
      cur_display->powerOFF();
      _display[_displays_count++] = cur_display;
      _display_added=true;
    }
  }
  //else if( SK9822::is_device( adr ) == true ) {

  // add check for additional device here

  // summary
  if( !_display_added ) return false;

  // everything is ok :)
  return true;
}



// check if at least one sensor exist
boolean display::is_empty( ) {
  return ( _displays_count==0 ? true : false );
}



/*
 * Module network startup procedure (MQTT)
 */
bool display::start( senso *sensocampus, JsonDocument &sharedRoot ) {

  log_info(F("\n[display] starting module ..."));
  // create module's JSON structure to hold all of our data
  // [aug.21] we create a dictionnary
  variant = sharedRoot.createNestedObject(MQTT_MODULE_NAME);
  // all sensors share the same units of values
  JsonObject _obj = variant.as<JsonObject>();
  // _obj[F("value_units")] = "°c"; no value_units for displays

  // initialize module's publish & subscribe topics
  snprintf( pubTopic, sizeof(pubTopic), "%s/%s", sensocampus->getBaseTopic(), MQTT_MODULE_NAME);
  snprintf( subTopic, sizeof(subTopic), "%s/%s", pubTopic, "command" );
  return base::start( sensocampus, sharedRoot );
}


/*
 * Handler for subscribed messages
 */
void display::handle_msg( JsonObject root ) {
  log_info(F("\n[display] received COMMAND to process ...")); log_flush();
  
  const char *_key_order = NULL;    // current 'order'
  int _key_value;                   // current 'value'
  bool _key_value_valid = false;    // is a key_value already get saved in _key_value ?
  
  // iterate over all [key,values] pairs
  for (JsonObject::iterator it=root.begin(); it!=root.end(); ++it) {

    {
      // skip 'dest'
      const char *_key = PSTR("dest");
      if( strncmp_P(it->key().c_str(), _key, strlen_P(_key))==0 ) {
        continue;
      }
    }
    
    {
      const char *_key = PSTR("order");
      if( strncmp_P(it->key().c_str(), _key, strlen_P(_key))==0 ) {
        // order received ... need to parse ...
        _key_order = it->value();
        _processOrder( _key_order, (_key_value_valid==true ? &_key_value : NULL) );
        continue;
      }
    }

    {
      const char *_key = PSTR("value");
      if( strncmp_P(it->key().c_str(), _key, strlen_P(_key))==0 ) {
        // value received ... 
        _key_value = it->value().as<int>();
        _key_value_valid = true;
        // if an order has already been received ...
        if( _key_order ) _processOrder( _key_order, &_key_value );
        continue;
      }
    }

    // unknown key ?!?!
    log_error(F("\n[display][callback] unknown key: "));log_debug((const char*)(it->key().c_str())); log_flush();
  }
}


/*
 * process module's activites
 */
bool display::process( void ) {

  bool _ret = false;
  
  /* 1st step, call process from base because
   * it will check MQTT client connectivity
   * and call handler for MQTT received messages
   */
  _ret = base::process();

  /* sensors internal processing */
  _process_sensors();

  // [aug.21] TXtime is not based on timer but upon data ready
  // to get sent !
  // reached time to transmit ?
  //if( !isTXtime() ) return _ret;

  // [aug.21] if global trigger has been activated, we'll parse all inputs
  // to check for individual triggers
  if( !_trigger ) return _ret;

  /*
   * Time to send a new message
   */
  // check that at least one sensor is available
  if( _displays_count==0 ) return false;
  
  // send all values
  return _sendValues();
}


/*
 * Status report sending
 */
void display::status( JsonObject root ) {
  
  // add base class status
  base::status( root );
    
  /*
   * TODO: add status from all displays
   */
}


/*
 * Module's sensOCampus config to load (if any)
 */
boolean display::loadSensoConfig( senso *sp ) {

  JsonArray _array;
  if( ! sp->getModuleConf( MQTT_MODULE_NAME, _array ) ) {
    //log_debug(F("\n[temperature] no sensOCampus config found")); log_flush();
    return false;
  }

  log_debug(F("\n[display] loading sensOCampus config is NOT YET IMPLEMENTED!")); log_flush();
  return false;



// TO BE CONTINUED



  // (re)load the local config file (to override default parameters values from sensOCampus)
  log_debug(F("\n[display] (re)loading config file (if any)")); log_flush();
  loadConfig();

  return true;
}


/* ------------------------------------------------------------------------------
 * Private methods 
 */

/*
 * sensors internal processing
 * this function is called every lopp() call and leverages
 * the needs for (e.g) continuous integration.
 */
void display::_process_sensors( void ) {
  // process all valid sensors
  for( uint8_t cur_display=0; cur_display<_displays_count; cur_display++ ) {
    if( _sensor[cur_display]==nullptr ) continue;
    // start display processing
    // [aug.21] _freq is our coolDown parameter
    _display[cur_display]->process( _freq );
    if( _display[cur_display]->getTrigger()!=true ) continue;

    // new data ready to get sent ==> activate module's trigger
    log_debug(F("\n[display][")); log_debug(_display[cur_display]->subID());
    log_debug(F("] has status/data to send: ")); log_debug(_sensor[cur_sensor]->getValue()); log_flush();
    _trigger = true;  // activate module level trigger

    /*
     * update shared JSON
     */
    JsonObject _obj = variant.as<JsonObject>();
    _obj[_display[cur_display]->subID()] = _display[cur_display]->getValue();
  }
}


/*
 * send all displays' values
 * This function gets called whenever the module's _trigger has been activated
 */
boolean display::_sendValues( void ) {
  /* grab and send values from all displays
   * Data sent from each display will result in a MQTT message
   */
  for( uint8_t cur_display=0; cur_display<_displays_count; cur_display++ ) {

    if( _display[cur_display]==nullptr || _display[cur_display]->getTrigger()!=true ) continue;

    StaticJsonDocument<DATA_JSON_SIZE> _doc;
    JsonObject root = _doc.to<JsonObject>();


TO BE CONTINUED


    // retrieve official value
    float value = _sensor[cur_sensor]->getValue();

    root[F("value")] = serialized(String(value,FLOAT_RESOLUTION));   // [nov.20] force float encoding
    //root[F("value")] = (float)( value );   // [may.20] force data as float (e.g ArduinoJson converts 20.0 to INT)
                                            // this doesn't work since ArduinoJson converts to STRING withiout decimal!
    root[F("value_units")] = _sensor[cur_sensor]->sensorUnits();
    root[F("subID")] = _sensor[cur_sensor]->subID();

    /*
     * send MQTT message
     */
    if( sendmsg( root ) ) {
      log_info(F("\n[temperature] successfully published msg :)")); log_flush();
      // _TXoccured = true;
    }
    else {
      // we stop as soon as we've not been able to successfully send one message
      log_error(F("\n[temperature] ERROR failure MQTT msg delivery :(")); log_flush();
      return false;
    }

    // mark data as sent
    _sensor[cur_sensor]->setDataSent();

    // delay between two successives values to send
    delay(20);
  }

  /* do we need to postpone to next TX slot:
   * required when no data at all have been published
   * [aug.21] useless since we don not rely anymore on periodic sending !
   *
  if( !_TXoccured ) cancelTXslot();
   */

  return true;
}


/*
 * orders processing ...
 */
bool temperature::_processOrder( const char *order, int *value ) {

  if( !order ) return false;
  
  {
    const char *_order = PSTR("status");
    if( strncmp_P(order, _order, strlen_P(_order))==0 ) {
      // required to send status ... so publishing while in callback :)
      StaticJsonDocument<DATA_JSON_SIZE> _doc;
      JsonObject root = _doc.to<JsonObject>();
      status( root );
      return sendmsg(root);
    }
  }
  
  {
    const char *_order = PSTR("acquire");
    if( strncmp_P(order, _order, strlen_P(_order))==0 ) {
      // required to send values ... so publishing while in callback :)
      return _sendValues();
    }
  }

  {
    const char *_order = PSTR("frequency");
    if( strncmp_P(order, _order, strlen_P(_order))==0 ) {
      if( value ) {
        setFrequency( (uint16_t)(*value), TEMPERATURE_MIN_FREQUENCY, TEMPERATURE_MAX_FREQUENCY );
        StaticJsonDocument<DATA_JSON_SIZE> _doc;
        JsonObject root = _doc.to<JsonObject>();
        status( root );
        sendmsg( root );
        return saveConfig();
      }
      else return false;
    }
  }

  log_error(F("\n[temperature][callback] unknown order: ")); log_debug(order); log_flush();
  return false;
}

/*
 * load an eventual module'specific config file
 */
bool temperature::loadConfig( void ) {
  
  if( ! SPIFFS.exists( MODULE_CONFIG_FILE(MQTT_MODULE_NAME) ) ) return false;

  File configFile = SPIFFS.open( MODULE_CONFIG_FILE(MQTT_MODULE_NAME), "r");
  if( !configFile ) return false;

  log_info(F("\n[temperature] load JSON config file")); log_flush();
  size_t size = configFile.size();
  // Allocate a buffer to store contents of the file.
  std::unique_ptr<char[]> buf(new char[size]);
  configFile.readBytes(buf.get(), size);
  configFile.close();

  // allocate JSON static buffer for module's config file
  StaticJsonDocument<CONFIG_JSON_SIZE> root;

  auto err = deserializeJson( root, buf.get() );
  if( err ) {
    log_error(F("\n[temperature] ERROR parsing module JSON config file!"));
    log_error(F("\n[temperature] ERROR msg: ")); log_error(err.c_str()); log_flush();
    SPIFFS.remove( MODULE_CONFIG_FILE(MQTT_MODULE_NAME) );
    return false;
  }
#if (LOG_LEVEL >= LOG_LVL_DEBUG)
  serializeJsonPretty( root, Serial );
#endif

  // parse and apply JSON config
  return _loadConfig( root.as<JsonObject>() );
}

/*
 * low-level load JSON config
 */
bool temperature::_loadConfig( JsonObject root ) {
  
  // check for 'frequency' field
  if( root.containsKey(F("frequency")) ) {
    setFrequency( (uint16_t)(root[F("frequency")].as<unsigned int>()), TEMPERATURE_MIN_FREQUENCY, TEMPERATURE_MAX_FREQUENCY );
  }

  /*
   * Parse additional fields here
   */
  
  return true;
}

/*
 * save module'specific config file
 */
bool temperature::saveConfig( void ) {
  
  // static JSON buffer
  StaticJsonDocument<CONFIG_JSON_SIZE> _doc;
  JsonObject root = _doc.to<JsonObject>();

  // frequency
  if( _freq != (uint16_t)DEFL_TEMPERATURE_FREQUENCY )
    root[F("frequency")] = _freq;

  // add additional parameters to save here
  
  
  // call parent save
  return base::saveConfig( MODULE_CONFIG_FILE(MQTT_MODULE_NAME), root );
}

