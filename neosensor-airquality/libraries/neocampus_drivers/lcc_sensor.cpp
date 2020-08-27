/**************************************************************************/
/*! 
  @file     lcc_sensor.cpp
  @author   F.Thiebolt (neOCampus / Univ.Tlse3)
  @license  
	
	LCC's Airquality sensors driver for neOCampus

	@section  HISTORY

    aug.20  F.Thiebolt  neOCampus integration
                        adapted for neOCampus
    2020  Aymen Sendhi  sensor low-level functions
*/
/**************************************************************************/


/*
 * Includes
 */
#include <Arduino.h>
#if defined(ESP32) && !defined(DISABLE_ADC_CAL)
  #include <esp_adc_cal.h>
  extern esp_adc_cal_characteristics_t *adc_chars;
#endif /* ESP32 adcanced ADC */

#include "neocampus.h"
#include "neocampus_debug.h"

#include "lcc_sensor.h"


/* 
 * Definitions
 */


/* Notes about ADC on esp32:
 * Arduino's defaults are: 12bits resolution, 8 samples, non-linearity above 2.5v
 *    ADC sar vref is 1.1v but may shift in the 1.0 to 1.2v range (ability to read this
 *    through ADC2 ---no WiFi use, to implement specific corrections).
 *  11DB attenuation (1/3.6 means 3.3v input --> 0.916v near the 1.1 ref voltage)
 * https://www.esp32.com/viewtopic.php?f=12&t=1045
 * https://forum.arduino.cc/index.php?topic=580984.0
 * 
 * Finally, new ESP32 are factory calibrated 
 * https://github.com/espressif/arduino-esp32/issues/1804#issuecomment-475281577
 * https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/adc.html#adc-calibration
 * 
 */

/* declare kind of units (value_units) */
const char *lcc_sensor::units = "ppm";
//const char *lcc_sensor::units = "ohm";  better to avoid this, we really need to give a ppm value


/**************************************************************************/
/*! 
    @brief  Instantiates a new lcc_sensor class
*/
/**************************************************************************/
lcc_sensor::lcc_sensor( void ) : generic_driver() {
  _initialized = false;
  _subID[0] = '\0';
  _heater = INVALID_GPIO;
  for( uint8_t i=0; i < sizeof(_inputs); i++ ) {
    _inputs[i] = INVALID_GPIO;
  }
}


/**************************************************************************/
/*! 
    @brief  Extract JSON config parameters to initialize the HW
*/
/**************************************************************************/
boolean lcc_sensor::begin( JsonVariant root ) {

  // check input(s)
  if( root.isNull() or not root.is<JsonArray>() ) {
    log_error(F("\n[lcc_sensor] either empty JSON or wrong JSON type (array expected)!")); log_flush();
    return false;
  }

  //log_debug(F("\n[lcc_sensor] params found :)\n")); log_flush();
  //serializeJsonPretty( root, Serial );

  boolean _param_subID = false;
  boolean _param_input = false;
  boolean _param_output = false;
  /* parse all parameters of  our sensor:
  [
    {
      "param": "subIDs",
      "value": "NO2"
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
        ]
      ]
    },
    {
      "param": "outputs",
      "value": -1
    }
  ]
  */
  for( JsonVariant item : root.as<JsonArray>() ) {

    if( item.isNull() or not item.is<JsonObject>() ) {
      log_warning(F("\n[lcc_sensor] format error while parsing parameters !")); log_flush();
      continue;
    }

    // SUBID
    {
      const char *_param = PSTR("subID");
      if( strncmp_P(item[F("param")], _param, strlen_P(_param))==0 ) {
        snprintf( _subID, sizeof(_subID), item[F("value")]);
        _param_subID = true;
      }
    }

    // INPUT(S)
    {
      const char *_param = PSTR("input");
      if( strncmp_P(item[F("param")], _param, strlen_P(_param))==0 ) {
        if( ! item[F("value")].is<JsonArray>() ) {
          log_error(F("\n[lcc_sensor] expecting inputs as a JSON array (of int) ?!")); log_flush();
          continue;
        }
        JsonArray gpio_root = item[F("value")];
        for( uint8_t i=0; i < min(sizeof(_inputs),gpio_root.size()); i++ ) {
          _inputs[i] = (uint8_t)gpio_root[i].as<int>();   // to force -1 to get converted to (uint8_t)255
        }
        _param_input = true;
      }
    }

    // OUTPUT(S)
    {
      const char *_param = PSTR("output");
      if( strncmp_P(item[F("param")], _param, strlen_P(_param))==0 ) {
        _heater = (uint8_t)item[F("value")].as<int>();    // to force -1 to get converted to (uint8_t)255
        _param_output = true;
      }
    }

  }


  /* DEBUG DEBUG DEBUG
  log_debug(F("\n[lcc_sensor] driver created with subID: ")); log_debug(_subID);
  log_debug(F("\n[lcc_sensor] heater: ")); log_debug(_heater,DEC);
  log_debug(F("\n[lcc_sensor] inputs: "));
  for( uint8_t i=0; i < sizeof(_inputs); i++ ) {
    log_debug(_inputs[i],DEC);log_debug(F(" "));
  }
  log_flush();
  */

  // check whether all parameters are set ...
  if( _param_subID and _param_input and _param_output ) return false;

  /*
   * sensor HW initialisation
   */
  return _init();
}


/**************************************************************************/
/*! 
    @brief  data retrieval and conversion from sensor
*/
/**************************************************************************/
float lcc_sensor::getSensorData( void )
{
  /*
   * Sensor data acquisition overall process
   * - select highest amplification that produce a measured voltage < ADC_MEASURE_THRESHOLD
   */
  
  return (float)0.0;
}


/**************************************************************************/
/*! 
    @brief  return sensor value read from pins
*/
/**************************************************************************/
float lcc_sensor::acquire( void )
{
  return getSensorData();
}


/**************************************************************************/
/*! 
    @brief  set heater mode (i.e ON/OFF) if GPIO pin specicifed
            second parameter pulse: 0 ==> toggle, otherwise ms impulse width
*/
/**************************************************************************/
void lcc_sensor::setHeater( lccSensorHeater_t mode, uint8_t pulse_ms=0 ) {

  if( _heater==INVALID_GPIO ) return;

  if( mode==lccSensorHeater_t::heater_pulse and pulse_ms ) {
    digitalWrite( _heater, !digitalRead(_heater) );
    delay( pulse_ms );   // max 255ms
    digitalWrite( _heater, !digitalRead(_heater) );
  }
  else if( mode<lccSensorHeater_t::heater_pulse ) {
    digitalWrite( _heater, (mode==lccSensorHeater_t::heater_off ? LOW : HIGH) );
  }
  else {
    log_warning(F("\n[lcc_sensor] inconsistent pulse mode & value!")); log_flush();
    return;
  }
}



/* ------------------------------------------------------------------------------
 * Private methods 
 */

/**************************************************************************/
/*! 
    @brief  Low-level HW initialization
*/
/**************************************************************************/
boolean lcc_sensor::_init( void ) {

  // configure gpio
  _reset_gpio();

  // powerON module
  powerON();  // as of [aug.20] there's no power settings
/*
  set state machine
  setGain();
  setTiming();
*/
  // powerOFF module
  powerOFF();  // as of [aug.20] there's no power settings

  _initialized = true;

  return _initialized;
}


/**************************************************************************/
/*! 
    @brief  Reset GPIO to their initial state
*/
/**************************************************************************/
void lcc_sensor::_reset_gpio( void ) {

  // configure gpio inputs
  for( uint8_t pin : _inputs ) {
    if( pin==INVALID_GPIO ) continue;
    pinMode( pin, INPUT );
  }

  // configure analog_input
#if defined(ESP32)
  #if !defined(DISABLE_ADC_CAL)
  if( _inputs[LCC_SENSOR_ANALOG]!=INVALID_GPIO ) {
    /* the default 11db attenuation enables analog input full range
     * Note: unsure if it's not already done somewhere ...
     */
    adc1_config_channel_atten( (adc1_channel_t)digitalPinToAnalogChannel(_inputs[LCC_SENSOR_ANALOG]), ADC_ATTEN_DB_11 );
  }
  #else
  /*
   * regular ADC configuration, defaults are:
   * - 8 times sampling
   * - 11dB attenuation
   * - 12 bits resolution
   */
  switch(ADC_RESOLUTION) {
    case ADC_WIDTH_BIT_11:
      analogSetWidth( 11 );
    case ADC_WIDTH_BIT_10:
      analogSetWidth( 10 );
  }
  #endif
#endif /* ESP32 adcanced ADC */

  // configure gpio output
  if( _heater != INVALID_GPIO ) {
    pinMode( _heater, OUTPUT );
    setHeater( LCC_SENSOR_HEATER_DEFL );
  }
}

