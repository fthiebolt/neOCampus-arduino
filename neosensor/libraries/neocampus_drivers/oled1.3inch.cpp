/**************************************************************************/
/*! 
    @file     oled1.3inch.h
    @author   F.Thiebolt
    @section  LICENSE
	  @license
	
    This is part of a the neOCampus drivers library.

    @note
    1.3 inches oleds --> 128x64 SH1106 based chips
    0.96 inche oleds --> 128x64 SSD1306 based
        
    (c) Copyright 2020 Thiebolt.F <thiebolt@irit.fr> for neOCampus

	  @section  HISTORY

    2021-Sep  - F.Thiebolt    considering 1.3 inches oleds based on SH1106
    2020-Nov  - F.Thiebolt    Initial Release
*/
/**************************************************************************/


#include <Arduino.h>

#include <U8x8lib.h>
//U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);   // OLEDs without Reset of the Display

#include "neocampus.h"
#include "neocampus_debug.h"
#include "neocampus_i2c.h"

#include "oled1.3inch.h"



/**************************************************************************/
/*! 
    @brief  Declare list of possible I2C addrs
*/
/**************************************************************************/
const uint8_t oled13inch::i2c_addrs[2] = { 0x3c, 0x3d };


/**************************************************************************/
/*! 
    @brief  Test if device at 'adr' is really our 1.3" OLED display
*/
/**************************************************************************/
boolean oled13inch::is_device( uint8_t a ) {
  
  // First step, parse all addresses
  boolean found = false;
  for( uint8_t i=0; i < sizeof(oled13inch::i2c_addrs); i++ ) {
    if( oled13inch::i2c_addrs[i] == a) {
      found = true;
      break;
    }
  }
  if( found == false ) return false;
  
  // check device identity
  if( !_check_identity(a) ) return false;

  // ... okay, we're probably our 1.3" oled :D
  return true;
}



/**************************************************************************/
/*! 
    @brief  Instantiates a new oled13inch class
*/
/**************************************************************************/
oled13inch::oled13inch( void ) : driver_display() {
  _i2caddr = INVALID_I2CADDR;
}

/**************************************************************************/
/*! 
    @brief  Setups the HW
*/
/**************************************************************************/
boolean oled13inch::begin( uint8_t addr=INVALID_I2CADDR) {
  // get i2caddr
  if( (addr < (uint8_t)(I2C_ADDR_START)) or (addr > (uint8_t)(I2C_ADDR_STOP)) ) return false;
  _i2caddr = addr;

  // check device identity
  if( !_check_identity(_i2caddr) ) return false;

  // instantiate u8 device

  /* set config:
   */

  // define defaults parameters

  return true;
}


/*
 * Power modes: ON or powerOFF
 */
void oled13inch::powerOFF( void ) {
}

void oled13inch::powerON( void ) {
}



/* ------------------------------------------------------------------------------
 * Private methods 
 */

/*
 * Check that device identity is what we expect!
 */
bool oled13inch::_check_identity( uint8_t a ) {

  /* Detection protocol:
   * - read status register
   * - change display ON/OFF mode according to its previsouly read status
   * - re-read status register
   * ==> ONLY status bit ought to have changed !
   */

  // Read status register ...
  uint8_t cur_status = read8(a, OLED13INCH_STATUS_REG);
  log_debug(F("\n[oled13inch] status register value read: 0x")); log_debug(cur_status,HEX); log_flush();

  // change display mode (i.e ON/OFF or OFF/ON)
  write8(a, OLED13INCH_CMD_REG, (cur_status & 0x40 ? OLED13INCH_DISPLAY_ON : OLED13INCH_DISPLAY_OFF) );
  yield();
  uint8_t new_status = read8(a, OLED13INCH_STATUS_REG);
  log_debug(F("\n[oled13inch] status register value read: 0x")); log_debug(new_status,HEX); log_flush();
  
  if( (cur_status ^ new_status) != 0x40 ) return false;

  return true;
}

