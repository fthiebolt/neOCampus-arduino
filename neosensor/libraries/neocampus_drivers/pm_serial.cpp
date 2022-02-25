/**************************************************************************/
/*! 
  @file     pms_serial.cpp
  @author   F.Thiebolt (neOCampus / Univ.Tlse3)
  @license
	
	PMS 5003 & derivatives Airquality sensors driver for neOCampus

	@section  HISTORY

    feb.22  F.thiebolt  IKEA sensor: switched to a new read command borrowed 
                        from on board IKEA PM sensor micro-controller
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
#define _MAX_MEASURES               DEFL_THRESHOLD_CPT    // max. number of measures during a single campaign
#define _MAX_FAILURES               3   // max. number of consecutives failures


/* declare kind of units (value_units) */
const char *pm_serial::units_pm   = "µg/m3";
const char *pm_serial::units_co2  = "ppm";
/* declare subIDs */
const char *pm_serial::subID_pm2_5  = "pm2_5";
const char *pm_serial::subID_pm10   = "pm10";
const char *pm_serial::subID_co2    = "co2";


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
  if( ! _powerSave ) return;

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
      if( _curTime - _lastMsRead < ((unsigned long)coolDown)*1000 ) break;
      log_debug(F("\n\t[pm_serial] going out of cooldown ...")); log_flush();
      _FSMtimerDelay = 0;

      // wake-up sensor ...
      _FSMstatus = pmSensorState_t::wakeup;
      if( FSMwakeUpStart() ) {
        log_debug(F("\n\t[pm_serial] start wake-up cycle ...")); log_flush();
      }
      // ... and continue with next step ...
      //yield();
      //break;

    // WAKE-UP
    case pmSensorState_t::wakeup:
      // still in wakeup cycle ?
      if( FSMwakeUpBusy() ) break;
      log_debug(F("\n\t[pm_serial] wake up cycle is over ...")); log_flush();

      // ok continue with next step: auto gain
      _FSMstatus = pmSensorState_t::measuring;
      if( FSMmeasureStart() ) {
        log_debug(F("\n\t[pm_serial] start measuring ...")); log_flush();
      }
      //yield();
      //break;

    // MEASURING
    case pmSensorState_t::measuring:
      // still in the measuring process ?
      if( FSMmeasureBusy() ) break;
      log_debug(F("\n\t[pm_serial] end of measures :)")); log_flush();

      // shutdown sensor
      if( _powerSave ) powerOFF();

      // ok continue with next step: wait4read
      _FSMstatus = pmSensorState_t::wait4read;
      if( _trigger ) {
        log_debug(F("\n\t[pm_serial] now waiting for data to get read ...")); log_flush();
      }
      //yield();
      //break;

    // WAIT4READ
    case pmSensorState_t::wait4read:
      // waiting for data to get read before acquiring new ones
      if( _trigger ) break;

      // let's restart on next loop()
      _FSMstatus = pmSensorState_t::idle;
      log_debug(F("\n\t[pm_serial] end of cycle, switching to IDLE state ...")); log_flush();
      break;

    // default
    default:
      log_error(F("\n\t[pm_serial] unknown FSM state ?!?! ... resetting !")); log_flush();
      delay(1000);
      _init();
  }
}


/**************************************************************************/
/*! 
    @brief  return current measure units
*/
/**************************************************************************/
const char* pm_serial::sensorUnits( uint8_t idx ) {
  if( idx==(uint8_t)(-1) ) return nullptr;

  // looking for first available units
  for( uint8_t i=idx; i<_nbMeasures; i++ ) {
    if( !_measures[i]._trigger ) continue;
    if( !_measures[i].units ) return nullptr;
    // ok, return current units
    return _measures[i].units;
  }

  // not found ...
  return nullptr;
};


/**************************************************************************/
/*! 
    @brief  return current measure subID
*/
/**************************************************************************/
String pm_serial::subID( uint8_t idx ) {
  if( idx==(uint8_t)(-1) ) return {};

  // looking for first available subTrigger
  for( uint8_t i=idx; i<_nbMeasures; i++ ) {
    if( !_measures[i]._trigger ) continue;
    if( !_measures[i].subID ) return {};
    // ok, return current subID
    return _measures[i].subID;
  }

  // not found ...
  return {};
}



/* ------------------------------------------------------------------------------
 * Private'n Protected methods 
 */


/**************************************************************************/
/*! 
    @brief  start wake up cycle up to 65535ms
            for short pulse duration (< MAIN_DELAY_LOOP ---i.e 250ms), we
            wait for the specified delay, hence blocking behaviour,
            otherwise this is a non blocking API.
    @note   TODO: on very first measurement cycle, we need to wait the specified delay
*/
/**************************************************************************/
boolean pm_serial::FSMwakeUpStart( uint16_t seconds ) {

  if( ! _powerSave ) {
    _FSMtimerDelay = 0;
    return false; // no delay
  }

  // start sensors
  powerON();

  unsigned long delay_ms = seconds * 1000;
  // short pulse ?
  if( delay_ms < MAIN_LOOP_DELAY ) {
    delay( delay_ms );
    _FSMtimerDelay = 0;
    return false; // no delay activated
  }

  // set FSM timer ...
  _FSMtimerStart = millis();
  _FSMtimerDelay = delay_ms;

  return true;
}


/**************************************************************************/
/*! 
    @brief  non blocking API requesting about wake up status
            return false: wake up is over
            return true: still waking up
 */
/**************************************************************************/
boolean pm_serial::FSMwakeUpBusy( void ) {

  if( _FSMtimerDelay==0 ) return false;

  /* reached the delay ?
   * look at https://arduino.stackexchange.com/questions/33572/arduino-countdown-without-using-delay/33577#33577
   * for an explanation about millis() that wrap around!
   */
  if( (millis() - _FSMtimerStart) >= (unsigned long)_FSMtimerDelay ) {
    // sensor wakeUp cycle is over
    _FSMtimerDelay = 0;
    return false;
  }

  // wakeUp still on way
  return true;
}


/**************************************************************************/
/*! 
    @brief  start measurment process now every previous steps have been
            undertaken
*/
/**************************************************************************/
boolean pm_serial::FSMmeasureStart( void ) {

  // reset count of measures
  for( uint8_t i=0; i<_nbMeasures; i++ ) {
    _measures[i]._currentSum = (float)0;
  }
  _readCpt = 0;
  _retryCpt = _MAX_FAILURES;

  // active mode
  if( !_activeMode ) {
    if( _ll_activeMode() ) {
      _activeMode = true;
      log_debug(F("\n[pm_serial] active mode enabled ..."));log_flush();
    }
    else {
      log_warning(F("\n[pm_serial] unable to switch to activeMode !?!? ... continuing"));log_flush();
      delay(500);
    }
  }
  // start reading immediately
  _FSMtimerDelay = 0;

  return true;
}


/**************************************************************************/
/*! 
    @brief  check about undergoing measuremt process
*/
/**************************************************************************/
boolean pm_serial::FSMmeasureBusy( void ) {

  boolean res = true;

  while( (_readCpt < _MAX_MEASURES) and _retryCpt ) {

    // do we need to wait (i.e are we busy) ?
    if( _FSMtimerDelay!=0 and 
        (millis() - _FSMtimerStart) < (unsigned long)_FSMtimerDelay ) return true;

    // acquire data trough blocking API
    switch( _sensor_type ) {

      // PMSX003
      case pmSensorType_t::PMSx003 :
        _ll_requestRead();
        res = serialRead_pmsx003(); // blocking read with timeout
        break;

      // SDS011
      case pmSensorType_t::SDS011 :
        _ll_requestRead();
        res = serialRead_sds011();  // blocking read with timeout
        break;

      // IKEA
      case pmSensorType_t::IKEA :
        _ll_requestRead();
        res = serialRead_ikea();    // blocking read with timeout
        break;

      // [CO2] MH-Z1x
      case pmSensorType_t::MHZ1x :
        _ll_requestRead();
        res = serialRead_mhz1x();   // blocking read with timeout
        break;

      // default
      default:
        log_error(F("\n\t[pm_serial] unknown sensor type Ox"));log_error((uint8_t)_sensor_type,HEX);log_flush();
        delay(1000);
        return false;
    }

    if( !res ) {
      log_debug(F("\n\t[pm_serial] read failure ?!?! ... next iteration :|")); log_flush();
      _retryCpt--;
    }
    else {
      _readCpt++;
    }

    // last data read ?
    if( _readCpt == _MAX_MEASURES ) break; // not busy anymore

    // delay between two measures
    if( _MEASURES_INTERLEAVE_MS < MAIN_LOOP_DELAY ) {
      delay( _MEASURES_INTERLEAVE_MS );
      _FSMtimerDelay = 0;
      continue;
    }

    // long delay between measures
    _FSMtimerStart = millis();
    _FSMtimerDelay = _MEASURES_INTERLEAVE_MS;
    return true; // we're busy so check on next loop() iteration
  }

  // check for many consecutives failures
  if( !_retryCpt ) {
    log_error(F("\n[pm_serial] too many consecutives reading errors ... next cycle :|")); log_flush();
    delay(500);
    return false; // not busy anymore :|
  }

  // END of measurement campaign ...
  unsigned long curTime = millis();
  _lastMsRead = curTime;
  log_debug(F("\n[pm_serial] compute avg values:"));log_flush();

  for( uint8_t i=0; i<_nbMeasures; i++ ) {
    _measures[i].value = round(_measures[i]._currentSum / (float)_readCpt);
    log_debug(F("\n[pm_serial] value["));log_debug(i);log_debug(F("] = "));log_debug(_measures[i].value);log_flush();
    // needs to get sent ?
    if( (abs(_measures[i].value - _measures[i].valueSent) >= 1) || 
      (curTime - _measures[i]._lastMsSent >= ((unsigned long)_MAX_COOLDOWN_SENSOR)*1000) ) {
      _measures[i]._trigger = true; // activate per subID trigger ...
      _trigger = true;  // ... then activate sensor's trigger  
    }
  }

  return false; // not busy anymore
}


/*
 * DATA integration related methods
 *  send back global sensor's trigger
 */
bool pm_serial::getTrigger( void ) {
  return _trigger;
}


/*
 * DATA integration related methods:
 *  get official value that has gone through the whole integration process
 * Note:
 * - if idx is nullptr ==> simple getValue() call --> send back first available value
 * - else search for first available value starting from *idx
 *    if no data available ==> *idx = -1
 */
float pm_serial::getValue( uint8_t *idx ) {

  // check validity of idx (if any)
  if( idx and *idx==(uint8_t)(-1) ) {
    log_error(F("\n[pm_serial] getValue(-1) ?!?!")); log_flush();
    return -42.0;
  }

  uint8_t startIdx = ( (idx==nullptr) ? 0 : *idx);

  // looking for first available subTrigger
  for( uint8_t i=startIdx; i<_nbMeasures; i++ ) {
    if( _measures[i]._trigger ) {
      if( idx ) *idx=i;
      return _measures[i].value;
    }
  }

  // no more data available
  if( idx ) *idx=(uint8_t)(-1);
  return -42.0;
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

    _measures[i].valueSent      = _measures[i].value;
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

  /* as a default power save is ENABLED
   * ==> sensor is powered down and woke up when needed
   */
  _powerSave = true;

  // check sensor type and initialize measurements
  switch( _sensor_type ) {

    // PMSx003
    case pmSensorType_t::PMSx003 :
      log_debug(F("\n[pm_serial] start PMSx003 PM sensor setup ..."));log_flush();
      _activeMode = true;
      _nbMeasures = (uint8_t)pmsx003DataIdx_t::last;
      _measures = new serialMeasure_t[_nbMeasures];
      for( uint8_t i=0; i<_nbMeasures; i++ ) {
        _measures[i].subID = nullptr;
        _measures[i].units = nullptr;
      }
      _measures[(uint8_t)pmsx003DataIdx_t::PM2_5].subID = subID_pm2_5;
      _measures[(uint8_t)pmsx003DataIdx_t::PM2_5].units = units_pm;
      _measures[(uint8_t)pmsx003DataIdx_t::PM10].subID = subID_pm10;
      _measures[(uint8_t)pmsx003DataIdx_t::PM10].units = units_pm;
      break;

    // SDS011
    case pmSensorType_t::SDS011 :
      log_debug(F("\n[pm_serial] start SDS011 PM sensor setup ..."));log_flush();
      _activeMode = true;   // [nov.21] mode really depends on previous config saved within sensor. Factory default is active
      _powerSave = false;   // [nov.21] noisy sensor, we prefer not having powerON / powerOFF cycles :|
      _nbMeasures = (uint8_t)sds011DataIdx_t::last;
      _measures = new serialMeasure_t[_nbMeasures];
      for( uint8_t i=0; i<_nbMeasures; i++ ) {
        _measures[i].subID = nullptr;
        _measures[i].units = nullptr;
      }
      _measures[(uint8_t)sds011DataIdx_t::PM2_5].subID = subID_pm2_5;
      _measures[(uint8_t)sds011DataIdx_t::PM2_5].units = units_pm;
      _measures[(uint8_t)sds011DataIdx_t::PM10].subID = subID_pm10;
      _measures[(uint8_t)sds011DataIdx_t::PM10].units = units_pm;
      break;

    // IKEA VINDRIKTNING
    case pmSensorType_t::IKEA :
      log_debug(F("\n[pm_serial] start IKEA PM sensor setup ..."));log_flush();
      _activeMode = false;  // [nov.21] yes, passive as default
      _nbMeasures = (uint8_t)ikeaDataIdx_t::last;
      _measures = new serialMeasure_t[_nbMeasures];
      for( uint8_t i=0; i<_nbMeasures; i++ ) {
        _measures[i].subID = nullptr;
        _measures[i].units = nullptr;
      }
      _measures[(uint8_t)ikeaDataIdx_t::PM2_5].subID = subID_pm2_5;
      _measures[(uint8_t)ikeaDataIdx_t::PM2_5].units = units_pm;
      _measures[(uint8_t)ikeaDataIdx_t::PM10].subID = subID_pm10;
      _measures[(uint8_t)ikeaDataIdx_t::PM10].units = units_pm;
      break;

    // [CO2] MH-Z1x
    case pmSensorType_t::MHZ1x :
      log_debug(F("\n[pm_serial] start MH-Z1x CO2 sensor setup ..."));log_flush();
      _activeMode = false;  // [nov.21] yes, passive as default
      _nbMeasures = (uint8_t)mhz1xDataIdx_t::last;
      _measures = new serialMeasure_t[_nbMeasures];
      for( uint8_t i=0; i<_nbMeasures; i++ ) {
        _measures[i].subID = nullptr;
        _measures[i].units = nullptr;
      }
      _measures[(uint8_t)mhz1xDataIdx_t::CO2].subID = subID_co2;
      _measures[(uint8_t)mhz1xDataIdx_t::CO2].units = units_co2;
      break;

    // add additional kind of sensor here

    default:
      log_error(F("\n[pm_serial] unknown PM sensor type "));log_error((uint8_t)_sensor_type);log_flush();
      return false;
  }

  _trigger = false;

  /* 
   * Measurement initialization
   */
  // initialize measures array
  for( uint8_t i=0; i<_nbMeasures; i++ ) {
    _measures[i]._trigger       = false;
    _measures[i]._lastMsSent    = ULONG_MAX/2;
    _measures[i].value          = -1.0; // fool guard
  }
  _currentCpt    = (uint8_t)(-1);
  _lastMsRead    = ULONG_MAX/2;

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
  if( _ll_passiveMode() ) {
    log_debug(F("\n[pm_serial] switch to passive mode"));log_flush();
    _activeMode = false;
  }

  // configure enable_gpio if any
  if( _enable_gpio != INVALID_GPIO ) {
    pinMode( _enable_gpio, INPUT );
    digitalWrite( _enable_gpio, LOW );  // this way, INPUT ==> normal ops, OUTPUT ==> sensor disabled
  }

  // powerSave mode ==> stop sensor, otherwise start it
  if( _powerSave ) powerOFF();
  else {
    log_debug(F("\n[pm_serial] DISABLED power save mode ... "));log_flush();
    powerON();
  }

  // flush serial buffer
  while( _stream->available() ) _stream->read();

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
  else if( _sensor_type ==  pmSensorType_t::SDS011 ) {
    uint8_t command[] = { 0xAA, 0xB4, 0x06, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0xFF,	0xFF, 0x05, 0xAB };
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
  else if( _sensor_type ==  pmSensorType_t::SDS011 ) {
    uint8_t command[] = { 0xAA, 0xB4, 0x06, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x06, 0xAB };
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
  else if( _sensor_type ==  pmSensorType_t::SDS011 ) {
    // [nov.21] SDS011 sleep command ... unless powerSave feature is disabled
    if( _powerSave ) {
      uint8_t command[] = { 0xAA, 0xB4, 0x02, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x02, 0xAB };
      _stream->write(command, sizeof(command)); delay(50);
    }
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
  else if( _sensor_type ==  pmSensorType_t::SDS011 ) {
    // SDS011 wake-up command ... whatever power save mode
    uint8_t command[] = { 0xAA, 0xB4, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x01, 0xAB };
    _stream->write(command, sizeof(command)); delay(50);
    res = true;
  }

  return res;
}


/**************************************************************************/
/*! 
    @brief  ask for data to get read
    @note   only relevant if PM sensor has support for such feature
*/
/**************************************************************************/
boolean pm_serial::_ll_requestRead( void ) {
  if( ! _stream ) return false;
  if( _activeMode ) return false;

  boolean res = false;

  if( _sensor_type ==  pmSensorType_t::PMSx003 ) {
    uint8_t command[] = { 0x42, 0x4D, 0xE2, 0x00, 0x00, 0x01, 0x71 };
    _stream->write(command, sizeof(command)); delay(50);
    res = true;
  }
  else if( _sensor_type ==  pmSensorType_t::SDS011 ) {
    uint8_t command[] = { 0xAA, 0xB4, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x02, 0xAB };
    _stream->write(command, sizeof(command)); delay(50);
    res = true;
  }
  else if( _sensor_type ==  pmSensorType_t::IKEA ) {
    // uint8_t command[] = { 0x11, 0x01, 0x02, 0xEC }; // regular command
    uint8_t command[] = { 0x11, 0x02, 0x0b, 0x01, 0xE1 }; // [feb.22] hidden command
    _stream->write(command, sizeof(command)); delay(50);
    res = true;
  }
  else if( _sensor_type ==  pmSensorType_t::MHZ1x ) {
    uint8_t command[] = { 0XFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79 };
    _stream->write(command, sizeof(command)); delay(50);
    res = true;
  }

  return res;
}



// ============================================================================
// === SENSORS SPECIFIC READ PROTOCOL methods =================================

/*!
 * PMSX003: serial data grabber and parser
 * @note blocking call till data OK or timeout
 */
boolean pm_serial::serialRead_pmsx003( uint16_t timeout ) {
  
  uint8_t _index = 0;
  uint16_t _frameLen = 0;
  uint16_t _checksum = 0;
  uint16_t _calculatedChecksum = 0;
  uint8_t _payload[12];

  unsigned long startTime=millis();

  do {
    // serial data available ?
    if( !_stream->available() ) {
      delay(5); continue;
    }
    // read serial char
    uint8_t ch = _stream->read();
    // switch to byte position in frame
    switch( _index ) {

      case 0:
        if( ch != 0x42 ) continue;
        _calculatedChecksum = ch;
        break;

      case 1:
        if( ch != 0x4D ) {
          _index = 0;
          continue;
        }
        _calculatedChecksum += ch;
        break;

      case 2:
        _calculatedChecksum += ch;
        _frameLen = ch << 8;
        break;

      case 3:
        _frameLen |= ch;
        // Unsupported sensor, different frame length, transmission error e.t.c.
        if( _frameLen != 2 * 9 + 2 && _frameLen != 2 * 13 + 2 ) {
          _index = 0;
          continue;
        }
        _calculatedChecksum += ch;
        break;

      default:
        if(_index == _frameLen + 2 ) {
          _checksum = ch << 8;
        }
        else if( _index == _frameLen + 2 + 1 ) {
          _checksum |= ch;

          if( _calculatedChecksum != _checksum ) {
            // bad checksum ...
            log_warning(F("\n[pm_serial][PMSx003] bad checksum ...")); log_flush();
            _index = 0;
            continue;
          }

          uint16_t value;
          /* Standard Particles, CF=1.
          _data->PM_SP_UG_1_0 = makeWord(_payload[0], _payload[1]);
          _data->PM_SP_UG_2_5 = makeWord(_payload[2], _payload[3]);
          _data->PM_SP_UG_10_0 = makeWord(_payload[4], _payload[5]);
          */
          // Atmospheric Environment.
          //value = makeWord(_payload[6], _payload[7]);   // PM1_0
          value = makeWord(_payload[8], _payload[9]);     // PM2_5
          log_debug(F("\n[pm_serial][PMSx003] PM2_5 = "));log_debug(value);log_flush();
          _measures[(uint8_t)pmsx003DataIdx_t::PM2_5]._currentSum += (float)value;
          log_debug(F("\t_currentSum = "));log_debug(_measures[(uint8_t)pmsx003DataIdx_t::PM2_5]._currentSum);
          log_flush();
          value = makeWord(_payload[10], _payload[11]);   // PM10
          log_debug(F("\n[pm_serial][PMSx003] PM10 = "));log_debug(value);log_flush();
          _measures[(uint8_t)pmsx003DataIdx_t::PM10]._currentSum += (float)value;

          // data acquired, finisk :)
          log_debug(F("\n[pm_serial][PMSx003] "));log_debug(millis()-startTime);
          log_debug(F("ms reading data over serial link"));log_flush();
          _index = 0; // useless since we return now ...
          return true;
        }
        else {
          // payload acquire
          _calculatedChecksum += ch;
          uint8_t payloadIndex = _index - 4;

          // Payload is common to all sensors (first 2x6 bytes).
          if( payloadIndex < sizeof(_payload) ) _payload[payloadIndex] = ch;
        }
    }
    _index++;

  } while( millis() - startTime < timeout );

  return false;
}


/*!
 * SDS011: serial data grabber and parser
 * @note blocking call till data OK or timeout
 */
boolean pm_serial::serialRead_sds011( uint16_t timeout ) {
  
  uint8_t _index = 0;
  uint16_t _checksum = 0;
  uint16_t _calculatedChecksum = 0;
  uint8_t _payload[4];

  unsigned long startTime=millis();

  do {
    // serial data available ?
    if( !_stream->available() ) {
      delay(5); continue;
    }
    // read serial char
    uint8_t ch = _stream->read();
    // switch to byte position in frame
    switch( _index ) {

      case 0:
        if( ch != 0xAA ) continue;
        break;

      case 1:
        if( ch != 0xC0 ) {
          _index = 0;
          continue;
        }
        break;

      case 8:
        _checksum = ch;
        break;

      case 9:
        if( ch != 0xAB ) {
          _index = 0;
          continue;
        }
        if( (uint8_t)_calculatedChecksum != (uint8_t)_checksum ) {
          log_warning(F("\n[pm_serial][SDS011] bad checksum ...")); log_flush();
          _index = 0;
          continue;
        }
        uint16_t value;
        // Atmospheric Environment.
        value = makeWord(_payload[1], _payload[0])/10;     // PM2_5
        log_debug(F("\n[pm_serial][SDS011] PM2_5 = "));log_debug(value);log_flush();
        _measures[(uint8_t)sds011DataIdx_t::PM2_5]._currentSum += (float)value;
        log_debug(F("\t_currentSum = "));log_debug(_measures[(uint8_t)sds011DataIdx_t::PM2_5]._currentSum);
        log_flush();
        value = makeWord(_payload[3], _payload[2])/10;   // PM10
        log_debug(F("\n[pm_serial][SDS011] PM10 = "));log_debug(value);log_flush();
        _measures[(uint8_t)sds011DataIdx_t::PM10]._currentSum += (float)value;

        // data acquired, finisk :)
        log_debug(F("\n[pm_serial][SDS011] "));log_debug(millis()-startTime);
        log_debug(F("ms reading data over serial link"));log_flush();
        _index = 0; // useless since we return now ...
        return true;

      default:
        if(_index == 2) _calculatedChecksum = ch;
        else _calculatedChecksum += ch;

        if( _index-2 < sizeof(_payload) ) _payload[_index-2] = ch;
    }
    _index++;

  } while( millis() - startTime < timeout );

  return false;
}


/*!
 * IKEA: serial data grabber and parser
 * @note blocking call till data OK or timeout
 */
boolean pm_serial::serialRead_ikea( uint16_t timeout ) {

  uint8_t _index = 0;
  uint16_t _frameLen = 0;
  uint16_t _checksum = 0;
  uint16_t _calculatedChecksum = 0;
  uint8_t _payload[16]; // [feb.22] newer command send back up to 16 bytes (no idea DF13-DF16 role)

  unsigned long startTime=millis();

  do {
    // serial data available ?
    if( !_stream->available() ) {
      delay(5); continue;
    }

    // read serial char
    uint8_t ch = _stream->read();

    // switch to byte position in frame
    switch( _index ) {

      case 0:
        if( ch != 0x16 ) continue;
        _calculatedChecksum = ch;
        break;

      case 1:
        _calculatedChecksum += ch;
        _frameLen = ch;
        break;

      case 2:
        // command byte answer: it could be 0x02 or 0x0b
        /*
        if( ch != 0x02 ) {
          _index = 0;
          continue;
        }
        */
        _calculatedChecksum += ch;
        break;

      default:
        if(_index == _frameLen + 2) {
          _checksum = ch;
          
          if( uint8_t(_calculatedChecksum+_checksum) != (uint8_t)0 ) {
            // bad checksum ...
            log_warning(F("\n[pm_serial][IKEA] bad checksum ...")); log_flush();
            _index = 0;
            continue;
          }

          uint16_t value;
          // Atmospheric Environment.
          //value = makeWord(_payload[6], _payload[7]);   // PM1_0
          value = makeWord(_payload[2], _payload[3]);     // PM2_5
          log_debug(F("\n[pm_serial][IKEA] PM2_5 = "));log_debug(value);log_flush();
          _measures[(uint8_t)ikeaDataIdx_t::PM2_5]._currentSum += (float)value;
          log_debug(F("\t_currentSum = "));log_debug(_measures[(uint8_t)ikeaDataIdx_t::PM2_5]._currentSum);
          log_flush();
          value = makeWord(_payload[10], _payload[11]);   // PM10
          log_debug(F("\n[pm_serial][IKEA] PM10 = "));log_debug(value);log_flush();
          _measures[(uint8_t)ikeaDataIdx_t::PM10]._currentSum += (float)value;
          // [feb.22] no idea DF13-DF16 bytes from 0x0B command are useful for ??

          // data acquired, finisk :)
          log_debug(F("\n[pm_serial][IKEA] "));log_debug(millis()-startTime);
          log_debug(F("ms reading data over serial link"));log_flush();
          _index = 0; // useless since we return now ...
          return true;

        }
        else {
          // payload acquire
          _calculatedChecksum += ch;
          uint8_t payloadIndex = _index - 3;

          if( payloadIndex < sizeof(_payload) ) _payload[payloadIndex] = ch;
        }
    }
    _index++;

  } while( millis() - startTime < timeout );

  return false;
}

/*!
 * MH-Z1x: serial data grabber and parser
 * @note blocking call till data OK or timeout
 */
boolean pm_serial::serialRead_mhz1x( uint16_t timeout ) {

  uint8_t _index = 0;
  uint16_t _frameLen = 0;
  uint16_t _checksum = 0;
  uint16_t _calculatedChecksum = 0;
  uint8_t _payload[6];

  unsigned long startTime=millis();

  do {
    // serial data available ?
    if( !_stream->available() ) {
      delay(5); continue;
    }

    // read serial char
    uint8_t ch = _stream->read();

    // switch to byte position in frame
    switch( _index ) {

      case 0:
        if( ch != 0xff ) continue;
        break;

      case 1:
        if( ch != 0x86 ) {
          _index = 0;
          continue;
        }
        _calculatedChecksum = ch; // yes, preamble is not part of checksum
        _frameLen = sizeof(_payload);  // fixed frame length
        break;

      default:
        if(_index == _frameLen + 2) {
          _checksum = ch;
          
          if( uint8_t(~_calculatedChecksum + 1) != (uint8_t)_checksum ) {
            // bad checksum ...
            log_warning(F("\n[pm_serial][MHZ-1x] bad checksum ...")); log_flush();
            _index = 0;
            continue;
          }

          uint16_t value;
          value = makeWord(_payload[0], _payload[1]);     // CO2
          log_debug(F("\n[pm_serial][MHZ-1x] CO2 = "));log_debug(value);log_flush();
          _measures[(uint8_t)mhz1xDataIdx_t::CO2]._currentSum += (float)value;
          log_debug(F("\t_currentSum = "));log_debug(_measures[(uint8_t)mhz1xDataIdx_t::CO2]._currentSum);
          log_flush();

          // data acquired, finisk :)
          log_debug(F("\n[pm_serial][MHZ-1x] "));log_debug(millis()-startTime);
          log_debug(F("ms reading data over serial link"));log_flush();
          _index = 0; // useless since we return now ...
          return true;
        }
        else {
          // payload acquire
          _calculatedChecksum += ch;
          uint8_t payloadIndex = _index - 2;

          if( payloadIndex < sizeof(_payload) ) _payload[payloadIndex] = ch;
        }
    }
    _index++;

  } while( millis() - startTime < timeout );

  return false;
}
