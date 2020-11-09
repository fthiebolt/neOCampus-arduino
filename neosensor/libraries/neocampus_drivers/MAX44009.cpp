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
const uint8_t MAX44009::i2c_addrs[2] = { 0x4a, 0x4b };

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
  setIntegration( _integrationTime );

  // power OFF chip ...
  powerOFF();

  return true;
}


/**************************************************************************/
/*! 
    @brief  set Mode (auto or manual)
*/
/**************************************************************************/
boolean MAX44009::setIntegration( max44009IntegrationT_t ms=max44009IntegrationT_t::ms_integrate_auto ) {

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
  uint16_t _res = 0;
  _res = (uint16_t)read8(a, static_cast<uint8_t>(max44009Regs_t::config));
  //log_debug(F("\n[MAX44009] REG_CONFIG = 0x")); log_debug(_res,HEX); log_flush();
  if( (uint8_t)_res != (uint8_t)MAX44009_REG_CONFIG_DEFL &&
      (uint8_t)_res != (uint8_t)MAX44009_REG_CONFIG_DEFL_ALT ) return false;

  // read threshold register
  _res = read16(a, static_cast<uint8_t>(max44009Regs_t::threshold_upper));  
  //log_debug(F("\n[MAX44009] REG_THRESHOLD = 0x")); log_debug(_res,HEX); log_flush();
  if( _res != (uint16_t)MAX44009_REG_THRESHOLD_DEFL &&
      _res != (uint16_t)MAX44009_REG_THRESHOLD_DEFL_ALT ) return false;

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

  // check for auto mode with continuous integration
  // [nov.20] only auto mode suported
  if( _integrationTime != max44009IntegrationT_t::ms_integrate_auto ) {
    log_warning(F("\n[MAX44009] only support for AUTO mode right now!")); log_flush();
    return false;
  }

  // switvh ON device
  powerON();

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

  // switvh OFF device
  powerOFF();

  /*
   * LUX = 2^exp*mantissa*0.72
   */
  uint8_t exposant, mantisse;
  
  // exposant
  exposant = buf[0] >> 4;
  if( exposant >= (uint8_t)0x0F ) {
    log_debug(F("\n[MAX44009] overrange exposant !")); log_flush();
    return false;
  }

  // mantisse
  mantisse = (buf[0] & 0xF0)<<4 | (buf[1]&0x0F);

  // LUX computation
  *pval = (float)(pow(2,exposant) * 0.72 * mantisse);

  return true;
}
