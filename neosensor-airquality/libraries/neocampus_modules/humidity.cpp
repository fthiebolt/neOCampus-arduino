/*
 * neOCampus operation
 * 
 * Humidity module to manage all hygro sensors
 * 
 * Note:
 *      force data sent through MQTT as an int
 *  
 * Thiebolt.F   may.20  initial release
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



// constructor
humidity::humidity(): base() 
{
  _freq = DEFL_HUMIDITY_FREQUENCY;
  for( uint8_t i=0; i < _MAX_SENSORS; i++ )
    _sensor[i] = NULL;

  // load json config file (if any)
  loadConfig( );
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
bool humidity::start( senso *sensocampus ) {

    log_info(F("\n[humidity] starting module ..."));
    // initialize module's publish & subscribe topics
    snprintf( pubTopic, sizeof(pubTopic), "%s/%s", sensocampus->getBaseTopic(), MQTT_MODULE_NAME);
    snprintf( subTopic, sizeof(subTopic), "%s/%s", pubTopic, "command" );
    return base::start( sensocampus );
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
void humidity::status( JsonObject root ) {
  
  // add base class status
  base::status( root );
    
  /*
   * TODO: list of sensors IDs
   */
}



/* ------------------------------------------------------------------------------
 * Private methods 
 */

/*
 * send all sensors' values
 */
boolean humidity::_sendValues( void ) {
  /* grab and send values from all sensors
   * each sensor will result in a MQTT message
   */
  for( uint8_t cur_sensor=0; cur_sensor<_sensors_count; cur_sensor++ ) {

    StaticJsonDocument<DATA_JSON_SIZE> _doc;
    JsonObject root = _doc.to<JsonObject>();

    // retrieve data from current sensor
    root[F("value")] = (int)( _sensor[cur_sensor]->acquire() );     // [may.20] force humidity as INT
    root[F("value_units")] = _sensor[cur_sensor]->sensorUnits();
    root[F("subID")] = _sensor[cur_sensor]->subID();

    /*
    * send MQTT message
    */
    if( sendmsg( root ) ) {
      log_info(F("\n[humidity] successfully published msg :)")); log_flush();
    }
    else {
      // we stop as soon as we've not been able to successfully send one message
      log_error(F("\n[humidity] ERROR failure MQTT msg delivery :(")); log_flush();
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
bool humidity::_processOrder( const char *order, int *value ) {

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

