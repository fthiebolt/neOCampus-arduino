/**************************************************************************/
/*
    @file     MAX44009.h
    @author   L.Jeanmougin & F.Thiebolt
    @section LICENSE
	  @license
	
    This is part of a the neOCampus drivers library.
    MAX44009 is a luminosity sensor featuring an I2C interface
    It allows measures up to 188.000 lux !!
        
    (c) Copyright 2020 Thiebolt.F <thiebolt@irit.fr>

	@section  HISTORY

    2020-Nov  - F.Thiebolt    integration
    2020-Oct  - L.Jeanmougin  Initial release
*/
/**************************************************************************/


#include <Arduino.h>

#include "neocampus.h"
#include "neocampus_debug.h"
#include "neocampus_i2c.h"

#include "MAX44009.h"



/**************************************************************************/
/*! 
    @brief  Declare list of possible I2C addrs
    Note: two MAX44009 i2c addr exists: 0x4A & 0x4B
*/
/**************************************************************************/
const uint8_t MAX44009::i2c_addrs[16] = { 0x4a, 0x4b };

/* declare kind of units */
const char *MAX44009::units = "lux";



WARNING: repeated start required to read I2C

TO BE CONTINUED



/*
 * Definitions
 */
#define MAX44009_DELAY_INTTIME_800MS (900)
#define MAX44009_DELAY_INTTIME_400MS (450)
#define MAX44009_DELAY_INTTIME_200MS (225)
#define MAX44009_DELAY_INTTIME_100MS (112)
#define MAX44009_DELAY_INTTIME_50MS  (56)
#define MAX44009_DELAY_INTTIME_25MS  (28)
#define MAX44009_DELAY_INTTIME_12MS  (14)
#define MAX44009_DELAY_INTTIME_6MS   (8)
/*#define MAX44009_DELAY_INTTIME_13MS    (15)
#define MAX44009_DELAY_INTTIME_101MS   (120)
#define MAX44009_DELAY_INTTIME_402MS   (450)
*/




/**************************************************************************/
/*! 
    @brief  Test if device at 'adr' is really a MAX44009
*/
/**************************************************************************/
boolean MAX44009::is_device( uint8_t a ) {
  
  // First step, parse all addresses
  boolean found = false;
  log_debug(F("\nStarting MAX adress test...")); log_debug(a); log_flush();
  for( uint8_t i=0; i < sizeof(MAX44009::i2c_addrs); i++ ) {
    if( MAX44009::i2c_addrs[i] == a) {
      log_debug(F("\n Comparing")); log_debug(MAX44009::i2c_addrs[i]); log_debug(a); log_flush();
      found = true;
      break;
    }
  }
  if( found == false ) return false;

  // check device identity
  if( !_check_identity(a) ) return false;

  // ... okay, we're probably a MAX44009 ;)
  return true;
}



/*
 * Constructor
 */
MAX44009::MAX44009( void ) : generic_driver() {
  _i2caddr = INVALID_I2CADDR;
  _initialized = false;
  _integration = MAX44009_DEFL_INTEGR_TIME;
}



boolean MAX44009::begin( uint8_t addr=INVALID_I2CADDR ) {
  // get i2caddr
  if( (addr < (uint8_t)(I2C_ADDR_START)) or (addr > (uint8_t)(I2C_ADDR_STOP)) ) return false;
  _i2caddr = addr;
  // check device identity
  if( !_check_identity(_i2caddr) ) return false;

  // Set default integration time and gain
  setTiming(_integration);
  //enable interrupt Register
  write8(_i2caddr, MAX44009_REGISTER_INTERRUPT_ENABLE, 0x01);
  log_debug(F("\n[MAX44000]Register 0x01 writen with value : "));log_debug(read8(_i2caddr, 0x01)) log_flush();
  write8(_i2caddr, MAX44009_REGISTER_CONFIG, MAX44009_DEFAULT_INTEGRATION_MODE | MAX44009_DEFAULT_CONFIG);
  log_debug(F("\n[MAX44000]Register 0x02 writen with value : "));log_debug(read8(_i2caddr, 0x02)); log_flush();
  // chip is now initialized
  _initialized = true;
  return true;
}

void MAX44009::setTiming(MAX44009IntegrationTime_t integration) {
  if (!_initialized) begin();
  _integration = integration;
  write8(_i2caddr, MAX44009_REGISTER_CONFIG, _integration); //has to be checked
}


/* ------------------------------------------------------------------------------
 * Private methods 
 */

/*
 * Check that device identity is what we expect!
 */
bool MAX44009::_check_identity( uint8_t a ) {
  uint8_t _res;
  //_res = read8(a, MAX44009_REGISTER_THRESHHOLDH_LOW);
  //if( _res != 0 ) return false;
  // log_debug(F("\n_res = ")); log_debug(_res);log_flush();
  _res = read8(a, MAX44009_REGISTER_CONFIG);
  // log_debug(F("\n_res = ")); log_debug(_res);log_flush();
  return (_res == (uint8_t)0x03);
  // check Register ID
}



/* ----------------------------------------------------------
 * Low-level channel 0 and channel 1 sensor acquisition
 *   Channel0 --> Visible + IR
 *   Channel1 --> IR only
 * - pointer to uint16_t that will hold ch0 and ch1 values
 * - return None
 */
void MAX44009::_getData (uint8_t* value) {
  /* Wait x ms for ADC to complete */
  switch( _integration )
  {
    case MAX44009_INTEGRATIONTIME_400MS:
      delay(MAX44009_DELAY_INTTIME_400MS);
      break;
    case MAX44009_INTEGRATIONTIME_200MS:
      delay(MAX44009_DELAY_INTTIME_200MS);
      break;
    case MAX44009_INTEGRATIONTIME_100MS:
      delay(MAX44009_DELAY_INTTIME_100MS);
      break;
    case MAX44009_INTEGRATIONTIME_50MS:
      delay(MAX44009_DELAY_INTTIME_50MS);
      break;
    case MAX44009_INTEGRATIONTIME_25MS:
      delay(MAX44009_DELAY_INTTIME_25MS);
      break;
    case MAX44009_INTEGRATIONTIME_12MS:
      delay(MAX44009_DELAY_INTTIME_12MS);
      break;
    case MAX44009_INTEGRATIONTIME_6MS:
      delay(MAX44009_DELAY_INTTIME_6MS);
      break;
    default:
      delay(MAX44009_DELAY_INTTIME_800MS);
      break;
  }
  return;
}



/*
 * Function that retrieves both broadband and IR channels of sensor
 * If 'auto Gain' is enabled, it may iterate with the various possible
 * gains.
 * - pointers to uint16_t that will hold ch0 and ch1 values
 * - return None
 */
void MAX44009::getLuminosity( uint8_t *value ) {
  if( !_initialized ) begin();
  uint8_t _interruptStatus;
  _interruptStatus = read8(_i2caddr, MAX44009_REGISTER_INTERRUPT_STATUS);
  if(_interruptStatus == 0x01){
    log_debug(F("\n[MAX44009] Ambient light intensity is outside the threshold window range .."))
  }

  // acquire data
  _getData( value );
  return;
}



/*
 * Compute LUX value according to ch0 and ch1 sensors values
 * - ch0 --> channel 0 sensor (broadband)
 * - ch1 --> channel 1 sensor (IR)
 * - return computed luminosity (lux)
 */
float MAX44009::calculateLux(uint8_t lowByte, uint8_t highByte)
{
  uint8_t mantissa = 0; //mantissa bytes
  uint8_t exponent = 0; //exponent bytes
  float lux;
  calculateMantissa(&mantissa);
  calculateExponent(&exponent);
  lux = pow(2,exponent) * mantissa * 0.045;
  return lux;
}

void MAX44009::calculateExponent(uint8_t* exponent){
  uint8_t highByteRegister = read8(_i2caddr, MAX44009_REGISTER_LUX_HIGH);
  *exponent = (highByteRegister&0x0f) / 16; 
}
void MAX44009::calculateMantissa(uint8_t* mantissa){
  uint8_t highByteRegister, lowByteRegister;
  highByteRegister = read8(_i2caddr, MAX44009_REGISTER_LUX_HIGH);
  lowByteRegister = read8(_i2caddr, MAX44009_REGISTER_LUX_LOW);
  *mantissa = (highByteRegister&0xf0 * 16) + (lowByteRegister&0xf0);
}


/*
 * Simple wrapper to retrieve lux value from sensor :)
 * - return Lux value :)
 */
boolean MAX44009::acquire( float *pval ) {

  uint8_t value;

  // get channels luminosity
  getLuminosity( &value );
  //*pval = 2^exp*mantissa*0.72
  *pval = 0;
  return true;
}