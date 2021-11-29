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

#ifndef _OLED13INCH_H_
#define _OLED13INCH_H_

/*
 * Includes
 */
#include <Arduino.h>
#include <U8g2lib.h>

// generic display driver
#include "driver_display.h"


/*
 * Definitions
 */
#define OLED13INCH_STATUS_REG       0x00  // (READ) status register

#define OLED13INCH_CMD_REG          0x00  // Single shoot command
#define OLED13INCH_MULTICMD_REG     0x80  // multiple commands in one i2c transaction

#define OLED13INCH_DISPLAY_OFF      0xAE  // (WRITE) set display OFF
#define OLED13INCH_DISPLAY_ON       0xAF  // (WRITE) set display ON

/* Brightness / Contrast
 * Oled displays have very poor brightness management hence we'll consider a
 *  threshold to switch between LOW or HIGH
 */
#define OLED13INCH_BRIGHTNESS_PERCENT_THRESHOLD   30    // < 30% ==> LOW, >= 30% ==> HIGH
#define OLED13INCH_BRIGHTNESS_HIGH                255   // oled API is uint8_t
#define OLED13INCH_BRIGHTNESS_LOW                 20



/*
 * Class
 */
class oled13inch : public driver_display {

  public:
    // constructor
    oled13inch( void );
    
    // destructor
    ~oled13inch( void );

    boolean begin( uint8_t );             // start with an i2c address
    // boolean begin( JsonVariant );   // sensor creation via sensOCampus JSON array 'params'
    void powerON( void );                 // switch ON
    void powerOFF( void );                // switch OFF
    void process( uint16_t coolDown=0 );  // internal processing

    String subID( uint8_t=0 ) { return String(_i2caddr); };

    // Brightness & others
    uint8_t setPercentBrightness( uint8_t );

    // Various display modes
    bool    dispLogo( void );  // display our logo
    uint8_t dispTime( uint8_t hours, uint8_t minutes, uint8_t seconds=0 );  // display time

    // --- static methods / constants -----------------------
    
    // list of possibles I2C addrs
    static const uint8_t i2c_addrs[];

    // device detection
    static boolean is_device( uint8_t );

  protected:
    // attributes
    uint8_t _i2caddr;

    U8G2_SH1106_128X64_NONAME_F_HW_I2C *_u8g2;  // full buffer mode (_F_) or single page (_1_)

    // time related
    uint8_t _curHours, _curMinutes;

    // methods ...
    const char* _getUnits( const char*, JsonObject ); // retrieve units for a key sensor in a Json dictionnary (JsonObject)
    static bool _check_identity( uint8_t );   // check device is what we expect!
};

#endif // _OLED13INCH_H_

