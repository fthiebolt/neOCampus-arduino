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



/**************************************************************************/
/*! 
    @brief  Test if device at 'adr' is really a MAX44009
*/
/**************************************************************************/
boolean MAX44009::is_device( uint8_t a ) {
  
  // First step, parse all addresses
  boolean found = false;
  for( uint8_t i=0; i < sizeof(MAX44009::i2c_addrs); i++ ) {
    if( MAX44009::i2c_addrs[i] == a) {
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
  _integrationTime = max44009IntegrationT_t::ms_integrate_auto;
}


/*
 * Power modes: ON or powerOFF
 */
void MAX44009::powerOFF( void )
{
  // TODO: stop continuous integration
}

void MAX44009::powerON( void )
{
  // TODO: start continuous integration
}


/**************************************************************************/
/*! 
    @brief  Setups the HW
*/
/**************************************************************************/
boolean MAX44009::begin( uint8_t addr=INVALID_I2CADDR ) {
  // get i2caddr
  if( (addr < (uint8_t)(I2C_ADDR_START)) or (addr > (uint8_t)(I2C_ADDR_STOP)) ) return false;
  _i2caddr = addr;

  // power ON chip ...
  powerON();

  // check device identity
  if( !_check_identity(_i2caddr) ) return false;

  // setup things ...
  setMode( _mode );

  // power OFF chip ...
  powerOFF();

  return true;
}


/**************************************************************************/
/*! 
    @brief  set Mode (auto or manual)
*/
/**************************************************************************/
boolean MAX44009::setMode( max44009IntegrationT_t ms=max44009IntegrationT_t::ms_integrate_auto ) {

  // [nov.20] only auto mode suported
  if( ms != max44009IntegrationT_t::ms_integrate_auto ) {
    log_warning(F("\n[MAX44009] only support for AUTO mode right now!")); log_flush();
    return false;
  }

  // set auto mode
  write8( _i2caddr, static_cast<uint8_t>(max44009Regs_t::config), MAX44009_AUTO_RANGING );

  return true;
}


/**************************************************************************/
/*! 
    @brief  Read registers and convert returned lux value to float
*/
/**************************************************************************/
boolean MAX44009::acquire( float *pval )
{
  // retrieve LUMINOSITY
  return _getLux( pval );
}



/* ------------------------------------------------------------------------------
 * Private methods 
 */

/*
 * Check that device identity is what we expect!
 */
bool MAX44009::_check_identity( uint8_t a ) {
  
  // read control register
  uint8_t _res = read8(a, max44009Regs_t::config);
  if( _res != (uint8_t)MAX44009_REG_CONFIG_DEFL) return false;

  // read threshold register
  uint16_t _res = read16(a, max44009Regs_t::threshold_upper);  
  if( _res != (uint16_t)MAX44009_REG_THRESHOLD_DEFL) return false;

  return true;
}


/**************************************************************************/
/*! 
    @brief  Read registers and convert returned lux value to float
*/
/**************************************************************************/
boolean MAX44009::_getLux( float *pval )
{
  if( pval==nullptr ) return false;

  // switvh ON device
  powerON();

  // check for auto mode with continuous integration
  // [nov.20] only auto mode suported
  if( ms != max44009IntegrationT_t::ms_integrate_auto ) {
    log_warning(F("\n[MAX44009] only support for AUTO mode right now!")); log_flush();
    return false;
  }


  // retrieve HIGH and LOW bytes in a SINGLE I2C transaction (repeated START)
  uint8_t buf[2]; // 16bits data

  /*
   * read sensor's values register
   * Note: using I2C repeated start --> single i2c transaction hence coherency for both bytes
   * Note: since auto mode with continuous integration, no need to wait
   */
  if( readList( _i2caddr, static_cast<uint8_t>(max44009Regs_t::lux_upper), buf, sizeof(buf) ) != sizeof(buf) ) {
    log_error(F("\n[MAX44009] insufficient bytes answered"));log_flush();
    return false;
  }

  /*
   * LUX = 2^exp*mantissa*0.72
   */



TO BE CONTINUED

  *pval = ...


  // switvh OFF device
  powerOFF();

  return true;
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
