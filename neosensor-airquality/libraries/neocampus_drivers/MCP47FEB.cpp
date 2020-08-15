/**************************************************************************/
/*! 
    @file     MCP47FEB.cpp
    @author   F. Thiebolt
	  @license
	
    This is part of a the neOCampus drivers library.
    MCP47FEB01 is a 8bits single DAC with I2C interface
    MCP47FEB02 is a 8bits dual DAC with I2C interface,
    MCP47FEB11 is a 10bits single DAC with I2C interface
    MCP47FEB12 is a 10bits dual DAC with I2C interface,
    MCP47FEB21 is a 12bits single DAC with I2C interface
    MCP47FEB22 is a 12bits dual DAC with I2C interface.

    Remember: all transfers are 16bits wide

    (c) Copyright 2017 Thiebolt F. <thiebolt@irit.fr>

	@section  HISTORY

    2017-Oct    - First release, 8bits single DAC support F. Thiebolt
    
*/
/**************************************************************************/


#include <Arduino.h>

#include "neocampus.h"
#include "neocampus_debug.h"
#include "neocampus_i2c.h"

#include "MCP47FEB.h"



/**************************************************************************/
/*! 
    @brief  Declare list of possible I2C addrs
*/
/**************************************************************************/
const uint8_t MCP47FEB::i2c_addrs[4] = { 0x60, 0x61, 0x62, 0x63 };



/**************************************************************************/
/*! 
    @brief  Test if device at 'adr' is really a MCP47FEB
*/
/**************************************************************************/
boolean MCP47FEB::is_device( uint8_t a ) {
  
  // First step, parse all addresses
  boolean found = false;
  for( uint8_t i=0; i < sizeof(MCP47FEB::i2c_addrs); i++ ) {
    if( MCP47FEB::i2c_addrs[i] == a) {
      found = true;
      break;
    }
  }
  if( found == false ) return false;
    
  /*
   * Read specific register to assert it is a MCP47FEB ...
   * register 0x1A ought to contain current I2C address
   */
  if( (uint8_t)(_read16(a, MCP47FEB_NVREG_I2C) & (uint16_t)MCP47FEB_I2C_MASK) != a ) return false;
  
  // ... okay
  return true;
}



/******************************************
 * Default constructor, uses default I2C address.
 */
MCP47FEB::MCP47FEB() : driver_dac() {
    _i2caddr = -1;
    _resolution = -1;
    _output = 0;    // default value will be defined later once we'll have discovered the kind of chip it is
    _initialized = false;
}



/******************************************
 * Power on and prepare for general usage.
 */
bool MCP47FEB::begin( uint8_t adr ) {
  _i2caddr = adr;
  
  // determine resolution
  // uint16_t _dac0 = _read16( adr, MCP47FEB_REG_DAC0);  // read DAC0 VOLATILE REG
  uint16_t _dac0 = _read16( adr, MCP47FEB_NVREG_DAC0);  // read DAC0 NVREG
  // DEBUG DEBUG
  // log_debug(F("\n[MCP47FEB] MCP47FEB_REG_DAC0 = 0x")); log_debug(_dac0,HEX); log_flush();
  
  if( _dac0 == (uint16_t)0x7F ) _resolution = 8;
  else if( _dac0 == (uint16_t)0x1FF ) _resolution = 10;
  else if( _dac0 == (uint16_t)0x7FF ) _resolution = 12;
  else if( _dac0 > (uint16_t)0x7FF ) {
    log_warning(F("\n[MCP47FEB] _dac0 is a bit odd since we can't have resolution > 12bits ?!?! ... set to 12bits :|")); log_flush();
    _resolution = 12;
  }
  else {
    log_error(F("\n[MCP47FEB] unable to determine DAC resolution ?!?! assuming 8bits")); log_flush();
    _resolution = 8;
  }

  // DEBUG DEBUG
  log_debug(F("\n[MCP47FEB] detected resolution = ")); log_debug(_resolution,DEC); log_debug(F("bits")); log_flush();
  
  // chip is initialized
  _initialized = true;

  // power up device
  setPowerMode( MCP47FEB_NORMAL_OPS );
  
  // default output level is max/4
  _output = (uint16_t)( (pow(2,_resolution) / 4) - 1 );
  
  // set dac output
  setOutputLevel( _output );

  return true;
}



/******************************************
 * Set the configuration bits for the DAC
 */
void MCP47FEB::setPowerMode( uint8_t pwrMode ) {
  // write volatile Power Mode for DAC(s) if initialized
  if( _initialized != true ) return;
  
  uint16_t _pwr;
  _pwr = _read16( _i2caddr, MCP47FEB_REG_POWER ) & MCP47FEB_POWER_MASK;
  _pwr |= ( pwrMode & ~MCP47FEB_POWER_MASK );
  // write power mode to volatile register
  _write16( _i2caddr, MCP47FEB_REG_POWER, _pwr );
}

void MCP47FEB::setGain( uint8_t gain ) {
  // write volatile Gain Mode for DAC(s) if initialized
  if( _initialized != true ) return;
  
  uint16_t _gain;
  _gain = _read16( _i2caddr, MCP47FEB_REG_GAIN ) & MCP47FEB_GAIN_MASK;
  _gain |= ( gain & ~MCP47FEB_GAIN_MASK );
  // write gain to volatile register
  _write16( _i2caddr, MCP47FEB_REG_GAIN, _gain );
}

void MCP47FEB::setVRef( uint8_t vref ) {
  // write volatile Vref for DAC(s) if initialized
  if( _initialized != true ) return;
  
  uint16_t _vref;
  _vref = _read16( _i2caddr, MCP47FEB_REG_VREF ) & MCP47FEB_VREF_MASK;
  _vref |= ( vref & ~MCP47FEB_VREF_MASK );
  // write vref to volatile register
  _write16( _i2caddr, MCP47FEB_REG_VREF, _vref );

  /* TEST TEST TO REMOVE!!
  uint16_t _val = _read16( _i2caddr, MCP47FEB_REG_VREF );
  log_debug(F("\n[MCP47FEB][DAC0] and re-read VREF setup ..."));log_debug(_val,DEC); log_flush();
  */
}



/*********************************************
 * Set output level 
 */
// set output level for DAC0 volatile
uint16_t MCP47FEB::setOutputLevel( uint16_t level ) {
  // check argument
  uint16_t _max = pow(2,_resolution)-1;
  if( level > _max ) level = _max;
  
  // set output value
  _output = level;

  log_debug(F("\n[MCP47FEB][DAC0] output level set to "));log_debug(_output,DEC); log_flush();
  
  // set output to DAC0 volatile register
  _write16( _i2caddr, MCP47FEB_REG_DAC0, _output );    

  /* TEST TEST TO REMOVE!!
  uint16_t _val = _read16( _i2caddr, MCP47FEB_REG_DAC0 );
  log_debug(F("\n[MCP47FEB][DAC0] and re-read output level ..."));log_debug(_val,DEC); log_flush();
  */
  
  return _output;
}
 
// Set percent output level
uint8_t MCP47FEB::setPercentOutput( uint8_t percent ) {
  
  /* 
   * compute DAC output level according to DAC resolution 
   * and noise detection subsystem.
   */
  uint32_t val;
  val = ((uint32_t)percent * pow(2,_resolution)) / (uint32_t)100;

  setOutputLevel( (uint16_t)val );
  
  // TODO: we *normally* ought to compute value returned by setOuputLevel
  // to compute the corresponding percent valut applied to the output
  return percent;
}


 
/*************************************************
 * Low-level functions
 */
// i2c specific read16
uint16_t MCP47FEB::_read16( uint8_t adr, uint8_t reg ) {
  
  uint8_t _tab[2];
  
  if( readList( adr, (reg<<3)|0x6, _tab, sizeof(_tab) )!=sizeof(_tab) ) {
    log_error(F("\n[MCP47FEB] error while reading reg 0x"));log_error(reg,HEX);log_flush();
    return -1;
  }
  
  return (uint16_t)(_tab[0]<<8 | _tab[1] );
}

// i2c specific write16
void MCP47FEB::_write16( uint8_t adr, uint8_t reg, uint16_t val ) {
    write16( adr, (reg<<3), val);
}

