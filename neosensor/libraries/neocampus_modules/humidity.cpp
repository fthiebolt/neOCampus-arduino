/*
 * neOCampus operation
 * 
 * Humidity module to manage all hygro sensors
 * 
 * ---
 * Notes:
 * - force data sent through MQTT as an int
 * ---
 * TODO:
 * - convert all 'frequency' parameters & define into 'cooldown' ones
 * ---
 * F.Thiebolt aug.20  switched to intelligent data sending vs timer based data sending
 * Thiebolt.F may.20  initial release
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

#include "humidity.h"


/* 
 * Definitions
 */
#define MQTT_MODULE_NAME        "humidity"  // used to build module's base topic
#define DATA_JSON_SIZE          (JSON_OBJECT_SIZE(20))
#define CONFIG_JSON_SIZE        (JSON_OBJECT_SIZE(3))   // config file contains: frequency



// constructors
humidity::humidity( void ): base() {
  // call lov-level constructor
  _constructor();
}

// low-level constructor
void humidity::_constructor( void ) {
  _freq = DEFL_HUMIDITY_FREQUENCY;
  for( uint8_t i=0; i < _MAX_SENSORS; i++ )
    _sensor[i] = nullptr;

  // load json config file (if any)
  loadConfig( );
}

// destructor
humidity::~humidity( void ) {
  for( uint8_t i=0; i < _sensors_count; i++ ) {
    if( _sensor[i] == nullptr ) continue;
    free( _sensor[i] );
    _sensor[i] = nullptr;
  }
  _sensors_count = 0;
}



/*
 * add_device method
 */
boolean humidity::add_sensor( uint8_t adr ) {
  // check if it is possible to add a sensor
  if( _sensors_count>=_MAX_SENSORS ) return false;

  bool _sensor_added=false;
  /*
   * check for SHTxx
   */
  if( SHT2x::is_device( adr ) == true ) {
    SHT2x *cur_sensor = new SHT2x( sht2xMeasureType_t::humidity );   // because it features two sensors
    if( cur_sensor->begin( adr ) != true ) {
      log_debug(F("\n[humidity] ###ERROR at SHT2x startup ... removing instance ..."));log_flush();
      free(cur_sensor);
      cur_sensor = NULL;
    }
    else {
      cur_sensor->powerON(); // remember that device is shared across 2 modules
      // cur_sensor->setResolution(); default is max resolution
      cur_sensor->powerOFF(); // remember that device is shared across 2 modules
      _sensor[_sensors_count++] = cur_sensor;
      _sensor_added=true;
    }
  }
  else if( SHT3x::is_device( adr ) == true ) {
    SHT3x *cur_sensor = new SHT3x( sht3xMeasureType_t::humidity );
    if( cur_sensor->begin( adr ) != true ) {
      log_debug(F("\n[humidity] ###ERROR at SHT3x startup ... removing instance ..."));log_flush();
      free(cur_sensor);
      cur_sensor = NULL;
    }
    else {
      cur_sensor->powerON(); // remember that device is shared across 2 modules
      // cur_sensor->setResolution(); default is max resolution
      cur_sensor->powerOFF(); // remember that device is shared across 2 modules
      _sensor[_sensors_count++] = cur_sensor;
      _sensor_added=true;
    }
  }
  // check for SCD4x
  else if( SCD4x::is_device( adr ) == true ) {
    SCD4x *cur_sensor = new SCD4x( scd4xMeasureType_t::humidity );    // because it features several sensors
    if( cur_sensor->begin( adr ) != true ) {
      log_debug(F("\n[temperature] ###ERROR at SCD4x startup ... removing instance ..."));log_flush();
      free(cur_sensor);
      cur_sensor = NULL;
    }
    else {
      cur_sensor->powerON();  // remember that device is shared across several modules
      cur_sensor->powerOFF(); // remember that device is shared across several modules
      _sensor[_sensors_count++] = cur_sensor;
      _sensor_added=true;
    }
  }

  // add check for additional device here

  // summary
  if( !_sensor_added ) return false;

  // everything is ok :)
  return true;
}



// check if at least one sensor exist
boolean humidity::is_empty( ) {
  return ( _sensors_count==0 ? true : false );
}



/*
 * Module network startup procedure (MQTT)
 */
bool humidity::start( senso *sensocampus, JsonDocument &sharedRoot  ) {

  log_info(F("\n[humidity] starting module ..."));
  // create module's JSON structure to hold all of our data
  // [aug.21] we create a dictionnary
  variant = sharedRoot.createNestedObject(MQTT_MODULE_NAME);
  // all sensors share the same units of values
  JsonObject _obj = variant.as<JsonObject>();
  _obj[F("value_units")] = "%r.H.";

  // initialize module's publish & subscribe topics
  snprintf( pubTopic, sizeof(pubTopic), "%s/%s", sensocampus->getBaseTopic(), MQTT_MODULE_NAME);
  snprintf( subTopic, sizeof(subTopic), "%s/%s", pubTopic, "command" );
  return base::start( sensocampus, sharedRoot );
}


/*
 * Handler for subscribed messages
 */
void humidity::handle_msg( JsonObject root ) {
  log_info(F("\n[humidity] received COMMAND to process ...")); log_flush();
  
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
    log_error(F("\n[humidity][callback] unknown key: "));log_debug((const char*)(it->key().c_str())); log_flush();
  }
}


/*
 * process module's activites
 */
bool humidity::process( void ) {

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
  if( _sensors_count==0 ) return false;
  
  // send all sensors' values
  return _sendValues();
}


/*
 * Status report sending
 */
void humidity::status( JsonObject root ) {
  
  // add base class status
  base::status( root );
    
  /*
   * TODO: list of sensors IDs
   */
}


/*
 * Module's sensOCampus config to load (if any)
 */
boolean humidity::loadSensoConfig( senso *sp ) {

  JsonArray _array;
  if( ! sp->getModuleConf( MQTT_MODULE_NAME, _array ) ) {
    //log_debug(F("\n[humidity] no sensOCampus config found")); log_flush();
    return false;
  }

  log_debug(F("\n[humidity] loading sensOCampus config is NOT YET IMPLEMENTED!")); log_flush();
  return false;



// TO BE CONTINUED



  // (re)load the local config file (to override default parameters values from sensOCampus)
  log_debug(F("\n[humidity] (re)loading config file (if any)")); log_flush();
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
void humidity::_process_sensors( void ) {
  // process all valid sensors
  for( uint8_t cur_sensor=0; cur_sensor<_sensors_count; cur_sensor++ ) {
    if( _sensor[cur_sensor]==nullptr ) continue;
    // start sensor processing according to our coolDown parameter
    // [aug.21] _freq is our coolDown parameter
    _sensor[cur_sensor]->process( _freq );
    if( _sensor[cur_sensor]->getTrigger()!=true ) continue;

    // new data ready to get sent ==> activate module's trigger
    log_debug(F("\n[humidity][")); log_debug(_sensor[cur_sensor]->subID());
    log_debug(F("] new official value = "));log_debug(_sensor[cur_sensor]->getValue()); log_flush();
    _trigger = true;  // activate module level trigger

    /*
     * update shared JSON
     */
    JsonObject _obj = variant.as<JsonObject>();
    _obj[_sensor[cur_sensor]->subID()] = _sensor[cur_sensor]->getValue();
  }
}


/*
 * send all sensors' values
 * [aug.21] this function gets called every XXX_FREQ seconds but according
 *  to sensors integration, a value may not be available (e.g it does not 
 *  changed for a long time).
 * However, there's some point over a period of time a data will get sent
 *  even if if didn't change.
 */
boolean humidity::_sendValues( void ) {
  /* grab and send values from all sensors
   * each sensor will result in a MQTT message
   */
  // boolean _TXoccured = false;

  for( uint8_t cur_sensor=0; cur_sensor<_sensors_count; cur_sensor++ ) {

    if( _sensor[cur_sensor]==nullptr || _sensor[cur_sensor]->getTrigger()!=true ) continue;

    StaticJsonDocument<DATA_JSON_SIZE> _doc;
    JsonObject root = _doc.to<JsonObject>();

    // retrieve data from current sensor
    float value = _sensor[cur_sensor]->getValue();

    root[F("value")] = (int)( value );   // [may.20] force humidity as INT
    root[F("value_units")] = _sensor[cur_sensor]->sensorUnits();
    root[F("subID")] = _sensor[cur_sensor]->subID();

    /*
    * send MQTT message
    */
    if( sendmsg( root ) ) {
      log_info(F("\n[humidity] successfully published msg :)")); log_flush();
      // _TXoccured = true;
  }
    else {
      // we stop as soon as we've not been able to successfully send one message
      log_error(F("\n[humidity] ERROR failure MQTT msg delivery :(")); log_flush();
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
bool humidity::_processOrder( const char *order, int *value ) {

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
        setFrequency( (uint16_t)(*value), HUMIDITY_MIN_FREQUENCY, HUMIDITY_MAX_FREQUENCY );
        StaticJsonDocument<DATA_JSON_SIZE> _doc;
        JsonObject root = _doc.to<JsonObject>();
        status( root );
        sendmsg( root );
        return saveConfig();
      }
      else return false;
    }
  }

  log_error(F("\n[humidity][callback] unknown order: ")); log_debug(order); log_flush();
  return false;
}

/*
 * load an eventual module'specific config file
 */
bool humidity::loadConfig( void ) {
  
  if( ! SPIFFS.exists( MODULE_CONFIG_FILE(MQTT_MODULE_NAME) ) ) return false;

  File configFile = SPIFFS.open( MODULE_CONFIG_FILE(MQTT_MODULE_NAME), "r");
  if( !configFile ) return false;

  log_info(F("\n[humidity] load JSON config file")); log_flush();
  size_t size = configFile.size();
  // Allocate a buffer to store contents of the file.
  std::unique_ptr<char[]> buf(new char[size]);
  configFile.readBytes(buf.get(), size);
  configFile.close();

  // allocate JSON static buffer for module's config file
  StaticJsonDocument<CONFIG_JSON_SIZE> root;

  auto err = deserializeJson( root, buf.get() );
  if( err ) {
    log_error(F("\n[humidity] ERROR parsing module JSON config file!"));
    log_error(F("\n[humidity] ERROR msg: ")); log_error(err.c_str()); log_flush();
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
bool humidity::_loadConfig( JsonObject root ) {
  
  // check for 'frequency' field
  if( root.containsKey(F("frequency")) ) {
    setFrequency( (uint16_t)(root[F("frequency")].as<unsigned int>()), HUMIDITY_MIN_FREQUENCY, HUMIDITY_MAX_FREQUENCY );
  }

  /*
   * Parse additional fields here
   */
  
  return true;
}

/*
 * save module'specific config file
 */
bool humidity::saveConfig( void ) {
  
  // static JSON buffer
  StaticJsonDocument<CONFIG_JSON_SIZE> _doc;
  JsonObject root = _doc.to<JsonObject>();

  // frequency
  if( _freq != (uint16_t)DEFL_HUMIDITY_FREQUENCY )
    root[F("frequency")] = _freq;

  // add additional parameters to save here
  
  
  // call parent save
  return base::saveConfig( MODULE_CONFIG_FILE(MQTT_MODULE_NAME), root );
}

