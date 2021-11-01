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

#include "pms_serial.h"   // neOCampus driver


/* 
 * Definitions
 */


/* declare kind of units (value_units) */
const char *pms_serial::units = "µg/m3";


/**************************************************************************/
/*! 
    @brief  Instantiates a new pms_serial class
*/
/**************************************************************************/
pms_serial::pms_serial( void ) : generic_driver() {
  _initialized = false;

  _psensor = nullptr;
  _link = SENSORS_SERIAL_LINK;        // serial link number (e.g 2 --> Serial2)
  _link_speed = PMS_DEFL_LINK_SPEED;  // serial link bauds rate

  /* [nov.21] we choose to disable PM%_ENABLE gpio because PMS sensors
   * already features both sleep() and wakeUp() commands
  _enable_gpio = PM_ENABLE;           // PM_ENABLE gpio
  */
  _enable_gpio = INVALID_GPIO
}


/*
 * Power modes: ON / OFF
 */
void pms_serial::powerOFF( void )
{
  if( !_initialized ) return;
  if( ! _psensor) return;

  if( _enable_GPIO != INVALID_GPIO ) {
    log_debug(F("\n[pms_serial] set PMS in sleep state through GPIO "));log_debug(_enable_gpio);log_flush();
    pinMode( _enable_gpio, OUTPUT );    // output value has already been set to LOW
  }
  else {
    log_debug(F("\n[pms_serial] set PMS in sleep state through sleep() command"));log_flush();
    _psensor->sleep();
  }
}

void pms_serial::powerON( void )
{
  if( !_initialized ) return;
  if( ! _psensor) return;

  if( _enable_GPIO != INVALID_GPIO ) {
    log_debug(F("\n[pms_serial] wakeup PMS through GPIO "));log_debug(_enable_gpio);log_flush();
    pinMode( _enable_gpio, INPUT );    // output value has already been set to LOW
  }
  else {
    log_debug(F("\n[pms_serial] wakeup PMS through wakeUp() command"));log_flush();
    _psensor->wakeUp();
  }
}


/**************************************************************************/
/*! 
    @brief  Extract JSON config parameters to initialize the HW
*/
/**************************************************************************/
boolean pms_serial::begin( JsonVariant root ) {

  // check input(s)
  if( root.isNull() or not root.is<JsonArray>() ) {
    log_error(F("\n[pms_serial] either empty JSON or wrong JSON type (array expected)!")); log_flush();
    return false;
  }

//  log_debug(F("\n[pms_serial] params found :)\n")); log_flush();
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

    // LINK
    {
      const char *_param = PSTR("link");
      if( strncmp_P(item[F("param")], _param, strlen_P(_param))==0 ) {
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


TO BE CONTINUED


/**************************************************************************/
/*! 
    @brief  sensor internal processing
*/
/**************************************************************************/
void pms_serial::process( void )
{
  if( !_initialized ) return;

  // process according to our FSM
  switch( _FSMstatus ) {

    // IDLE
    case lccSensorState_t::idle:
      log_debug(F("\n\t[lcc_sensor]["));log_debug(_subID);log_debug(F("] about to start a new acquisition cycle ...")); log_flush();
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
}


/**************************************************************************/
/*! 
    @brief  return sensor value.
            Note that we send back values collected during the internal
            sensor processing.
*/
/**************************************************************************/
boolean lcc_sensor::acquire( float *pval )
{
  /* it's not possible to generate the data on the fly because there are
   * some huge delays (especially with pulse mode) before reading a data.
   * Hence, average data collected during the sensor internal processing
   * (i.e process()) we'll be sent back now.
   */

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

  return true;
}


/* ------------------------------------------------------------------------------
 * Private'n Protected methods 
 */


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


/**************************************************************************/
/*! 
    @brief  Low-level HW initialization
*/
/**************************************************************************/
boolean pms_sensor::_init( void ) {

  _initialized = false;

  /* PMS instantiation with selected serial link.
   * Note: link number is the serialX stream object
   */
  if( _link != SENSORS_SERIAL_LINK ) {
    log_error(F("\n[pms_serial] inappropriate serial link number "));log_error(_link);log_flush();
    return _initialized;
  }

  // configure serial link and instantiate PMS low-level driver
  Serial2.begin( _link_speed );
  if( !_psensor ) {
    _psensor = new PMS(Serial2);
    if( _psensor == nullptr ) {
      log_error(F("\n[pms_serial] failed to instantiate PMS(serial_link) ?!?!"));log_flush();
      return _initialized;
    }

$    log_debug(F("\n[pms_serial] successfully instantiated PMS(serial_link) sensor :)"));log_flush();
  }
  else {
    log_debug(F("`\n[pms_serial} PMS already instantiated ?! ... continuing"));log_flush();
  }

  // switch to passive mode
  log_debug(F("\n[pms_serial] switch to passive mode"));log_flush();
  _psensor->passiveMode();delay(5);

  // powerOFF module
  powerOFF();

  // flush serial buffer
  while( Serial2.available() ) Serial2.read();

  // configure enable_gpio if any
  if( _enable_gpio != INVALID_GPIO ) {
    pinMode( _enable_gpio, INPUT );
    digitalWrite( _enable_gpio, LOW );  // this way, INPUT ==> normal ops, OUTPUT ==> sensor disabled
  }

  // set FSM initial state
  _FSMstatus = PMS_FSMSTATE_DEFL;
  _FSMtimerDelay = 0;

  return _initialized=true;
}

