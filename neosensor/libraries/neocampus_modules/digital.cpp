/*
 * neOCampus operation
 * 
 * Digital module to manage all kind of digital inputs.
 * Unlike others module, this one does not feature automatic sending
 * of sensors' data on a time interval basis but on configurable fronts
 * detection.
 * 
 * F.Thiebolt   Aug.21  initial release
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

#include "digital.h"


/*
 * Definitons
 */
#define MQTT_MODULE_NAME        "digital"  // used to build module's base topic
#define DATA_JSON_SIZE          (JSON_OBJECT_SIZE(20))  // for MQTT data sending
#define CONFIG_JSON_SIZE        (JSON_OBJECT_SIZE(3))   // for config FILE that contains: frequency
                                                        // note: others parameters are sent from sensOCampus
                                                        // hence not saved ;)



// constructor
digital::digital( void ): base() {
  // call low-level constructor
  _constructor();
}

// low-level constructor
void digital::_constructor( void ) {

  for( uint8_t i=0; i < _MAX_GPIOS; i++ ) {
    _gpio[i] = nullptr;
  }

  // initialize total count of registered GPIOs
  _gpio_count = 0;

  // [aug.21] reset module trigger
  // ... ought to get done in base constructor
  //_trigger = false;

  // load json config file (if any)
  loadConfig();
}


// destructor
digital::~digital( void )
{
  for( uint8_t i=0; i < _MAX_GPIOS; i++ ) {
    if( _gpio[i] == nullptr ) continue;
    free(_gpio[i]);
    _gpio[i] = nullptr;
  }
}



/*
 * add_gpio method
 * Note: specifying front as 'none' will not send their value through MQTT
 */
boolean digital::add_gpio( const char* subID, uint8_t pin, digitalInputType_t type, digitalFrontDetect_t front, uint16_t coolDown ) {
  
  if( subID==nullptr or strlen(subID)==0 ) return false;
  if( pin == INVALID_GPIO ) return false;
  if( _gpio_count>=_MAX_GPIOS ) return false;

//  bool _gpio_added=false;
  digitalGPIO_t *_cur_gpio = nullptr;

  // search for an already existing structure
  for( uint8_t i=0; i < _gpio_count; i++ ) {
    if( _gpio[i]==nullptr || _gpio[i]->pin!=pin ) continue;
    // gpio already exists
    _cur_gpio = _gpio[i];
    break;
  }

  // allocate structure if needed
  if( _cur_gpio == nullptr ) {
    _gpio[_gpio_count] = new digitalGPIO_t;
    if( _gpio[_gpio_count] == nullptr ) {
      log_error(F("\n[digital] unable to allocate digitalGPIO_t instance ?!?!")); log_flush();
      return false;
    }
    _cur_gpio = _gpio[_gpio_count];
  }
  else {
    log_warning(F("\n[digital] GPIO"));log_warning(pin);
    log_warning(F(" already exists! >>>> UPDATING <<<< ...")); log_flush();
    delay(500);
  }

  strncpy( _cur_gpio->subID, subID, sizeof(_cur_gpio->subID));
  _cur_gpio->subID[sizeof(_cur_gpio->subID)-1] = '\0';

  _cur_gpio->pin         = pin;
  _cur_gpio->type        = type;
  _cur_gpio->front       = front;
  _cur_gpio->_trigger    = false;
  _cur_gpio->coolDown    = coolDown;
  _cur_gpio->_lastTX     = millis() - ((unsigned long)coolDown*1000UL);

  // initialize values. _previous field will get used in process()
  pinMode( pin, INPUT );
  _cur_gpio->_current    = digitalRead( pin );
  _cur_gpio->value       = _cur_gpio->_current;
/*
  _gpio_added = true;

  // summary
  if( !_gpio_added ) {
    free(_gpio[_gpio_count]);
    _gpio[_gpio_count] = nullptr;
    _cur_gpio = nullptr;
    return false;
  }
*/
  log_debug(F("\n[digital] added '"));log_debug(_cur_gpio->subID);
  log_debug(F("' GPIO"));log_debug(_cur_gpio->pin);
  log_debug(F(" front="));log_debug((uint8_t)_cur_gpio->front);
  log_debug(F(" coolDown="));log_debug(_cur_gpio->coolDown,DEC);
  log_flush();

  _gpio_count++;

  // everything is ok :)
  return true;
}


/**************************************************************************/
/*! 
    @brief  Extract JSON config parameters to initialize the GPIO
    @note   may get called as many times as modules specified
*/
/**************************************************************************/
boolean digital::add_gpio( JsonVariant root ) {

  // check input(s)
  if( root.isNull() or not root.is<JsonArray>() ) {
    log_error(F("\n[digital] either empty JSON or wrong JSON type (array expected)!")); log_flush();
    return false;
  }

  //log_debug(F("\n[lcc_sensor] params found :)\n")); log_flush();
  //serializeJsonPretty( root, Serial );

  const char *_subID          = nullptr;
  uint8_t _input              = INVALID_GPIO;
  digitalInputType_t _type    = digitalInputType_t::undefined;
  uint16_t _cooldown          = 0;
  bool _front_param           = false;
  digitalFrontDetect_t _front = digitalFrontDetect_t::none;

  /* parse all parameters of our sensor:
  [
    {
      "param": "subID",
      "value": "IRsensor"
    },
    {
      "param": "input",
      "value": 4
    },
    {
      "param": "type",
      "value": 1
    },
    {
      "param": "cooldown",
      "value": 60
    },
    {
      "param": "front",
      "value": 1
    }
  ]
  */

  for( JsonVariant item : root.as<JsonArray>() ) {

    if( item.isNull() or not item.is<JsonObject>() ) {
      log_warning(F("\n[digital] format error while parsing parameters !")); log_flush();
      continue;
    }

    // SUBID
    {
      const char *_param = PSTR("subID");
      if( strncmp_P(item[F("param")], _param, strlen_P(_param))==0 ) {
        _subID = item[F("value")];
        // _subID = item[F("value")].as<String>();
      }
    }

    // INPUT
    {
      const char *_param = PSTR("input");
      if( strncmp_P(item[F("param")], _param, strlen_P(_param))==0 ) {
        _input = (uint8_t)item[F("value")].as<int>();   // to force -1 to get converted to (uint8_t)255
      }
    }

    // TYPE
    {
      const char *_param = PSTR("type");
      if( strncmp_P(item[F("param")], _param, strlen_P(_param))==0 ) {
        _type = (digitalInputType_t)item[F("value")].as<int>();
      }
    }

    // COOLDOWN
    {
      const char *_param = PSTR("cooldown");
      if( strncmp_P(item[F("param")], _param, strlen_P(_param))==0 ) {
        _cooldown = (uint16_t)item[F("value")].as<int>();
      }
    }

    // FRONT
    {
      const char *_param = PSTR("front");
      if( strncmp_P(item[F("param")], _param, strlen_P(_param))==0 ) {
        _front = (digitalFrontDetect_t)item[F("value")].as<int>();
        _front_param = true;
      }
    }

  }


  /* DEBUG DEBUG DEBUG
  log_debug(F("\n[digital][sensOCampus] add gpio with subID: ")); log_debug(_subID);
  log_debug(F(" GPIO")); log_debug(_input,DEC);
  log_debug(F(" type: ")); log_debug((uint8_t)_type,DEC);
  log_debug(F(" front: ")); log_debug((uint8_t)_front,DEC);
  log_debug(F(" cooldown: ")); log_debug(_cooldown,DEC);
  log_flush();
  return false;
  */

  /* check whether all parameters are set ...
   */
  if( !_front_param ) return false;

  /*
   * sensor HW initialisation
   */
  return add_gpio( _subID, _input, _type, _front, _cooldown );
}



// check if at least one sensor exist
boolean digital::is_empty( ) {
  return ( _gpio_count==0 ? true : false );
}



/*
 * Module network startup procedure (MQTT)
 */
bool digital::start( senso *sensocampus, JsonDocument &sharedRoot ) {

  log_info(F("\n[digital] starting module ..."));
  // create module's JSON structure to hold all of our data
  // [aug.21] we create a dictionnary
  variant = sharedRoot.createNestedObject(MQTT_MODULE_NAME);

  // initialize module's publish and subscribe topics
  snprintf( pubTopic, sizeof(pubTopic), "%s/%s", sensocampus->getBaseTopic(), MQTT_MODULE_NAME);
  snprintf( subTopic, sizeof(subTopic), "%s/%s", pubTopic, "command" );
  return base::start( sensocampus, sharedRoot );
}



/*
 * Handler for subscribed messages
 */
void digital::handle_msg( JsonObject root ) {
  log_info(F("\n[digital] received COMMAND to process ...")); log_flush();
  
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
    log_error(F("\n[digital][callback] unknown key: "));log_debug((const char*)(it->key().c_str())); log_flush();
  }
}



/*
 * process module's activites
 * [aug.21] unlike others sensors, digital sensor values are sent immediately
 * according to their front type, coolDown and lastTX own value
 */
bool digital::process( void ) {

  bool _ret = false;
  
  /* 1st step, call process from base because
   * it will check MQTT client connectivity
   * and call handler for MQTT received messages
   */
  _ret = base::process();

  /* sensors internal processing */
  _process_sensors();

  // [aug.21] TXtime is not based on timer but upon digital inputs events
  // reached time to transmit ?
  //if( !isTXtime() ) return _ret;

  // [aug.21] if global trigger has been activated, we'll parse all inputs
  // to check for individual triggers
  if( !_trigger ) return _ret;

  /*
   * Time to send a new message
   */
  // check that at least one sensor is available
  if( _gpio_count==0 ) return false;
  
  // send all sensors' values
  return _sendValues();
}


/*
 * Status report sending
 */
void digital::status( JsonObject root ) {
  
  // add base class status
  base::status( root );
  
  // add module specific
  root[F("inputs")] = _gpio_count;

  /*
   * TODO: add list of sensors IDs
   */

}



/*
 * load an eventual module'specific config file
 */
bool digital::loadConfig( void ) {
  return false;
#if 0  
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
#endif /* 0 */
}


/*
 * save module'specific config file
 */
bool digital::saveConfig( void ) {
  return false;
#if 0
  // static JSON buffer
  StaticJsonDocument<CONFIG_JSON_SIZE> _doc;
  JsonObject root = _doc.to<JsonObject>();

  // frequency
  if( _freq != (uint16_t)DEFL_AIRQUALITY_FREQUENCY )
    root[F("frequency")] = _freq;

  // add additional parameters to save here
  
  
  // call parent save
  return base::saveConfig( MODULE_CONFIG_FILE(MQTT_MODULE_NAME), root );
#endif /* 0 */
}



/*
 * Module's sensOCampus config to load (if any)
 * Note: only called ONE TIME ... hence we support dynamic allocation :)
 */
boolean digital::loadSensoConfig( senso *sp ) {
#if 0
  // [aug.21] experiment without data duplication
  JsonObject _obj;
  JsonArray::iterator _iter;

  // initialize iterator
  if( !sp->getModuleConf2(MQTT_MODULE_NAME, &_obj, &_iter) ) {
    log_debug(F("\n[digital] no sensOCampus config found")); log_flush();
    return false;
  }

  // loop to iterate over all of our modules' config
  //while( !sp->getModuleConf2(MQTT_MODULE_NAME, _array, _iter) )

  if( _obj.isNull() ) {
    log_error(F("\n[digital] error JsonArray is null while it ought to be non empty ?!?!")); log_flush();
    return false;
  }

  log_info(F("\n[digital] FOUND SENSOCAMPUS config to parse :) ... CONTINUING")); log_flush();
  return false;
#endif /* 0 */
  boolean _sensor_added = false;

  // [aug.20] ought to be >= of senso config Json ?!?!
  //StaticJsonDocument<SENSO_JSON_SIZE> _doc;   // [aug.21] crash on esp8266 !
  DynamicJsonDocument _doc(SENSO_JSON_SIZE);  
  JsonArray root = _doc.to<JsonArray>();

  if( !sp->getModuleConf(MQTT_MODULE_NAME, root) ) {
    //log_debug(F("\n[digital] no sensOCampus config found")); log_flush();
    return false;
  }

  if( root.isNull() ) {
    log_error(F("\n[digital] error JsonArray is null while it ought to be non empty ?!?!")); log_flush();
    return false;
  }

  //log_debug(F("\n[digital] modulesArray was found :)\n")); log_flush();
  //serializeJsonPretty( root, Serial );

  // now parse items from array
  for( JsonVariant item : root ) {
    // check things ...
    if( not item.is<JsonObject>() or not item.containsKey(F("module")) or
        strcmp( MQTT_MODULE_NAME, item[F("module")])!=0 ) {
      log_warning(F("\n[digital] strange ... we found a sensOCampus config that does not match our module ?!?! ... continuing")); log_flush();
    }

    // already reached MAX_GPIO ?
    if( _gpio_count>=_MAX_GPIOS ) {
      log_warning(F("\n[digital] already reached maximum number of sensors: ")); log_debug(_gpio_count); log_flush();
      continue;
    }

    // GPIO driver
    {
      const char *_unit = PSTR("gpio");
      if( (item[F("driver")] and strncmp_P(item[F("driver")], _unit, strlen_P(_unit))==0) or
           strncmp_P(item[F("unit")], _unit, strlen_P(_unit))==0 ) {
        // add gpio
        if( add_gpio( item[F("params")] )!= true ) {
          log_debug(F("\n[digital] unable to add_gpio from sensOCampus config ..."));log_flush();
        }
        else {
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
  }

  // (re)load the local config file (to override default parameters values from sensOCampus)
  log_debug(F("\n[digital] (re)loading locale config file (if any)")); log_flush();
  loadConfig();

  return _sensor_added;
}


/* ------------------------------------------------------------------------------
 * Private methods 
 */

/*
 * digital inputs internal processing
 * this function is called every lopp() call and leverages
 * the needs for (e.g) continuous integration.
 */
void digital::_process_sensors( void ) {
  // same time for all inputs ...
  unsigned long curTime = millis();

  // declare pointer to shared JSON
  JsonObject _obj = variant.as<JsonObject>();

  // process all digital inputs
  for( uint8_t i=0; i < _gpio_count; i++ ) {

    if( _gpio[i]==nullptr || _gpio[i]->pin==INVALID_GPIO ) continue;

    bool _xor, _value;
    // first, save previous value
    _gpio[i]->_previous = _gpio[i]->_current;
    // then read actual value
    pinMode( _gpio[i]->pin, INPUT );
    _gpio[i]->_current = digitalRead( _gpio[i]->pin );
    // does pin value changed over last acquisition ?
    _xor = _gpio[i]->_current ^ _gpio[i]->_previous;
    /* Now let's compute new official digital input value:
     * pin ought to get stable over two consecutives measure ...
     * ==> hence if xor=1, we keep the previous official (stable) value, otherwise
     * the current value WILL become the new official one.
     */
    _value = (_gpio[i]->value & _xor) | (_gpio[i]->_current & ~_xor);

    // update shared JSON structure if new official value (trigger independant)
    if( _value ^ _gpio[i]->value ) {
      //String _key = "GPIO" + _gpio[i]->pin;
      //_obj[_key] = _value;
      _obj[_gpio[i]->subID] = _value;
    }

    // if the new official digital inputs changed from previous one AND trigger is not already active
    if( (_value ^ _gpio[i]->value) && not(_gpio[i]->_trigger) ) {
      bool _fdetect, _isTXtime;
      // ok, a change has been officially detected ...
      // but do we need to declare a trigger ?
      _fdetect =  (_value==true && _gpio[i]->front==digitalFrontDetect_t::rising) ||
                  (_value==false && _gpio[i]->front==digitalFrontDetect_t::falling) ||
                  _gpio[i]->front==digitalFrontDetect_t::both;

      log_debug(F("\n[digital] event "));log_debug(_gpio[i]->subID);
      log_debug(F("\n(GPIO"));log_debug(_gpio[i]->pin);
      log_debug(F(")="));log_debug(_value);
      log_debug(F(" _fdetect="));log_debug(_fdetect);
      log_flush();

      // time to transmit ?
      _isTXtime = (curTime - _gpio[i]->_lastTX) >= ((unsigned long)_gpio[i]->coolDown)*1000UL;

      if( _fdetect && _isTXtime ) {
        _gpio[i]->_trigger = true;
        _trigger = true;  // notify global module's trigger
      }
      // ... and finally save the new official value :)
      _gpio[i]->value = _value;
    }
  }
}



/*
 * send all sensors' values
 */
boolean digital::_sendValues( void ) {
  /* grab and send values from all sensors
   * each sensor will result in a MQTT message
   */
  bool _TXoccured = false;

  for( uint8_t i=0; i<_gpio_count; i++ ) {

    if( _gpio[i]==nullptr || not _gpio[i]->_trigger ) continue;

    StaticJsonDocument<DATA_JSON_SIZE> _doc;
    JsonObject root = _doc.to<JsonObject>();

    // retrieve data from current sensor
    bool _value = _gpio[i]->value;
    root[F("value")] = _value;
    root[F("input")] = _gpio[i]->pin;

    if( _gpio[i]->type == digitalInputType_t::presence ) { root[F("type")] = "presence"; }
    else if( _gpio[i]->type == digitalInputType_t::on_off ) { root[F("type")] = "on_off"; }
    else if( _gpio[i]->type == digitalInputType_t::open_close ) { root[F("type")] = "open_close"; }
    else {
      // last chance ...
      log_warning(F("\n[digital] unsupported type :")); log_warning((uint8_t)_gpio[i]->type,DEC); log_flush();
      root[F("type")] = "unknown";
    }
    root[F("subID")] = _gpio[i]->subID;

    /*
    * send MQTT message
    */
    if( sendmsg(root) ) {
      log_debug(F("\n[digital] successfully published msg pin"));
      log_debug(_gpio[i]->pin,DEC); log_debug(F(" = ")); log_debug(_value,DEC); log_flush();
      _gpio[i]->_trigger = false;
      _gpio[i]->_lastTX = millis();
      _TXoccured = true;
    }
    else {
      // we stop as soon as we've not been able to successfully send one message
      log_error(F("\n[digital] ERROR failure MQTT msg delivery :(")); log_flush();
      return false;
    }
    
    // delay between two successives values to send
    delay(20); 
  }

  /* do we need to postpone to next TX slot:
   * required when no data at all have been published
   * [aug.21] useless for digital module ...
   */
  if( !_TXoccured ) {
    /* DEBUG DEBUG DEBUG */
    log_debug(F("\n[digital] postponed to next TX slot ...")); log_flush();
    delay(100);
    #warning "remove me !!!"
    
    cancelTXslot();
  }
  else {
    // ... and cancel module's trigger
    _trigger = false;
  }

  return true;
}


/*
 * orders processing ...
 */
bool digital::_processOrder( const char *order, int *value ) {

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

  log_error(F("\n[digital][callback] unknown order: ")); log_debug(order); log_flush();
  return false;
}


/*
 * low-level load JSON config
 */
boolean digital::_loadConfig( JsonObject root ) {
  return false;
#if 0  
  // check for 'frequency' field
  if( root.containsKey(F("frequency")) ) {
    setFrequency( (uint16_t)(root[F("frequency")].as<unsigned int>()), AIRQUALITY_MIN_FREQUENCY, AIRQUALITY_MAX_FREQUENCY );
  }

  /*
   * Parse additional fields here
   */
  
  return true;
#endif /* 0 */
}

/*
 * sensOCampus sample module config (see end of file sensocampus.cpp for full config)
 *
[
	{
		"topic": "irit2/366",
		"modules":
		[
			{
				"module": "digital",
        "unit": "inside",
        "driver": "gpio",
				"params":
				[
					{
						"param": "subID",
						"value": 	"IRsensor"
					},
					{
						"param": "input",
						"value": 	4
					},
					{
						"param": "type",
						"value": 1
					},
					{
						"param": "cooldown",
						"value": 60
					},
					{
						"param": "front",
						"value": 1
					}
				]
			}
		]
	}
]
*/