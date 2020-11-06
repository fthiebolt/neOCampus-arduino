/**************************************************************************/
/*! 
    @file     Adafruit_MCP9808.cpp
    @author   K.Townsend (Adafruit Industries)
	@license  BSD (see license.txt)
	
	I2C Driver for Microchip's MCP9808 I2C Temp sensor

	This is a library for the Adafruit MCP9808 breakout
	----> http://www.adafruit.com/products/1782
		
	Adafruit invests time and resources providing this open source code, 
	please support Adafruit and open-source hardware by purchasing 
	products from Adafruit!

	@section  HISTORY

    dec.18  F.Thiebolt  added I2C addr range for 0x48->4F
                        adapted for neOCampus
    v1.0 - First release
*/
/**************************************************************************/


#include <Arduino.h>

#include "neocampus.h"
#include "neocampus_debug.h"
#include "neocampus_i2c.h"

#include "Adafruit_MCP9808.h"



/**************************************************************************/
/*! 
    @brief  Declare list of possible I2C addrs
    Note: two MCP9808 i2c addr ranges exists: 0x18->1F and 0x48->4f
*/
/**************************************************************************/
const uint8_t Adafruit_MCP9808::i2c_addrs[16] = { 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, \
                                                    0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f };

/* declare kind of units */
const char *Adafruit_MCP9808::units = "celsius";

/**************************************************************************/
/*! 
    @brief  Test if device at 'adr' is really a MCP9808
*/
/**************************************************************************/
boolean Adafruit_MCP9808::is_device( uint8_t a ) {
  
  // First step, parse all addresses
  boolean found = false;
  for( uint8_t i=0; i < sizeof(Adafruit_MCP9808::i2c_addrs); i++ ) {
    if( Adafruit_MCP9808::i2c_addrs[i] == a) {
      found = true;
      break;
    }
  }
  if( found == false ) return false;
  
  // check device identity
  if( !_check_identity(a) ) return false;

  // ... okay, we're probably a MCP9808 ;)
  return true;
}

/**************************************************************************/
/*! 
    @brief  Instantiates a new MCP9808 class
*/
/**************************************************************************/
Adafruit_MCP9808::Adafruit_MCP9808( void ) : generic_driver() {
  _i2caddr = INVALID_I2CADDR;
  _resolution = MCP9808_DEFL_RESOLUTION;
}

/**************************************************************************/
/*! 
    @brief  Setups the HW
*/
/**************************************************************************/
boolean Adafruit_MCP9808::begin( uint8_t addr=INVALID_I2CADDR) {
  // get i2caddr
  if( (addr < (uint8_t)(I2C_ADDR_START)) or (addr > (uint8_t)(I2C_ADDR_STOP)) ) return false;
  _i2caddr = addr;

  // Wire.begin();  // no only one time operation at higher level ...

  // check device identity
  if( !_check_identity(_i2caddr) ) return false;

  /* set config:
   * - disable all alerts, hysteresis, critical threshold and so on
   * - set continuous conversions
   */
  write16(_i2caddr, MCP9808_REG_CONFIG, 0x0);

  // define defaults parameters
  setResolution(_resolution);

  return true;
}


/*
 * Power modes: ON or powerOFF
 */
void Adafruit_MCP9808::powerOFF( void )
{
  _shutdown_wake(1);
}

void Adafruit_MCP9808::powerON( void )
{
  _shutdown_wake(0);
}


/**************************************************************************/
/*! 
    @brief  Setups the HW
*/
/**************************************************************************/
bool Adafruit_MCP9808::setResolution( mcp9808Resolution_t res ) {
  // set resolution and timing
  if( res == MCP9808_RESOLUTION_05DEG ) {
    _integrationTime = (uint8_t)(MCP9808_INTEGRATIONTIME_05);
    _resolution = res;
  }
  else if( res == MCP9808_RESOLUTION_025DEG ) {
    _integrationTime = (uint8_t)(MCP9808_INTEGRATIONTIME_025);
    _resolution = res;
  }
  else if( res == MCP9808_RESOLUTION_0125DEG ) {
    _integrationTime = (uint8_t)(MCP9808_INTEGRATIONTIME_0125);
    _resolution = res;
  }
  else if( res == MCP9808_RESOLUTION_00625DEG ) {
    _integrationTime = (uint8_t)(MCP9808_INTEGRATIONTIME_00625);
    _resolution = res;
  }
  else {
    return false;
  }

  // set resolution to chip
  write8(_i2caddr,MCP9808_REG_RESOLUTION,(uint8_t)res);

  // add some constant to integration time ...
  _integrationTime+=(uint8_t)MCP9808_INTEGRATION_TIME_CTE;

  // finish :)
  return true;
}

/**************************************************************************/
/*! 
    @brief  Reads the 16-bit temperature register and returns the Centigrade
            temperature as a float.

*/
/**************************************************************************/
boolean Adafruit_MCP9808::acquire( float *pval )
{
  if( !pval ) return false;

  // read configuration register ...
  uint16_t conf_register = read16(_i2caddr, MCP9808_REG_CONFIG);
  
  // ... and check if active (i.e continuous conversions active)
  if( conf_register & MCP9808_REG_CONFIG_SHUTDOWN ) {
    powerON();
    delay(_integrationTime);
  }
  
  // Ta conversion
  uint16_t _raw = read16(_i2caddr, MCP9808_REG_AMBIENT_TEMP);

  float Ta = _raw & 0x0FFF;
  Ta /=  16.0;
  if (_raw & 0x1000) Ta -= 256;

  // ... and finally restore previous power state if needed
  if( conf_register & MCP9808_REG_CONFIG_SHUTDOWN ) powerOFF();

  /* [Mar.18] temperature correction for last i2c sensor ... the one
   * supposed to get tied to the main board. */
#ifdef TEMPERATURE_CORRECTION_LASTI2C
  static uint8_t _last_i2c = INVALID_I2CADDR;
  // determine last i2c addr
  if( _last_i2c == INVALID_I2CADDR ) {
    _last_i2c=i2c_addrs[0];
    for(uint8_t i=0; i<sizeof(i2c_addrs)/sizeof(i2c_addrs[0]); i++) {
      // same i2c addr range (e.g 0x1? or 0x4?) ?
      if( (_i2caddr>>4) == (i2c_addrs[i]>>4) ) 
        _last_i2c = ( (i2c_addrs[i]>_last_i2c) ? i2c_addrs[i] : _last_i2c );
    }
  }
  // is last sensor ?
  if ( _i2caddr == _last_i2c ) {
    Ta += TEMPERATURE_CORRECTION_LASTI2C;
    log_debug(F("\n[MCP9808] corrected temperature for i2c=0x"));log_debug(_i2caddr,HEX);log_flush();
  }
#endif

  // ok, send back value
  *pval = Ta;
  return true;
}



/* ------------------------------------------------------------------------------
 * Private methods 
 */

/*
 * Check that device identity is what we expect!
 */
bool Adafruit_MCP9808::_check_identity( uint8_t a ) {

  uint16_t manID = read16(a,MCP9808_REG_MANUF_ID);
  if( manID != (uint16_t)MCP9808_MANUFACTURER_ID ) return false;

  // check for device id
  uint16_t devID = read16(a,MCP9808_REG_DEVICE_ID);
  if( (uint8_t)(devID >> 8) != (uint8_t)MCP9808_DEVICE_ID ) return false;

  return true;
}

//*************************************************************************
// Set Sensor to Shutdown-State or wake up (Conf_Register BIT8)
// 1= shutdown / 0= wake up
//*************************************************************************
void Adafruit_MCP9808::_shutdown_wake( uint8_t sw_ID )
{
    uint16_t conf_shutdown ;
    uint16_t conf_register = read16(_i2caddr, MCP9808_REG_CONFIG);
    if (sw_ID == 1)
    {
      // stop conversions
      conf_shutdown = conf_register | MCP9808_REG_CONFIG_SHUTDOWN ;
      write16(_i2caddr, MCP9808_REG_CONFIG, conf_shutdown);
    }
    if (sw_ID == 0)
    {
      // start continuous conversions
      conf_shutdown = conf_register & ~MCP9808_REG_CONFIG_SHUTDOWN ;
      write16(_i2caddr, MCP9808_REG_CONFIG, conf_shutdown);
    }
}

