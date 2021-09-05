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



// constructor
oled13inch::oled13inch( void ) : driver_display() {
  _i2caddr      = INVALID_I2CADDR;
  _u8g2         = nullptr;

  _curHours     = (uint8_t)(-1);
  _curMinutes   = (uint8_t)(-1);
}

// destructor
oled13inch::~oled13inch( void ) {
  if( _u8g2 != nullptr ) {
    free( _u8g2 );
    _u8g2 = nullptr;
  }
}


/**************************************************************************/
/*! 
    @brief  Setups the HW
*/
/**************************************************************************/
boolean oled13inch::begin( uint8_t addr=INVALID_I2CADDR ) {
  // get i2caddr
  if( (addr < (uint8_t)(I2C_ADDR_START)) or (addr > (uint8_t)(I2C_ADDR_STOP)) ) return false;
  _i2caddr = addr;

  // check device identity
  if( !_check_identity(_i2caddr) ) return false;

  /* instantiate u8 device
   * _F_ full frame buffer
   * _1_ or _2_ single or dual frame
   */
  _u8g2 = new U8G2_SH1106_128X64_NONAME_1_HW_I2C(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
  if( _u8g2==nullptr ) return false;

  /* set config:
   * - brightness 100%
   */
  powerON();
  setPercentBrightness( 100 );

  // define defaults parameters

  // call parent class:begin()
  return driver_display::begin( addr );
}


/*
 * Power modes: ON or powerOFF
 */
void oled13inch::powerON( void ) {
  // init display
  _u8g2->begin();
}

void oled13inch::powerOFF( void ) {
  // clear buffer & display
  _u8g2->clear();
  // enter power save mode
  _u8g2->setPowerSave( true );
}


/**************************************************************************/
/*! 
    @brief  display internal processing
*/
/**************************************************************************/
void oled13inch::process( uint16_t coolDown )
{
  if( _u8g2==nullptr ) return;

  /* 1st call FSM management @ driver_display
   * It will update the current status of our display
   * call process() from prent class */
  driver_display::process( coolDown );


  /*
   * add local processing to our current display here
   */

  // e.g central dot blinking or logo animation

}


/*
 * Brightness control of oled display
 */
uint8_t oled13inch::setPercentBrightness( uint8_t percent ) {

  uint8_t _contrast = ( percent<OLED13INCH_BRIGHTNESS_PERCENT_THRESHOLD ? OLED13INCH_BRIGHTNESS_LOW : OLED13INCH_BRIGHTNESS_HIGH );
  _u8g2->setContrast( _contrast );
  return ( _contrast==OLED13INCH_BRIGHTNESS_HIGH ? 100 : (_contrast*100)/OLED13INCH_BRIGHTNESS_HIGH);
}


#if 0
  u8g2.begin();  
  
  u8g2.setFont(u8g2_font_inb30_mr);	// set the target font to calculate the pixel width
  width = u8g2.getUTF8Width(text);		// calculate the pixel width of the text
  
  u8g2.setFontMode(0);		// non transparent mode
}


void loop(void) {
  u8g2_uint_t x;
  
  u8g2.firstPage();
  do {
  
    // draw the scrolling text at current offset
    x = offset;
    u8g2.setFont(u8g2_font_inb30_mr);		// set the target font
    do {								// repeated drawing of the scrolling text...
      u8g2.drawUTF8(x, 30, text);			// draw the scolling text
      x += width;						// add the pixel width of the scrolling text
    } while( x < u8g2.getDisplayWidth() );		// draw again until the complete display is filled
    
    u8g2.setFont(u8g2_font_inb16_mr);		// draw the current pixel width
    u8g2.setCursor(0, 58);
    u8g2.print(width);					// this value must be lesser than 128 unless U8G2_16BIT is set
    
  } while ( u8g2.nextPage() );
  
  offset-=1;							// scroll by one pixel
  if ( (u8g2_uint_t)offset < (u8g2_uint_t)-width )	
    offset = 0;							// start over again
    
  delay(10);							// do some small delay
}
#endif /* 0 */


/*
 * Display LOGO
 */
bool oled13inch::dispLogo( void ) {
  if( _u8g2==nullptr ) return false;

  // FSMstate check
  if( !isFSMstatus(displayState_t::logo) ) return false;

  const char _str[]="neOCampus";

  // compute display offsets
  _u8g2->setFont(u8g2_font_inb30_mr);	// set the target font to calculate the pixel width
  uint8_t str_width = _u8g2->getUTF8Width(_str);  // calculate the pixel width of the text
  uint8_t screen_width = _u8g2->getDisplayWidth();
  uint8_t x_offset = ( str_width>=screen_width ? 0 : (screen_width-str_width)/2 );

  uint8_t str_height = _u8g2->getMaxCharHeight(); // calculate the max pixel height of the text
  uint8_t screen_height = _u8g2->getDisplayHeight();
  uint8_t y_offset = ( str_height>=screen_height ? str_height : (screen_height-str_height)/2 );

  _u8g2->setFontMode(0);		// non transparent mode

  // display logo
  _u8g2->firstPage();
  do {
    _u8g2->drawUTF8(x_offset, y_offset, _str);
  } while ( _u8g2->nextPage() );

  // finish :)
  return true;
}


/*
 * Display TIME
 * Note: Beware that this method may get called at any time ...
 */
uint8_t oled13inch::dispTime( uint8_t hours, uint8_t minutes, uint8_t seconds ) {
  if( _u8g2==nullptr ) return false;

  _hours = ( hours!=(uint8_t)(-1) ? hours : _hours );
  _minutes = ( minutes!=(uint8_t)(-1) ? minutes : _minutes );

  // FSMstate check
  if( !isFSMstatus(displayState_t::time) ) return false;

  char _str[6];
  snprintf( _str, sizeof(_str), "%2d:%02d", _hours, _minutes );

  // compute display offsets
  _u8g2->setFont(u8g2_font_inb30_mr);	// set the target font to calculate the pixel width
  uint8_t str_width = _u8g2->getUTF8Width(_str);		// calculate the pixel width of the text
  uint8_t screen_width = _u8g2->getDisplayWidth();
  uint8_t x_offset = ( str_width>=screen_width ? 0 : (screen_width-str_width)/2 );

  uint8_t str_height = _u8g2->getMaxCharHeight(); // calculate the max pixel height of the text
  uint8_t screen_height = _u8g2->getDisplayHeight();
  uint8_t y_offset = ( str_height>=screen_height ? str_height : (screen_height-str_height)/2 );

  _u8g2->setFontMode(0);		// non transparent mode

  // display logo
  _u8g2->firstPage();
  do {
    _u8g2->drawUTF8(x_offset, y_offset, _str);
  } while ( _u8g2->nextPage() );

  // finish :)
  return true;
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
  //log_debug(F("\n[oled13inch] status register value read: 0x")); log_debug(cur_status,HEX); log_flush();

  // change display mode (i.e ON/OFF or OFF/ON)
  write8(a, OLED13INCH_CMD_REG, (cur_status & 0x40 ? OLED13INCH_DISPLAY_ON : OLED13INCH_DISPLAY_OFF) );
  yield();
  uint8_t new_status = read8(a, OLED13INCH_STATUS_REG);
  //log_debug(F("\n[oled13inch] status register value read: 0x")); log_debug(new_status,HEX); log_flush();
  
  if( ((cur_status&0x40) ^ (new_status&0x40)) != 0x40 ) return false;

  return true;
}

