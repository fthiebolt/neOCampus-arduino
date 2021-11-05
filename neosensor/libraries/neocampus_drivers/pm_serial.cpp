/**************************************************************************/
/*! 
  @file     pms_serial.cpp
  @author   F.Thiebolt (neOCampus / Univ.Tlse3)
  @license
	
	PMS 5003 & derivatives Airquality sensors driver for neOCampus

	@section  HISTORY

    oct.21  F.thiebolt  initial release
*/
/**************************************************************************/


/*
 * Includes
 */
#include <Arduino.h>

#include "neocampus.h"
#include "neocampus_debug.h"

#include "pm_serial.h"   // neOCampus driver


/* 
 * Definitions
 */
#define _MEASURES_INTERLEAVE_MS     DEFL_READ_MSINTERVAL  // delay between two measures in the 'measuring' state
#define _MAX_MEASURES               (uint8_t)5            // max. number of measures during a single campaign


/* declare kind of units (value_units) */
const char *pm_serial::units = "µg/m3";
/* declare subIDs */
const char *pm_serial::subID_pm2_5  = "pm2_5";
const char *pm_serial::subID_pm10   = "pm10";


/**************************************************************************/
/*! 
    @brief  Instantiates a new pms_serial class
    Note: we're setting generic_driver specific parameters
*/
/**************************************************************************/
pm_serial::pm_serial( void ) : generic_driver( _MEASURES_INTERLEAVE_MS,
                                               _MAX_MEASURES ) {
  _initialized = false;

  _stream = nullptr;
  _link = SENSORS_SERIAL_LINK;        // serial link number (e.g 2 --> Serial2)
  _link_speed = PM_DEFL_LINK_SPEED;   // serial link bauds rate

  _sensor_type = pmSensorType_t::undefined;

  _measures = nullptr;
  _nbMeasures = 0;

  /* [nov.21] we choose to disable PM%_ENABLE gpio because PMS sensors
   * already features both sleep() and wakeUp() commands
  _enable_gpio = PM_ENABLE;           // PM_ENABLE gpio
  */
  _enable_gpio = INVALID_GPIO;
}


/*
 * Destructor
 * Mostly intended to free the dynamically allocated structures
 * Note: very scrase use of dynammic allocation in our code
 */
pm_serial::~pm_serial( void ) {
  delete [] _measures;
  _measures = nullptr;
}


/*
 * Power modes: ON / OFF
 */
void pm_serial::powerOFF( void )
{
  if( !_initialized or !_stream ) return;

  if( _enable_gpio != INVALID_GPIO ) {
    log_debug(F("\n[pm_serial] going to sleep through GPIO "));log_debug(_enable_gpio);log_flush();
    pinMode( _enable_gpio, OUTPUT );    // output value has already been set to LOW
  }
  else if( _ll_sleep() ) {
    log_debug(F("\n[pm_serial] going to sleep through sleep() command"));log_flush();
  }
}

void pm_serial::powerON( void )
{
  if( !_initialized or !_stream ) return;

  if( _enable_gpio != INVALID_GPIO ) {
    log_debug(F("\n[pm_serial] wakeup through GPIO "));log_debug(_enable_gpio);log_flush();
    pinMode( _enable_gpio, INPUT );    // output value has already been set to LOW
  }
  else if( _ll_wakeUp() ) {
    log_debug(F("\n[pm_serial] wakeup through wakeUp() command"));log_flush();
  }
}


/**************************************************************************/
/*! 
    @brief  Extract JSON config parameters to initialize the HW
*/
/**************************************************************************/
boolean pm_serial::begin( JsonVariant root ) {

  // check input(s)
  if( root.isNull() or not root.is<JsonArray>() ) {
    log_error(F("\n[pms_serial] either empty JSON or wrong JSON type (array expected)!")); log_flush();
    return false;
  }

//  log_debug(F("\n[pm_serial] params found :)\n")); log_flush();
//  serializeJsonPretty( root, Serial );

  /* parse all parameters of our sensor:
  [
    {
      "param": "link",  // optional since serial2 is the only one available serial link for sensors
      "value": 2
    },
    {
      "param": "link_speed",
      "value": 9600
    },
    {
      "param": "type",
      "value": 16
    },
    {
      "param": "enable_gpio", // hardware enable gpio pin
      "value": 5
    }
  ]
  */
  for( JsonVariant item : root.as<JsonArray>() ) {

    if( item.isNull() or not item.is<JsonObject>() ) {
      log_warning(F("\n[pms_serial] format error while parsing parameters !")); log_flush();
      continue;
    }

    /* LINK
     * Note: added strlen comparison to distinguish between 'link' and 'link_speed'
     */
    {
      const char *_param = PSTR("link");
      if( strlen_P(_param)==strlen_P(item[F("param")]) and
          strncmp_P(item[F("param")], _param, strlen_P(_param))==0 ) {
        _link = (uint8_t)item[F("value")].as<int>();    // to force -1 to get converted to (uint8_t)255
      }
    }

    // LINK_SPEED
    {
      const char *_param = PSTR("link_speed");
      if( strncmp_P(item[F("param")], _param, strlen_P(_param))==0 ) {
        _link_speed = (unsigned int)item[F("value")].as<int>();    // to force -1 to get converted to (uint8_t)255
      }
    }

    // TYPE
    {
      const char *_param = PSTR("type");
      if( strncmp_P(item[F("param")], _param, strlen_P(_param))==0 ) {
        _sensor_type = (pmSensorType_t)item[F("value")].as<int>();    // to force -1 to get converted to (uint8_t)255
      }
    }

    // ENABLE_GPIO
    {
      const char *_param = PSTR("enable_gpio");
      if( strncmp_P(item[F("param")], _param, strlen_P(_param))==0 ) {
        _enable_gpio = (uint8_t)item[F("value")].as<int>();    // to force -1 to get converted to (uint8_t)255
      }
    }

  }


  /*
   * sensor HW initialisation
   */
  return _init();
}


/**************************************************************************/
/*! 
    @brief  sensor internal processing
    [nov.21] overriding generic_sensor::process()
*/
/**************************************************************************/
void pm_serial::process( uint16_t coolDown, uint8_t decimals ) {

  if( !_initialized ) return;
  
  unsigned long _curTime = millis();
  // process according to our FSM
  switch( _FSMstatus ) {

    // IDLE
    case pmSensorState_t::idle:
      // still in cooldown phase ?
      if( )
to be continued

      log_debug(F("\n\t[pm_serial] about to start a new acquisition cycle ...")); log_flush();
      _FSMtimerDelay = 0;

      // activate heating ...
      _FSMstatus = lccSensorState_t::heating;
      if( heaterStart() ) {
        log_debug(F("\n\t[lcc_sensor]["));log_debug(_subID);log_debug(F("] start heating ...")); log_flush();
      }
      // ... and continue with next step ...
      //yield();
      //break;

    // HEATING
    case lccSensorState_t::heating:
      // still in heating process ?
      if( heaterBusy() ) break;
      log_debug(F("\n\t[lcc_sensor]["));log_debug(_subID);log_debug(F("] heating is over (or not available) ...")); log_flush();

      // ok continue with next step: auto gain
      _FSMstatus = lccSensorState_t::auto_gain;
      if( autoGainStart() ) {
        log_debug(F("\n\t[lcc_sensor]["));log_debug(_subID);log_debug(F("] sensor auto-gain activation ...")); log_flush();
      }
      //yield();
      //break;

    // AUTO-GAIN
    case lccSensorState_t::auto_gain:
      // still in the autoGain process ?
      if( autoGainBusy() ) break;
      log_debug(F("\n\t[lcc_sensor]["));log_debug(_subID);log_debug(F("] auto-gain ends ...")); log_flush();

      // ok continue with next step: measure
      _FSMstatus = lccSensorState_t::measuring;
      if( measureStart() ) {
        log_debug(F("\n\t[lcc_sensor]["));log_debug(_subID);log_debug(F("] start measuring ...")); log_flush();
      }
      //yield();
      //break;

    // MEASURING
    case lccSensorState_t::measuring:
      // still in the measuring process ?
      if( measureBusy() ) break;
      log_debug(F("\n\t[lcc_sensor]["));log_debug(_subID);log_debug(F("] end of measures :)")); log_flush();

      // ok continue with next step: wait4read
      _FSMstatus = lccSensorState_t::wait4read;
      if( _nb_measures ) {
        log_debug(F("\n\t[lcc_sensor]["));log_debug(_subID);log_debug(F("] IDLE --> now waiting for data to get read ...")); log_flush();
      }
      //yield();
      //break;

    // WAIT4READ
    case lccSensorState_t::wait4read:
      // waiting for data to get read before acquiring new ones
      if( _nb_measures ) break;

      // let's restart on next loop()
      _FSMstatus = lccSensorState_t::idle;
      break;

    // default
    default:
      log_error(F("\n\t[lcc_sensor]["));log_debug(_subID);log_debug(F("] unknown FSM state ?!?! ... resetting !")); log_flush();
      _init();
  }

note: wakeup state --> nb_measures to 0

}


/**************************************************************************/
/*! 
    @brief  return sensor value(s) read from serial link
*/
/**************************************************************************/
boolean pm_serial::acquire( float *pval )
{
#if 0

WARNING: il older airquality module, acquire was intended to retrieve data
for sending over MQTT --> now replaced with getValue method

  // data available ?
  if( _nb_measures < _MAX_MEASURES ) return false;
  if( _cur_gain == LCC_SENSOR_GAIN_NONE ) return false; // because it is needed to compute Rgain

  // we'll now parse our raw measures array to produce an average
  uint32_t mv_sum = 0;
  for( uint8_t i=0; i<_nb_measures; i++ ) {
    mv_sum += _measures[i];
  }

  // we then convert the mv average value to a ppm one
  *pval = calculatePPM( (float)mv_sum / (float)_nb_measures );

  // reset measures counter (to avoid sending the same values)
  _nb_measures = 0;
#endif /* 0 */

  *pval = 42.0;

  return true;
}


/**************************************************************************/
/*! 
    @brief  return current measure subID
*/
/**************************************************************************/
String pm_serial::subID( uint8_t idx ) {
  if( idx==(uint8_t)(-1) ) return {};
  if( idx>=_nbMeasures ) return {};
  if( !_measures[idx].subID ) return {};
  
  // ok, return current subID
  return _measures[idx].subID;
}


/* ------------------------------------------------------------------------------
 * Private'n Protected methods 
 */

#if 0
/**************************************************************************/
/*! 
    @brief  start heater for a specified duration up to 65535ms
            for short pulse duration (< MAIN_DELAY_LOOP ---i.e 250ms), we
            wait for the specified delay, hence blocking behaviour,
            otherwise this is a non blocking API.
*/
/**************************************************************************/
boolean lcc_sensor::heaterStart( uint16_t pulse_ms ) {

  if( _heater_gpio==INVALID_GPIO or pulse_ms==0 ) return false;

  // ok, we start heating the sensor
  digitalWrite( _heater_gpio, HIGH );

  // short pulse ?
  if( pulse_ms < MAIN_LOOP_DELAY ) {
    delay( pulse_ms );
    digitalWrite( _heater_gpio, LOW );
    _FSMtimerDelay = 0;
    return false; // no delay activated
  }

  // set FSM timer ...
  _FSMtimerStart = millis();
  _FSMtimerDelay = pulse_ms;

  return true;
}


/**************************************************************************/
/*! 
    @brief  non blocking API requesting about heating status
            return false: heating is over
            return true: heating is currently active
 */
/**************************************************************************/
boolean lcc_sensor::heaterBusy( void ) {

  if( _heater_gpio==INVALID_GPIO or _FSMtimerDelay==0 ) return false;

  /* reached the delay ?
   * look at https://arduino.stackexchange.com/questions/33572/arduino-countdown-without-using-delay/33577#33577
   * for an explanation about millis() that wrap around!
   */
  if( (millis() - _FSMtimerStart) >= (unsigned long)_FSMtimerDelay ) {
    // end of heating period
    digitalWrite( _heater_gpio, LOW );
    _FSMtimerDelay = 0;
    return false;
  }

  // heating still on way
  return true;
}


/**************************************************************************/
/*! 
    @brief  automatic selection of highest available gain for our AOP
 */
/**************************************************************************/
boolean lcc_sensor::autoGainStart( uint16_t integration_ms ) {

  // activate highest possible (and available) gain
  boolean _gainSet = false;

  // WARNING: int8_t for g ... not Uint8_t ;)
  for( int8_t g=LCC_SENSOR_GAIN_MAX; g>=LCC_SENSOR_GAIN_MIN; g-- ) {

    if( _inputs[g]==INVALID_GPIO ) continue;
    // ok we found a valid GPIO
    if( _gainSet ) {
      // since gain is already selected, set others as input
      pinMode( _inputs[g], INPUT );
      continue;
    }
    // ok found highest gain gpio available
    pinMode( _inputs[g], OUTPUT );
    digitalWrite( _inputs[g], LOW );
    _gainSet = true;
    // if current gain is already the selected one ... don't have to wait !
    if( g == _cur_gain ) {
      integration_ms = 0; // warning, overwriting input parameter !
      continue;
    }
    _cur_gain = g;
  }

  if( !_gainSet ) {
    log_error(F("\n\t[lcc_sensor]["));log_debug(_subID);log_debug(F("] no gpio available to set proper gain ... continuing")); log_flush();
    _cur_gain = LCC_SENSOR_GAIN_NONE;
    return false;
  }

  // integration delay
  _FSMtimerDelay = 0;
  if( integration_ms >= MAIN_LOOP_DELAY ) {
    _FSMtimerDelay = integration_ms;
    _FSMtimerStart = millis();
  }
  else if( integration_ms!=0 ) {
    delay( integration_ms );
  }

  log_debug(F("\n\t[lcc_sensor]["));log_debug(_subID);log_debug(F("][autoGainStart] _cur_gain = ")); log_debug(_cur_gain); log_flush();

  return true;
}


/**************************************************************************/
/*! 
    @brief  automatic selection of highest available gain for our AOP
 */
/**************************************************************************/
boolean lcc_sensor::autoGainBusy( uint16_t integration_ms ) {

  // no gain set means no gpio available
  if( _cur_gain == LCC_SENSOR_GAIN_NONE ) return false;

  boolean _found = false;

  do {

    // do we need to wait (i.e are we busy) ?
    if( _FSMtimerDelay!=0 and 
        (millis() - _FSMtimerStart) < (unsigned long)_FSMtimerDelay ) {
      return true;
    }

    // read adc
    uint32_t _adc_val;
    if( !readSensor_mv( &_adc_val ) ) {
      log_error(F("\n\t[lcc_sensor]["));log_debug(_subID);log_debug(F("] unable to read our ADC ?!?! ... aborting")); log_flush();
      break;
    }
    float adc_mv = (float)_adc_val;

    // log_debug(F("\n[lcc_sensor] _adc_val(mv) = ")); log_debug(_adc_val); log_flush();

    // check value < LCC_SENSOR_VTH
    if( adc_mv <= LCC_SENSOR_MVTH ) {
      _found = true;
      break;
    }

    // decrease gain
    if( !_decreaseGain() ) {
      // unable to decrease gain ... thus this is our best value
      _found = true;
      break;
    }

    // wait delay (short pulse)
    if( integration_ms < MAIN_LOOP_DELAY ) {
      delay( integration_ms );
      _FSMtimerDelay = 0;
      continue; // restart ADC acquire
    }
    _FSMtimerStart = millis();
    _FSMtimerDelay = integration_ms;
    return true; // we're busy so check on next loop() iteration

  } while( _found==false );


  if( !_found ) {
    log_warning(F("\n\t[lcc_sensor]["));log_debug(_subID);log_debug(F("] ADC failure ?!?! ... continuing")); log_flush();
  }
  else {
    log_debug(F("\n\t[lcc_sensor]["));log_debug(_subID);log_debug(F("] selected _cur_gain = ")); log_debug(_cur_gain); log_flush();
  }

  // whatever happened, we're not busy anymore
  return false;
}


/**************************************************************************/
/*! 
    @brief  internal ADC read; sends back voltage_mv
*/
/**************************************************************************/
boolean lcc_sensor::readSensor_mv( uint32_t *pval ) {

  if( pval==nullptr ) return false;

#if defined(ESP32)
  #if !defined(DISABLE_ADC_CAL)
  // advanced ADC reading
  esp_err_t res;
  uint8_t _retry = 3;
  do {
    res = esp_adc_cal_get_voltage( (adc_channel_t)digitalPinToAnalogChannel(_inputs[LCC_SENSOR_ANALOG]),
                                  adc_chars, pval );
    if( res!=ESP_OK ) delay(20);
  } while( res!=ESP_OK and _retry-- );
  return (res==ESP_OK ? true : false);

  #else /* ADC_CAL is disabled */
  // regular ADC reading
  *pval = ((uint32_t)(analogRead(_inputs[LCC_SENSOR_ANALOG]))*_adc_voltageRef) / ((uint32_t)(pow(2,_adc_resolution))-1);
  return true;

  #endif /* DISABLE_ADC_CAL */

#elif defined(ESP8266)
  // 10bits resolution with 1.1 ref. voltage
  *pval = ((uint32_t)(analogRead(_inputs[LCC_SENSOR_ANALOG]))*_adc_voltageRef) / ((uint32_t)(pow(2,_adc_resolution))-1);
  return true;
#endif

  // error as default
  return false;
}


/**************************************************************************/
/*! 
    @brief  start measurment process now every previous steps have been
            undertaken
*/
/**************************************************************************/
boolean lcc_sensor::measureStart( void ) {

  // reset count of measures
  _nb_measures = 0;

  return true;
}


/**************************************************************************/
/*! 
    @brief  check about undergoing measuremt process
*/
/**************************************************************************/
boolean lcc_sensor::measureBusy( void ) {

  boolean res;
  while( _nb_measures < _MAX_MEASURES ) {
    
    // do we need to wait (i.e are we busy) ?
    if( _FSMtimerDelay!=0 and 
        (millis() - _FSMtimerStart) < (unsigned long)_FSMtimerDelay ) return true;

    // acquire data
    res = readSensor_mv( &_measures[_nb_measures] );
    if( !res ) {
      log_debug(F("\n\t[lcc_sensor]["));log_debug(_subID);log_debug(F("] read failure ?!?! ... next iteration :|")); log_flush();
      return true;
    }
    _nb_measures++;

    // last data written ?
    if( _nb_measures == _MAX_MEASURES ) break; // not busy anymore

    // delay between two measures
    if( _MEASURES_INTERLEAVE_MS < MAIN_LOOP_DELAY ) {
      delay( _MEASURES_INTERLEAVE_MS );
      _FSMtimerDelay = 0;
      continue;z
    }

    // long delay between measures
    _FSMtimerStart = millis();
    _FSMtimerDelay = _MEASURES_INTERLEAVE_MS;
    return true; // we're busy so check on next loop() iteration
  }

  /* DEBUG DEBUG DEBUG */
  log_debug(F("\n\t[lcc_sensor]["));log_debug(_subID);log_debug(F("] raw DATA read:")); log_flush();
  for( uint32_t _val : _measures ) {
    log_debug(F("\n\t")); log_debug(_val);log_debug(F("mv"));
  }

  return false; // not busy anymore
}
#endif /* 0 */



/*
 * DATA integration related methods:
 *  get official value that has gone through the whole integration process
 */
float pm_serial::getValue( uint8_t *idx ) {
#if 0
  if( _idx ) {
    if( *_idx==(uint8_t)(-1) ) {
      return nb_vars2retrieve
    }
  }
#endif /* 0 */
//TO BE CONTINUED

  // TODO !!
  log_error(F("\n[pm_serial] NOT YET IMPLEMENTED"));log_flush();
  return -1.0;
}


/*
 * DATA integration related methods
 *  cancel trigger and set value sent
 */
void pm_serial::setDataSent( void ) {

  _trigger = false;
  unsigned long curTime = millis();

  // reinitialize measures array
  for( uint8_t i=0; i<_nbMeasures; i++ ) {
    if( !_measures[i]._trigger ) continue;

    _measures[i]._trigger       = false;

    _measures[i].valueSent      = value;
    _measures[i]._lastMsSent    = curTime;
  }
}


/**************************************************************************/
/*! 
    @brief  Low-level initialization
*/
/**************************************************************************/
boolean pm_serial::_init( void ) {

  // assertion checks
  if( _measures ) {
    log_error(F("\n[pm_serial] already allocated measurement array ?!?!"));log_flush();
    return false;   // measurment array OUGHT not to get already initialized
  }

  // check sensor type and initialize measurements
  switch( _sensor_type ) {

    // PMSx003
    case pmSensorType_t::PMSx003 :
      log_debug(F("\n[pm_serial] start PMSx003 PM sensor setup ..."));log_flush();
      _nbMeasures = (uint8_t)pmsx003DataIdx_t::last;
      _measures = new serialMeasure_t[_nbMeasures];
      for( uint8_t i=0; i<_nbMeasures; i++ ) _measures[i].subID = nullptr;
      _measures[(uint8_t)pmsx003DataIdx_t::PM2_5].subID = subID_pm2_5;
      _measures[(uint8_t)pmsx003DataIdx_t::PM10].subID = subID_pm10;
      break;

    // SDS011
    case pmSensorType_t::SDS011 :
      log_debug(F("\n[pm_serial] start SDS011 PM sensor setup ..."));log_flush();
      _nbMeasures = (uint8_t)sds011DataIdx_t::last;
      _measures = new serialMeasure_t[_nbMeasures];
      for( uint8_t i=0; i<_nbMeasures; i++ ) _measures[i].subID = nullptr;
      _measures[(uint8_t)sds011DataIdx_t::PM2_5].subID = subID_pm2_5;
      _measures[(uint8_t)sds011DataIdx_t::PM10].subID = subID_pm10;
      break;

    // add additional kind of sensor here

    default:
      log_error(F("\n[pm_serial] unknown PM sensor type "));log_error((uint8_t)_sensor_type);log_flush();
      return false;
  }

  _trigger = false;

  /* Measurement array initialization
   */
  // initialize measures array
  for( uint8_t i=0; i<_nbMeasures; i++ ) {
    _measures[i]._trigger       = false;

    _measures[i]._currentCpt    = (uint8_t)(-1);
    _measures[i]._lastMsRead    = ULONG_MAX/2;
    _measures[i]._lastMsWrite   = ULONG_MAX/2;
    _measures[i]._lastMsSent    = ULONG_MAX/2;

    _measures[i].value          = -1.0; // fool guard
  }

  /* Initialize serial link.
   * Note: link number is the serialX stream object
   */
  if( _link != SENSORS_SERIAL_LINK ) {
    log_error(F("\n[pm_serial] inappropriate serial link number "));log_error(_link);log_flush();
    return false;
  }
  Serial2.begin( _link_speed );
  _stream = &Serial2;  // TODO pointer to stream according to link number specified ... maybe later ;)
  if( !_stream ) return false;
  
  // switch to passive mode (if any)
  _activeMode = true;
  if( _ll_passiveMode() ) {
    log_debug(F("\n[pm_serial] switch to passive mode"));log_flush();
    _activeMode = false;
  }

  // powerOFF module
  powerOFF();

  // flush serial buffer
  while( _stream->available() ) _stream->read();

  // configure enable_gpio if any
  if( _enable_gpio != INVALID_GPIO ) {
    pinMode( _enable_gpio, INPUT );
    digitalWrite( _enable_gpio, LOW );  // this way, INPUT ==> normal ops, OUTPUT ==> sensor disabled
  }

  // set FSM initial state
  _FSMstatus = PM_FSMSTATE_DEFL;
  _FSMtimerDelay = 0;

  // the end ...
  _initialized = true;
  log_debug(F("\n[pm_serial] successfully initialized PM sensor"));log_flush();

  return _initialized;
}


// ============================================================================
// === LOW-LEVEL serial methods ===============================================

/**************************************************************************/
/*! 
    @brief  Low-level sleep PM sensor
    @note   only relevant if PM sensor has support for such feature
*/
/**************************************************************************/
boolean pm_serial::_ll_sleep( void ) {

  if( ! _stream ) return false;

  boolean res = false;

  if( _sensor_type ==  pmSensorType_t::PMSx003 ) {
    uint8_t command[] = { 0x42, 0x4D, 0xE4, 0x00, 0x00, 0x01, 0x73 };
    _stream->write(command, sizeof(command)); delay(50);
    res = true;
  }
  return res;
}

/**************************************************************************/
/*! 
    @brief  Low-level wakeUP PM sensor
    @note   only relevant if PM sensor has support for such feature
*/
/**************************************************************************/
boolean pm_serial::_ll_wakeUp( void ) {

  if( ! _stream ) return false;

  boolean res = false;

  if( _sensor_type ==  pmSensorType_t::PMSx003 ) {
      uint8_t command[] = { 0x42, 0x4D, 0xE4, 0x00, 0x01, 0x01, 0x74 };
      _stream->write(command, sizeof(command)); delay(50);
      res = true;
  }
  return res;
}

/**************************************************************************/
/*! 
    @brief  Low-level passive mode PM sensor
    @note   only relevant if PM sensor has support for such feature
*/
/**************************************************************************/
boolean pm_serial::_ll_passiveMode( void ) {

  if( ! _stream ) return false;

  boolean res = false;

  if( _sensor_type ==  pmSensorType_t::PMSx003 ) {
      uint8_t command[] = { 0x42, 0x4D, 0xE1, 0x00, 0x00, 0x01, 0x70 };
      _stream->write(command, sizeof(command)); delay(50);
      res = true;
  }
  return res;
}

/**************************************************************************/
/*! 
    @brief  Low-level active mode PM sensor
    @note   only relevant if PM sensor has support for such feature
*/
/**************************************************************************/
boolean pm_serial::_ll_activeMode( void ) {

  if( ! _stream ) return false;

  boolean res = false;

  if( _sensor_type ==  pmSensorType_t::PMSx003 ) {
      uint8_t command[] = { 0x42, 0x4D, 0xE1, 0x00, 0x01, 0x01, 0x71 };
      _stream->write(command, sizeof(command)); delay(50);
      res = true;
  }
  return res;
}
