/**************************************************************************/
/*! 
    @file     MCP47x6.cpp
    @author   F. Thiebolt
	  @license
	
    This is part of a the neOCampus drivers library.
    MCP47x6 DAC 8/10/12bits with an I2C interface
        
    (c) Copyright 2017 Thiebolt.F <thiebolt@irit.fr>

	@section  HISTORY

    2017-July    - F.Thiebolt Initial release
    
*/
/**************************************************************************/


#include <Arduino.h>

#include "neocampus.h"
#include "neocampus_debug.h"
#include "neocampus_i2c.h"

#include "MCP47X6.h"



/**************************************************************************/
/*! 
    @brief  Declare list of possible I2C addrs
*/
/**************************************************************************/
const uint8_t MCP47X6::i2c_addrs[8] = { 0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67 };

/**************************************************************************/
/*! 
    @brief  Test if device at 'adr' is really a MCP47X6
*/
/**************************************************************************/
boolean MCP47X6::is_device( uint8_t a ) {
  
  // First step, parse all addresses
  boolean found = false;
  for( uint8_t i=0; i < sizeof(MCP47X6::i2c_addrs); i++ ) {
    if( MCP47X6::i2c_addrs[i] == a) {
      found = true;
      break;
    }
  }
  if( found == false ) return false;

  /*
   * UNABLE to read MCP47X6 devices because of the specific I2C protocol that does not match
   * those used at arduino :(
   */
  // unable to read anything so we assume it is ok :s

  // ... okay
  return true;
}



/******************************************
 * Default constructor, uses default I2C address.
 * @see MCP47X6_DEFAULT_ADDRESS
 */
MCP47X6::MCP47X6() : driver_dac() {
    _initialized = false;
    _i2caddr = -1;
    _config = MCP47X6_DEFL_CONFIG;
    _output = 0;    // default value will be defined later once we'll have discovered the kind of chip it is
    _resolution = -1;
}



/******************************************
 * Set the configuration bits for the DAC
 */
void MCP47X6::setGain( uint8_t gain ) {
  if( _initialized != true ) return;
  
  _config = (_config & MCP47X6_GAIN_MASK) | (gain & ~MCP47X6_GAIN_MASK);

  // apply config (only if initialized
  setConfig();
}

void MCP47X6::setVRef( uint8_t vref ) {
  if( _initialized != true ) return;
  
  _config = (_config & MCP47X6_VREF_MASK) | (vref & ~MCP47X6_VREF_MASK);

  // apply config (only if initialized
  setConfig();
}



/******************************************
 * Power on and prepare for general usage.
 * This device recalls the previously saved state at power up. The begin()
 * function reads these values from the DAC and uses them to reinitialize
 * the driver since a reset of the microcontroller
 * did not necessarily restart the device.
 */
bool MCP47X6::begin( uint8_t adr ) {
  _i2caddr = adr;
  
  // read the settings from DAC EEPROM
  // TODO!
  log_debug(F("\n\t\t[MCP47X6] unable to determine DAC type, assuming 8 bits one!"));
  // We assume a default MCP4706
  _resolution = MCP4706_RESOLUTION;

  // chip is initialized
  _initialized = true;

  // write configuration bits to volatile memory
  setConfig();
  
  // default output level is max/4
  _output = (uint8_t)( (pow(2,_resolution) / 4) - 1 );
  
  // set dac output
  setOutputLevel( _output );

  return true;
}



/*
 * Save config bits to volatile memory of DAC
 */
void inline MCP47X6::setConfig( void ) {
  setConfig( _config );
}

void inline MCP47X6::setConfig( uint8_t configBits ) {
  // write config to device only if initialized
  if( _initialized == true ) {
    // write configuration bits to volatile memory
    write8( _i2caddr, ( MCP47X6_CMD_VOLCONFIG | configBits ));    
  }
}



// Special case for 8-bit device (MCP4706) - saves one byte of transfer
// and is therefore faster
uint16_t MCP47X6::setOutputLevel( uint16_t level ) {
  // check argument
  uint16_t _max = pow(2,_resolution)-1;
  if( level > _max ) level = _max;
  
  // set output value
  _output = level;

  log_debug(F("\n[MCP47X6] output level set to "));log_debug(_output,DEC); log_flush();
  
  if( _initialized == true ) {
    // write configuration bits to volatile memory
    write8( _i2caddr, (uint8_t)MCP47X6_CMD_VOLDAC, _output );    
  }
  
  return _output;
}


// Set percent output level
uint8_t MCP47X6::setPercentOutput( uint8_t percent ) {
  
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
