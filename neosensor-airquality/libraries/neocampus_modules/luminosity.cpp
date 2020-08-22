/*
 * neOCampus operation
 * 
 * Luminosity module to manage all luminosity sensors
 * 
 * Thiebolt.F may.20  force data sent through MQTT as an int
 * Thiebolt.F may.18  send back status upon any change settings received order 
 * Thiebolt.F jul.17  initial release
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

#include "luminosity.h"


/*
 * Definitons
 */
#define MQTT_MODULE_NAME        "luminosity"  // used to build module's base topic
#define DATA_JSON_SIZE          (JSON_OBJECT_SIZE(20))
#define CONFIG_JSON_SIZE        (JSON_OBJECT_SIZE(3))   // config file contains: frequency


// constructor
luminosity::luminosity(): base() 
{
  _freq = DEFL_LUMINOSITY_FREQUENCY;
  for( uint8_t i=0; i < _MAX_SENSORS; i++ )
    _sensor[i] = NULL;
  
  // load json config file (if any)
  loadConfig( );
}



/*
 * add_device method
 */
boolean luminosity::add_sensor( uint8_t adr ) {
  /* only one sensor support for now :| */
  if( _sensors_count>=_MAX_SENSORS ) return false;

  bool _sensor_added=false;
  /*
   * check for TSL2561
   */
  if( TSL2561::is_device( adr ) == true ) {
    TSL2561 *cur_sensor = new TSL2561();
    if( cur_sensor->begin( adr ) != true ) {
      log_debug(F("\n[luminosity] ###ERROR at TSL2561 startup ... removing instance ..."));log_flush();
      free(cur_sensor);
      cur_sensor = NULL;
    }
    else {
      // TODO: set auto_gain ?
      cur_sensor->powerOFF();
      _sensor[_sensors_count++] = cur_sensor;
      _sensor_added = true;
    }
  }
  // add check for additional device here

  // summary
  if( !_sensor_added ) return false;

  // everything is ok :)
  return true;
}



// check if at least one sensor exist
boolean luminosity::is_empty( ) {
  return ( _sensors_count==0 ? true : false );
}



/*
 * Module network startup procedure (MQTT)
 */
bool luminosity::start( senso *sensocampus ) {

    log_info(F("\n[luminosity] starting module ..."));
    // initialize module's publish and subscribe topics
    snprintf( pubTopic, sizeof(pubTopic), "%s/%s", sensocampus->getBaseTopic(), MQTT_MODULE_NAME);
    snprintf( subTopic, sizeof(subTopic), "%s/%s", pubTopic, "command" );
    return base::start( sensocampus );
}



/*
 * Handler for subscribed messages
 */
void luminosity::handle_msg( JsonObject root ) {
  log_info(F("\n[luminosity] received COMMAND to process ...")); log_flush();
  
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
    log_error(F("\n[luminosity][callback] unknown key: "));log_debug((const char*)(it->key().c_str())); log_flush();
  }
}


/*
 * process module's activites
 */
bool luminosity::process( void ) {

  bool _ret = false;
  
  /* 1st step, call process from base because
   * it will check MQTT client connectivity
   * and call handler for MQTT received messages
   */
  _ret = base::process();
  // reached time to transmit ?
  if( !isTXtime() ) return _ret;

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
void luminosity::status( JsonObject root ) {
  
  // add base class status
  base::status( root );
    
  /*
   * TODO: add list of sensors IDs
   */
}


/*
 * Module's sensOCampus config to load (if any)
 */
boolean luminosity::loadSensoConfig( senso *sp ) {

  JsonArray _array;
  if( ! sp->getModuleConf( MQTT_MODULE_NAME, &_array ) ) {
    //log_debug(F("\n[luminosity] no sensOCampus config found")); log_flush();
    return false;
  }

  log_debug(F("\n[luminosity] loading sensOCampus config is NOT YET IMPLEMENTED!")); log_flush();
  return false;



// TO BE CONTINUED



  // (re)load the local config file (to override default parameters values from sensOCampus)
  log_debug(F("\n[luminosity] (re)loading config file (if any)")); log_flush();
  loadConfig();

  return true;
}



/* ------------------------------------------------------------------------------
 * Private methods 
 */

/*
 * send all sensors' values
 */
boolean luminosity::_sendValues( void ) {
  /* grab and send values from all sensors
   * each sensor will result in a MQTT message
   */
  for( uint8_t cur_sensor=0; cur_sensor<_sensors_count; cur_sensor++ ) {

    StaticJsonDocument<DATA_JSON_SIZE> _doc;
    JsonObject root = _doc.to<JsonObject>();

    // retrieve data from current sensor
    root[F("value")] = (int)( _sensor[cur_sensor]->acquire() );   // [may.20] force value as INT
    root[F("value_units")] = _sensor[cur_sensor]->sensorUnits();
    root[F("subID")] = _sensor[cur_sensor]->subID();

    /*
    * send MQTT message
    */
    if( sendmsg(root) ) {
      log_info(F("\n[luminosity] successfully published msg :)")); log_flush();
    }
    else {
      // we stop as soon as we've not been able to successfully send one message
      log_error(F("\n[luminosity] ERROR failure MQTT msg delivery :(")); log_flush();
      return false;
    }
    
    // delay between two successives values to send
    delay(20); 
  }

  // ok, everything is sent
  return true;
}



/*
 * orders processing ...
 */
bool luminosity::_processOrder( const char *order, int *value ) {

  {
    const char *_order = PSTR("status");
    if( strncmp_P(order, _order, strlen_P(_order))==0 ) {
      // required to send status ... so publishing while in callback :)
      StaticJsonDocument<DATA_JSON_SIZE> _doc;
      JsonObject root = _doc.to<JsonObject>();
      status( root );
      return sendmsg( root );
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
        setFrequency( (uint16_t)(*value), LUMINOSITY_MIN_FREQUENCY, LUMINOSITY_MAX_FREQUENCY );
        StaticJsonDocument<DATA_JSON_SIZE> _doc;
        JsonObject root = _doc.to<JsonObject>();
        status( root );
        sendmsg(root);
        return saveConfig();
      }
      else return false;
    }
  }

  log_error(F("\n[luminosity][callback] unknown order: ")); log_debug(order); log_flush();
  return false;
}

/*
 * load an eventual module'specific config file
 */
bool luminosity::loadConfig( void ) {
  
  if( ! SPIFFS.exists( MODULE_CONFIG_FILE(MQTT_MODULE_NAME) ) ) return false;

  File configFile = SPIFFS.open( MODULE_CONFIG_FILE(MQTT_MODULE_NAME), "r");
  if( !configFile ) return false;

  log_info(F("\n[luminosity] load JSON config file")); log_flush();
  size_t size = configFile.size();
  // Allocate a buffer to store contents of the file.
  std::unique_ptr<char[]> buf(new char[size]);
  configFile.readBytes(buf.get(), size);
  configFile.close();

  // allocate JSON static buffer for module's config file
  StaticJsonDocument<CONFIG_JSON_SIZE> root;

  auto err = deserializeJson( root, buf.get() );
  if( err ) {
    log_error(F("\n[luminosity] ERROR parsing module JSON config file!"));
    log_error(F("\n[device] ERROR msg: ")); log_error(err.c_str()); log_flush();
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
bool luminosity::_loadConfig( JsonObject root ) {
  
  // check for 'frequency' field
  if( root.containsKey(F("frequency")) ) {
    setFrequency( (uint16_t)(root[F("frequency")].as<unsigned int>()), LUMINOSITY_MIN_FREQUENCY, LUMINOSITY_MAX_FREQUENCY );
  }

  /*
   * Parse additional fields here
   */
  
  return true;
}

/*
 * save module'specific config file
 */
bool luminosity::saveConfig( void ) {
  
  // static JSON buffer
  StaticJsonDocument<CONFIG_JSON_SIZE> _doc;
  JsonObject root = _doc.to<JsonObject>();

  // frequency
  if( _freq != (uint16_t)DEFL_LUMINOSITY_FREQUENCY )
    root[F("frequency")] = _freq;

  // add additional parameters to save here
  
  
  // call parent save
  return base::saveConfig( MODULE_CONFIG_FILE(MQTT_MODULE_NAME), root );
}


