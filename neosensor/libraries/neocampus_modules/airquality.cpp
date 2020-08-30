/*
 * neOCampus operation
 * 
 * AirQuality module to manage all kind of air quality sensors that does not
 * fit within the existing sensOCampus classes.
 * 
 * F.Thiebolt   Aug.20  initial release
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

#include "airquality.h"


/*
 * Definitons
 */
#define MQTT_MODULE_NAME        "airquality"  // used to build module's base topic
#define DATA_JSON_SIZE          (JSON_OBJECT_SIZE(20))  // for MQTT data sending
#define CONFIG_JSON_SIZE        (JSON_OBJECT_SIZE(3))   // for config FILE that contains: frequency
                                                        // note: others parameters are sent from sensOCampus
                                                        // hence not saved ;)



// constructor
airquality::airquality(): base() 
{
  _freq = DEFL_AIRQUALITY_FREQUENCY;
  for( uint8_t i=0; i < _MAX_SENSORS; i++ )
    _sensor[i] = NULL;
  
  // load json config file (if any)
  loadConfig();
}



/*
 * add_device method
 */
boolean airquality::add_sensor( uint8_t adr ) {
  
  if( _sensors_count>=_MAX_SENSORS ) return false;

  bool _sensor_added=false;
  /*
   * check for XXXX
   * TODO: add auto-detect for some air quality sensors !
   *
   * The following was left as an example:
   * 
  if( TSL2561::is_device( adr ) == true ) {
    TSL2561 *cur_sensor = new TSL2561();
    if( cur_sensor->begin( adr ) != true ) {
      log_debug(F("\n[airquality] ###ERROR at TSL2561 startup ... removing instance ..."));log_flush();
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
   */
  // add check for additional device here

  // summary
  if( !_sensor_added ) return false;

  // everything is ok :)
  return true;
}



// TODO: boolean airquality::add_sensor( JsonObject obj ) {



// check if at least one sensor exist
boolean airquality::is_empty( ) {
  return ( _sensors_count==0 ? true : false );
}



/*
 * Module network startup procedure (MQTT)
 */
bool airquality::start( senso *sensocampus ) {

    log_info(F("\n[airquality] starting module ..."));
    // initialize module's publish and subscribe topics
    snprintf( pubTopic, sizeof(pubTopic), "%s/%s", sensocampus->getBaseTopic(), MQTT_MODULE_NAME);
    snprintf( subTopic, sizeof(subTopic), "%s/%s", pubTopic, "command" );
    return base::start( sensocampus );
}



/*
 * Handler for subscribed messages
 */
void airquality::handle_msg( JsonObject root ) {
  log_info(F("\n[airquality] received COMMAND to process ...")); log_flush();
  
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
    log_error(F("\n[airquality][callback] unknown key: "));log_debug((const char*)(it->key().c_str())); log_flush();
  }
}


/*
 * process module's activites
 */
bool airquality::process( void ) {

  bool _ret = false;
  
  /* 1st step, call process from base because
   * it will check MQTT client connectivity
   * and call handler for MQTT received messages
   */
  _ret = base::process();

  /* sensors internal processing */
  _process_sensors();

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
void airquality::status( JsonObject root ) {
  
  // add base class status
  base::status( root );
    
  /*
   * TODO: add list of sensors IDs
   */
}


/*
 * load an eventual module'specific config file
 */
bool airquality::loadConfig( void ) {
  
  if( ! SPIFFS.exists( MODULE_CONFIG_FILE(MQTT_MODULE_NAME) ) ) return false;

  File configFile = SPIFFS.open( MODULE_CONFIG_FILE(MQTT_MODULE_NAME), "r");
  if( !configFile ) return false;

  log_info(F("\n[airquality] load JSON config file")); log_flush();
  size_t size = configFile.size();
  // Allocate a buffer to store contents of the file.
  std::unique_ptr<char[]> buf(new char[size]);
  configFile.readBytes(buf.get(), size);
  configFile.close();

  // allocate JSON static buffer for module's config file
  StaticJsonDocument<CONFIG_JSON_SIZE> root;

  auto err = deserializeJson( root, buf.get() );
  if( err ) {
    log_error(F("\n[airquality] ERROR parsing module JSON config file!"));
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
 * save module'specific config file
 */
bool airquality::saveConfig( void ) {
  
  // static JSON buffer
  StaticJsonDocument<CONFIG_JSON_SIZE> _doc;
  JsonObject root = _doc.to<JsonObject>();

  // frequency
  if( _freq != (uint16_t)DEFL_AIRQUALITY_FREQUENCY )
    root[F("frequency")] = _freq;

  // add additional parameters to save here
  
  
  // call parent save
  return base::saveConfig( MODULE_CONFIG_FILE(MQTT_MODULE_NAME), root );
}


/*
 * Module's sensOCampus config to load (if any)
 */
boolean airquality::loadSensoConfig( senso *sp ) {

  boolean _sensor_added = false;

  // [aug.20] ought to be >= of senso config Json ?!?!
  StaticJsonDocument<SENSO_JSON_SIZE> _doc;
  JsonArray root = _doc.to<JsonArray>();

  if( !sp->getModuleConf(MQTT_MODULE_NAME, &root) ) {
    //log_debug(F("\n[airquality] no sensOCampus config found")); log_flush();
    return false;
  }
  if( root.isNull() ) {
    log_error(F("\n[airquality] error JsonArray is null while it ought to be non empty ?!?!")); log_flush();
    return false;
  }

  //log_debug(F("\n[airquality] modulesArray was found :)\n")); log_flush();
  //serializeJsonPretty( root, Serial );

  // now parse items from array
  for( JsonVariant item : root ) {
    // check things ...
    if( not item.is<JsonObject>() or not item.containsKey(F("module")) or
        strcmp( MQTT_MODULE_NAME, item[F("module")])!=0 ) {
      log_warning(F("\n[airquality] strange ... we found a sensOCampus config that does not match our module ?!?! ... continuing")); log_flush();
    }

    // already reached MAX_SENSORS ?
    if( _sensors_count>=_MAX_SENSORS ) {
      log_warning(F("\n[airquality] already reached maximum number of sensors: ")); log_debug(_sensors_count); log_flush();
      continue;
    }

    // LCC_SENSOR
    {
      const char *_unit = PSTR("lcc_sensor");
      if( (item[F("driver")] and strncmp_P(item[F("driver")], _unit, strlen_P(_unit))==0) or
           strncmp_P(item[F("unit")], _unit, strlen_P(_unit))==0 ) {
        // instantiate sensor
        lcc_sensor *cur_sensor = new lcc_sensor();
        if( cur_sensor->begin( item[F("params")] ) != true ) {
          log_debug(F("\n[airquality] ###ERROR at lcc_sensor startup ... removing instance ..."));log_flush();
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
    }


    // add additional sensors here



    /* we'll now search for 'module' common parameters like 'unit' or 'frequency' or ...
     * no need to apply for a saveConfig() because these parameters are grabbed every reboot
     */
    {
      if( item.containsKey(F("unit")) ) {
        setIdentity( item[F("unit")] );
      }
    }

    {
      if( item.containsKey(F("frequency")) ) {
        setFrequency( item[F("frequency")].as<unsigned int>(), AIRQUALITY_MIN_FREQUENCY, AIRQUALITY_MAX_FREQUENCY );
      }
    }

  }

  // (re)load the local config file (to override default parameters values from sensOCampus)
  log_debug(F("\n[airquality] (re)loading locale config file (if any)")); log_flush();
  loadConfig();

  return _sensor_added;
}


/* ------------------------------------------------------------------------------
 * Private methods 
 */

/*
 * sensors internal processing
 * this function is called every lopp() call and leverages
 * the needs for (e.g) continuous integration.
 */
void airquality::_process_sensors( void ) {
  // process all valid sensors
  for( uint8_t cur_sensor=0; cur_sensor<_sensors_count; cur_sensor++ ) {
    _sensor[cur_sensor]->process();
  }
}


/*
 * send all sensors' values
 */
boolean airquality::_sendValues( void ) {
  /* grab and send values from all sensors
   * each sensor will result in a MQTT message
   */
  for( uint8_t cur_sensor=0; cur_sensor<_sensors_count; cur_sensor++ ) {

    StaticJsonDocument<DATA_JSON_SIZE> _doc;
    JsonObject root = _doc.to<JsonObject>();

    // retrieve data from current sensor
    float value;
    if( !_sensor[cur_sensor]->acquire(&value) ) {
      log_warning(F("\n[airquality] unable to retrieve data from sensor "));
      log_warning(_sensor[cur_sensor]->subID()); log_flush();
      continue;
    }
    root[F("value")] = (float)( value );   // [may.20] force data as float (e.g ArduinoJson converts 20.0 to INT)
    root[F("value_units")] = _sensor[cur_sensor]->sensorUnits();
    root[F("subID")] = _sensor[cur_sensor]->subID();

    /*
    * send MQTT message
    */
    if( sendmsg(root) ) {
      log_info(F("\n[airquality] successfully published msg :)")); log_flush();
    }
    else {
      // we stop as soon as we've not been able to successfully send one message
      log_error(F("\n[airquality] ERROR failure MQTT msg delivery :(")); log_flush();
      return false;
    }
    
    // delay between two successives values to send
    delay(20); 
  }

  return true;
}


/*
 * orders processing ...
 */
bool airquality::_processOrder( const char *order, int *value ) {

  if( !order ) return false;

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
        setFrequency( (uint16_t)(*value), AIRQUALITY_MIN_FREQUENCY, AIRQUALITY_MAX_FREQUENCY );
        StaticJsonDocument<DATA_JSON_SIZE> _doc;
        JsonObject root = _doc.to<JsonObject>();
        status( root );
        sendmsg(root);
        return saveConfig();
      }
      else return false;
    }
  }

  log_error(F("\n[airquality][callback] unknown order: ")); log_debug(order); log_flush();
  return false;
}


/*
 * low-level load JSON config
 */
boolean airquality::_loadConfig( JsonObject root ) {
  
  // check for 'frequency' field
  if( root.containsKey(F("frequency")) ) {
    setFrequency( (uint16_t)(root[F("frequency")].as<unsigned int>()), AIRQUALITY_MIN_FREQUENCY, AIRQUALITY_MAX_FREQUENCY );
  }

  /*
   * Parse additional fields here
   */
  
  return true;
}

/*
 * sensOCampus sample module config (see end of file sensocampus.cpp for full config)
 *

{
  "module": "airquality",
  "unit": "lcc_sensor",
  "frequency": 60,
  "params": [
    {
      "param": "subIDs",
      "value": "NO2"
    },
    {
      "param": "inputs",
      "value": [
          16,
          17,
          5,
          18,
          35
        ]
    },
    {
      "param": "outputs",
      "value": -1
    }
  ]
},
{
  "module": "airquality",
  "unit": "lcc_sensor",
  "frequency": 60,
  "params": [
    {
      "param": "subIDs",
      "value": "CO"
    },
    {
      "param": "inputs",
      "value": [
          19,
          21,
          22,
          23,
          34
        ]
    },
    {
      "param": "outputs",
      "value": -1
    }
  ]
},
{
  "module": "airquality",
  "unit": "lcc_sensor",
  "frequency": 60,
  "params": [
    {
      "param": "subIDs",
      "value": "CH20"
    },
    {
      "param": "inputs",
      "value": [
          13,
          12,
          14,
          27,
          33
        ]
    },
    {
      "param": "outputs",
      "value": 25
    }
  ]
},
{
  "module": "airquality",
  "unit": "lcc_sensor",
  "frequency": 60,
  "params": [
    {
      "param": "subIDs",
      "value": "NO2_alt"
    },
    {
      "param": "inputs",
      "value": [
          15,
          2,
          0,
          4,
          32
        ]
    },
    {
      "param": "outputs",
      "value": 26
    }
  ]
}

=== alternative (more compact) ===
{
  "module": "airquality",
  "unit": "lcc_sensor",
  "params": [
    {
      "param": "frequency",
      "value": 0
    },
    {
      "param": "subIDs",
      "value": [
        "NO2",
        "CO",
        "CH20",
        "NO2_alt"
      ]
    },
    {
      "param": "inputs",
      "value": [
        [
          16,
          17,
          5,
          18,
          35
        ],
        [
          19,
          21,
          22,
          23,
          34
        ],
        [
          13,
          12,
          14,
          27,
          33
        ],
        [
          15,
          2,
          0,
          4,
          32
        ]
      ]
    },
    {
      "param": "outputs",
      "value": [
        -1,
        -1,
        25,
        26
      ]
    }
  ]
}
*/
