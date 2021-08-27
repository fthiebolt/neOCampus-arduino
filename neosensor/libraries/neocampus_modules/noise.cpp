/*
 * neOCampus operation
 * 
 * Noise module to detect noise according to parameters
 * 
 * Thiebolt.F may.20  force value sent throught MQTT as INT (useless but just
 *                    to get coherent with others classes) 
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

#include "noise.h"


/*
 * Definitions
 */
#define MQTT_MODULE_NAME        "noise"  // used to build module's base topic
#define DATA_JSON_SIZE          (JSON_OBJECT_SIZE(20))
#define CONFIG_JSON_SIZE        (JSON_OBJECT_SIZE(3))   // config file contains: frequency, threshold, sensitivity



/* constructor
noise::noise( uint8_t pinSensor ): base() 
{
  noise( pinSensor, (uint8_t)-1 );
}
*/

// constructor
noise::noise( uint8_t pinSensor, voidFuncPtr isr, uint8_t pinLed ): base() 
{
  _freq = DEFL_NOISE_FREQUENCY; // Reminder: frequency is not relevant for the noise module

  _pinLed = pinLed;
  if( _pinLed != INVALID_GPIO ) {
    pinMode( _pinLed, OUTPUT );
    _ledOFF();
  }

  _pinSensor = pinSensor;
  if( _pinSensor != INVALID_GPIO ) pinMode( _pinSensor, INPUT );
  
  _dac = NULL;
  sensitivity = DEFL_SENSITIVITY;
  _pulseCountThreshold = DEFL_PULSES_THRESHOLD;
  for( uint8_t i=0; i <= DEFL_TIME_WINDOW; i++)
    noiseTab[i]=0;
  curNoiseEntry = 0;
  noiseDetected = false;
  _SumNoisePulseCount = 0;
  _noiseDetectISR = isr;

  // load json config file (if any)
  loadConfig( );
}



/*
 * add_dac method
 */
boolean noise::add_dac( uint8_t adr ) {
  /* only one dac support for now :| */
  if( _dac != NULL ) return false;

  // add check for additional device here

  /* 
   * check for MCP47FEB
   */
  if( MCP47FEB::is_device( adr ) == true ) {
    MCP47FEB *cur_dac = new MCP47FEB();
    /* DAC setup for neOCampus sensor: 
     *  Gain=1, 
     *  Voltage reference is Vref unbuffered (VCC/2)
     */
    if( cur_dac->begin( adr ) != true ) {
      log_error(F("\n[noise] ###ERROR at MCP47FEB startup ... removing instance ..."));log_flush();
      free(cur_dac);
      cur_dac = NULL;
    }
    else {
      cur_dac->setGain( MCP47FEB_GAIN_1X );
      cur_dac->setVRef( MCP47FEB_VREF_VREFPIN );
      _dac = cur_dac;
      _sensors_count++;
    }
  }    
  /* check for MCP47X6 [ought to be the last one]
   * Note: this one is not readable from I2C so it is almost impossible
   * to assert what kind of device it is except its i2c addr :|
   */
  else if( MCP47X6::is_device( adr ) == true ) {
    MCP47X6 *cur_dac = new MCP47X6();
    /* DAC setup for neOCampus sensor: 
     *  Gain=1, 
     *  Voltage reference is Vref unbuffered (VCC/2)
     */
    if( cur_dac->begin( adr ) != true ) {
      log_error(F("\n[noise] ###ERROR at MCP47X6 startup ... removing instance ..."));log_flush();
      free(cur_dac);
      cur_dac = NULL;
    }
    else {
      cur_dac->setGain( MCP47X6_GAIN_1X );
      cur_dac->setVRef( MCP47X6_VREF_VREFPIN );
      _dac = cur_dac;
      _sensors_count++;
    }
  }

  // we now have a DAC :)
  if( _dac == NULL ) return false;

  // applying sensitivity level to our DAC
  setSensitivity( sensitivity );

  // everything is ok :)
  return true;
}



// check if at least one DAC exist
boolean noise::is_empty( ) {
  // only support for a single DAC anyway ;)
  return ( _dac==NULL ? true : false );
}



/*
 * NoiseMeter: setting percent sensitivity (0 --> 100%)
 */
uint8_t noise::setSensitivity( uint8_t percent ) {
  // check argument
  if( percent < SENSITIVITY_MIN ) percent = SENSITIVITY_MIN;
  else if( percent > SENSITIVITY_MAX ) percent = SENSITIVITY_MAX;

  log_debug(F("\n[noise] sensitivity set to ")); log_debug(percent,DEC); log_debug(F("%")); log_flush();
  
  // set DAC percent output
  sensitivity = _dac->setPercentOutput( percent );
  return sensitivity;
}



/*
 * Noise detection parameters:
 * Maximum pulses allowed over the whole sliding window
 * before we declare that noise has been detected.
 */
bool noise::setThreshold( uint16_t threshold ) {
  if( threshold < THRESHOLD_MIN ) {
    threshold = THRESHOLD_MIN;
  }
  else if( threshold > THRESHOLD_MAX ) {
    threshold = THRESHOLD_MAX;
  }

  _pulseCountThreshold = threshold;

  log_debug(F("\n[noise] pulses threshold set to ")); log_debug(_pulseCountThreshold,DEC); log_flush();
  return true;
}


/*
 * Module network startup procedure (MQTT)
 */
bool noise::start( senso *sensocampus, JsonDocument &sharedRoot ) {

  log_info(F("\n[noise] starting module ..."));
  // create module's JSON structure to hold all of our data
  // [aug.21] we create a dictionnary
  variant = sharedRoot.createNestedObject(MQTT_MODULE_NAME);

  // initialize module's publish & subscribe topics
  snprintf( pubTopic, sizeof(pubTopic), "%s/%s", sensocampus->getBaseTopic(), MQTT_MODULE_NAME);
  snprintf( subTopic, sizeof(subTopic), "%s/%s", pubTopic, "command" );
  
  // start timer and pulse count interrupt handler 
  pulseT.attach( 1, timerHandler, this );
  
  //mqttClient.setCallback( [this] (char* topic, byte* payload, unsigned int length) { this->callback(topic, payload, length); });
  //attachInterrupt(digitalPinToInterrupt(_pinSensor), noiseDetectISR, FALLING);
  //attachInterrupt(digitalPinToInterrupt(_pinSensor), [this] ( void ) { this->noiseDetectISR(void); }, FALLING);
  //attachInterrupt(digitalPinToInterrupt(_pinSensor), std::bind(&noise::noiseDetectISR,[this]), FALLING);
  //attachInterrupt(digitalPinToInterrupt(_pinSensor), std::function<void(void)> f = std::bind(&noise::noiseDetectISR, this), FALLING);
  //std::function<void(int,int)> f = [this](int a, int b) { this->noiseDetectISR(); }
  //std::function<void(void)> f = [this](void) { this->noiseDetect2ISR(void); };
  //attachInterrupt(digitalPinToInterrupt(_pinSensor), f, FALLING);
  
  //std::function<void()> func = [&]{noiseModule->noiseDetectISR();};

  attachInterrupt(digitalPinToInterrupt(_pinSensor),
                  _noiseDetectISR,
                  FALLING);
  
  // call to start from base class
  return base::start( sensocampus, sharedRoot );
}


/*
 * Handler for subscribed messages
 */
void noise::handle_msg( JsonObject root ) {
  log_info(F("\n[noise][callback] received COMMAND to process ...")); log_flush();

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
    log_error(F("\n[noise][callback] unknown key: "));log_debug((const char*)(it->key().c_str())); log_flush();
  }
}


/*
 * process module's activites
 */
bool noise::process( void ) {

  bool _ret = false;
  
  /* 1st step, call process from base because
   * it will check MQTT client connectivity
   * and call handler for MQTT received messages
   */
  _ret = base::process();

  // reached time to transmit ?
  if( !isTXtime() and !noiseDetected ) return _ret;

  /*
   * Time to send a new message
   */
  // check that at least one DAC is available
  if( !_dac ) return false;
  
  StaticJsonDocument<DATA_JSON_SIZE> _doc;
  JsonObject root = _doc.to<JsonObject>();

  if( noiseDetected ) {
    /* add relevant values to MQTT msg related
     * to the noise that has been detected
     */
    noiseDetectedMsg( root );
    
    // ... and disable noise detection flag
    noiseDetected = false;
  }
  else {
    /* add items to JSON object:
    * - current status
    */
    status( root );
  }
  
  /*
   * send MQTT message
   */
  if( !sendmsg(root) ) {
    log_error(F("\n[noise] ERROR failure MQTT msg delivery :(")); log_flush();
    return false;
  }
  
  log_info(F("\n[noise] successfully published msg :)")); log_flush();
  return true;
}


/*
 * Sliding Window for noise pulse count:
 * timer handler method called EVERY SECOND
 */
void ICACHE_RAM_ATTR noise::timerHandler( noise *p ) {

  uint8_t _curNoiseEntry, nextNoiseEntry;
  
  // increment curNoiseEntry
  if( p->curNoiseEntry == DEFL_TIME_WINDOW ) {
    nextNoiseEntry = 0;
  }
  else {
    nextNoiseEntry = p->curNoiseEntry + 1;
  }
  
  /*
   * compute sum of noiseTab (clever way ;)
   * - remove noiseTab[next_entry] from sum (circular buffer)
   * - affect curNoiseEntry with next
   * - add previous to sum
   */
  p->_SumNoisePulseCount -= p->noiseTab[nextNoiseEntry];
  p->noiseTab[nextNoiseEntry] = 0;
  _curNoiseEntry = p->curNoiseEntry;
  p->curNoiseEntry = nextNoiseEntry;   // ISR will immediately update
  p->_SumNoisePulseCount += p->noiseTab[_curNoiseEntry];
  
  //log_debug(F("\n[noise][timer] noiseTab["));log_debug(_curNoiseEntry,DEC); log_debug(F("] = "));log_debug(p->noiseTab[_curNoiseEntry],DEC);log_flush();
  
  // check for noise _pulse_count_threshold
  if( p->_SumNoisePulseCount >= p->_pulseCountThreshold ) {
    p->noiseDetected = true;
    p->_ledON();
  }
  else {
    p->_ledOFF();
  }
}


/*
 * noise detect ISR
 * This method called upon interrup on _pinSensor
 * will increment noiseTab[curNoiseEntry]
 */
void ICACHE_RAM_ATTR noise::noiseDetectISR( void ) {

  // static uint8_t _curNoiseEntry = -1;
  
  /*
   * this method is called because _pinSensor interrupt was activated
   */
  
  /* check for change in noiseEntry
  if( _curNoiseEntry != curNoiseEntry ) {
    _curNoiseEntry = curNoiseEntry;
    noiseTab[curNoiseEntry] = 1;
  }
  else */
  if( noiseTab[curNoiseEntry] < (uint8_t)-1 ) {
    // only increment if not already at maximum
    noiseTab[curNoiseEntry]++;
  }
}


/*
 * Noise detected report
 */
void noise::noiseDetectedMsg( JsonObject root ) {
  // OK, so noise has been detected
  root[F("value")] = (int)( _SumNoisePulseCount );  // number of pulses counted across the sliding window
  root[F("value_units")] = F("pulses");       // nature of 'value'
  root[F("input")] = _pinSensor;           // input used to count pulses
  root[F("subID")] = _dac->subID();        // i2c addr of DAC
}


/*
 * Status report sending
 */
void noise::status( JsonObject root ) {
  
  // add base class status
  base::status( root );

  // noise module parameters
  root[F("sensitivity")] = sensitivity;
  root[F("_scan_window")] = DEFL_TIME_WINDOW;
  root[F("threshold")] = _pulseCountThreshold;
}


/*
 * Module's sensOCampus config to load (if any)
 */
boolean noise::loadSensoConfig( senso *sp ) {

  JsonArray _array;
  if( ! sp->getModuleConf( MQTT_MODULE_NAME, _array ) ) {
    //log_debug(F("\n[noise] no sensOCampus config found")); log_flush();
    return false;
  }

  log_debug(F("\n[noise] loading sensOCampus config is NOT YET IMPLEMENTED!")); log_flush();
  return false;



// TO BE CONTINUED



  // (re)load the local config file (to override default parameters values from sensOCampus)
  log_debug(F("\n[noise] (re)loading config file (if any)")); log_flush();
  loadConfig();

  return true;
}


/* ------------------------------------------------------------------------------
 * Private methods 
 */

/*
 * LED ON / OFF
 */
void inline noise::_ledON( void ) {
  if( _pinLed != INVALID_GPIO ) {
    // [aug.21] NOISE_LED is shared with LED and CLEAR_SW that may change its mode
    // hence we ensure that mode is output ;)
    pinMode( _pinLed, OUTPUT );
    digitalWrite( _pinLed, HIGH );
  }
}
void inline noise::_ledOFF( void ) {
  if( _pinLed != INVALID_GPIO ) {
    // [aug.21] NOISE_LED is shared with LED and CLEAR_SW that may change its mode
    // hence we ensure that mode is output ;)
    pinMode( _pinLed, OUTPUT );
    digitalWrite( _pinLed, LOW );
  }
}


/*
 * orders processing ...
 */
bool noise::_processOrder( const char *order, int *value ) {

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
      StaticJsonDocument<DATA_JSON_SIZE> _doc;
      JsonObject root = _doc.to<JsonObject>();
      noiseDetectedMsg( root );
      return sendmsg( root );
    }
  }

  {
    const char *_order = PSTR("frequency");
    if( strncmp_P(order, _order, strlen_P(_order))==0 ) {
      if( value ) {
        setFrequency( (uint16_t)(*value), NOISE_MIN_FREQUENCY, NOISE_MAX_FREQUENCY );
        StaticJsonDocument<DATA_JSON_SIZE> _doc;
        JsonObject root = _doc.to<JsonObject>();
        status( root );
        sendmsg( root );
        return saveConfig();
      }
      else return false;
    }
  }

  {
    const char *_order = PSTR("sensitivity");
    if( strncmp_P(order, _order, strlen_P(_order))==0 ) {
      if( value ) {
        setSensitivity( (uint8_t)(*value) );
        StaticJsonDocument<DATA_JSON_SIZE> _doc;
        JsonObject root = _doc.to<JsonObject>();
        status( root );
        sendmsg( root );
        return saveConfig();
      }
      else return false;
    }
  }
  
  {
    const char *_order = PSTR("threshold");
    if( strncmp_P(order, _order, strlen_P(_order))==0 ) {
      if( value ) {
        setThreshold( (uint16_t)(*value) );
        StaticJsonDocument<DATA_JSON_SIZE> _doc;
        JsonObject root = _doc.to<JsonObject>();
        status( root );
        sendmsg( root );
        return saveConfig();
      }        
      else return false;
    }
  }

  log_error(F("\n[noise][callback] unknown order: ")); log_debug(order); log_flush();
  return false;
}

/*
 * load an eventual module'specific config file
 */
bool noise::loadConfig( void ) {
  
  if( ! SPIFFS.exists( MODULE_CONFIG_FILE(MQTT_MODULE_NAME) ) ) return false;

  File configFile = SPIFFS.open( MODULE_CONFIG_FILE(MQTT_MODULE_NAME), "r");
  if( !configFile ) return false;

  log_info(F("\n[noise] load JSON config file")); log_flush();
  size_t size = configFile.size();
  // Allocate a buffer to store contents of the file.
  std::unique_ptr<char[]> buf(new char[size]);
  configFile.readBytes(buf.get(), size);
  configFile.close();

  // allocate JSON static buffer for module's config file
  StaticJsonDocument<CONFIG_JSON_SIZE> root;

  auto err = deserializeJson( root, buf.get() );
  if( err ) {
    log_error(F("\n[noise] ERROR parsing module JSON config file!"));
    log_error(F("\n[noise] ERROR msg: ")); log_error(err.c_str()); log_flush();
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
bool noise::_loadConfig( JsonObject root ) {
  
  /* check for 'frequency' field
   * Note: remember that frequency is not relevant for the noise module
   */
  if( root.containsKey(F("frequency")) ) {
    setFrequency( (uint16_t)(root[F("frequency")].as<unsigned int>()), NOISE_MIN_FREQUENCY, NOISE_MAX_FREQUENCY );
  }

  // check for 'threshold' field
  if( root.containsKey(F("threshold")) ) {
    setThreshold( (uint16_t)(root[F("threshold")].as<unsigned int>()) );
  }
  
  // check for 'sensitivity' field
  if( root.containsKey(F("sensitivity")) ) {
    setThreshold( (uint8_t)(root[F("sensitivity")].as<unsigned int>()) );
  }

  /*
   * Parse additional fields here
   */
  
  return true;
}

/*
 * save module'specific config file
 */
bool noise::saveConfig( void ) {
  
  // static JSON buffer
  StaticJsonDocument<CONFIG_JSON_SIZE> _doc;
  JsonObject root = _doc.to<JsonObject>();

  // frequency
  if( _freq != (uint16_t)DEFL_NOISE_FREQUENCY )
    root[F("frequency")] = _freq;

  // sensitivity
  if( sensitivity != (uint8_t)DEFL_SENSITIVITY )
    root[F("sensitivity")] = sensitivity;
  
  // threshold
  if( _pulseCountThreshold != (uint16_t)DEFL_PULSES_THRESHOLD )
    root[F("threshold")] = _pulseCountThreshold;
  
  
  // add additional parameters to save here
  
  
  // call parent save
  return base::saveConfig( MODULE_CONFIG_FILE(MQTT_MODULE_NAME), root );
}

