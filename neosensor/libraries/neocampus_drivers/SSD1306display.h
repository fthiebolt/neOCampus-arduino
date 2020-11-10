/**************************************************************************/
/*! 
    @file     SSD1306display.h
    @author   F.Thiebolt
    @section LICENSE
	  @license
	
    This is part of a the neOCampus drivers library.
    SSD1306 is a COG driver for 128 x 64 oled displays
        
    (c) Copyright 2020 Thiebolt.F <thiebolt@irit.fr>

	@section  HISTORY

    2020-Nov  - F.Thiebolt    Initial Release
*/
/**************************************************************************/

#ifndef _SSD1306DISPLAY_H_
#define _SSD1306DISPLAY_H_

/*
 * Includes
 */
#include <Arduino.h>

// generic display driver
#include "driver_display.h"


/*
#include <inttypes.h>
#include <Ticker.h>
*/

#if 0

TO BE CONTINUED

/*
 * Class
 */
class SSD1306display : public driver_display {

  public:
    //! Initialize a TM1637Display object, setting the clock and
    //! data pins.
    //!
    //! @param pinClk - The number of the digital pin connected to the clock pin of the module
    //! @param pinDIO - The number of the digital pin connected to the DIO pin of the module
    SSD1306display( uint8_t pinClk, uint8_t pinDIO, uint16_t usBitDelay=25 );

    //! Sets the brightness of the display.
    //!
    //! The setting takes effect when a command is given to change the data being
    //! displayed.
    //!
    //! @param brightness A number from 0 (lowes brightness) to 7 (highest brightness)
    //! @param on Turn display on or off
    void setBrightness(uint8_t brightness, bool on = true);

    //! Display arbitrary data on the module
    //!
    //! This function receives raw segment values as input and displays them. The segment data
    //! is given as a byte array, each byte corresponding to a single digit. Within each byte,
    //! bit 0 is segment A, bit 1 is segment B etc.
    //! The function may either set the entire display or any desirable part on its own. The first
    //! digit is given by the @ref pos argument with 0 being the leftmost digit. The @ref length
    //! argument is the number of digits to be set. Other digits are not affected.
    //!
    //! @param segments An array of size @ref length containing the raw segment values
    //! @param length The number of digits to be modified
    //! @param pos The position from which to start the modification (0 - leftmost, 3 - rightmost, 4 - central dots)
    void setSegments(const uint8_t segments[], uint8_t length = 4, uint8_t pos = 0);

    //! Displayes a decimal number
    //!
    //! Dispalyes the given argument as a decimal number
    //!
    //! @param num The number to be shown
    //! @param leading_zero When true, leading zeros are displayed. Otherwise unnecessary digits are
    //!        blank
    //! @param length The number of digits to set. The user must ensure that the number to be shown
    //!        fits to the number of digits requested (for example, if two digits are to be displayed,
    //!        the number must be between 0 to 99)
    //! @param pos The position most significant digit (0 - leftmost, 3 - rightmost)
    void showNumberDec(int num, bool leading_zero = false, uint8_t length = 4, uint8_t pos = 0);

    //! Displayes a decimal number, with dot control
    //!
    //! Dispalyes the given argument as a decimal number. The dots between the digits (or colon)
    //! can be individually controlled
    //!
    //! @param num The number to be shown
    //! @param dots Dot enable. The argument is a bitmask, with each bit corresponding to a dot
    //!        between the digits (or colon mark, as implemented by each module). The MSB is the 
    //!        leftmost dot of the digit being update. For example, if pos is 1, the MSB of dots
    //!        will correspond the dot near digit no. 2 from the left. Dots are updated only on
    //!        those digits actually being update (that is, no more than len digits)
    //! @param leading_zero When true, leading zeros are displayed. Otherwise unnecessary digits are
    //!        blank
    //! @param length The number of digits to set. The user must ensure that the number to be shown
    //!        fits to the number of digits requested (for example, if two digits are to be displayed,
    //!        the number must be between 0 to 99)
    //! @param pos The position least significant digit (0 - leftmost, 3 - rightmost)
    void showNumberDecEx(int num, uint8_t dots = 0, bool leading_zero = false, uint8_t length = 4, uint8_t pos = 0);

    //! Translate a single digit into 7 segment code
    //!
    //! The method accepts a number between 0 - 15 and converts it to the
    //! code required to display the number on a 7 segment display.
    //! Numbers between 10-15 are converted to hexadecimal digits (A-F)
    //!
    //! @param digit A number between 0 to 15
    //! @return A code representing the 7 segment image of the digit (LSB - segment A;
    //!         bit 6 - segment G; bit 7 - always zero)
    uint8_t encodeDigit(uint8_t digit);

    
    /* ------------------------------------------
     * Enable dots blinking every 1s 
     */
    bool setDotsBlinking( bool );
    
    // Every half-second timer call to this method for central dots blinking
    static void ICACHE_RAM_ATTR timerHandler( TM1637Display * );


    /* ------------------------------------------
    * Driver display overloads
    */
    void powerON( void );       // switch ON
    void powerOFF( void );      // switch OFF
    
    uint8_t setPercentBrightness( uint8_t );  // 0 -> 100% luminosity
      
    uint8_t dispMsg( const char * );    // display a text message
    uint8_t dispTime( uint8_t hours, uint8_t minutes, uint8_t seconds=0 ); // display time
      
    String subID(void) { return String(""); }             // retrieve identity (no subID available)

    bool animate( bool activate=true, uint8_t mode=1 );   // display some animations :)

  protected:
    void bitDelay();

    void start();

    void stop();

    bool writeByte(uint8_t b);
    
    // process animations
    uint8_t _processAnim( void );

    /*
     * protected attributes
     */
    static const uint8_t digitToSegment[];
    static const uint8_t dispOFF[];
    static const uint8_t animMode1[];
    
  private:
    
    uint8_t m_pinClk;
    uint8_t m_pinDIO;
    uint8_t m_brightness;
    
    uint16_t _usBitDelay;
    
    // power status
    bool _powerON;
    
    // dots blinking & others animations
    bool _enabledDotsBlinking;  // enable / disable flag
    Ticker _blinkTimer;         // timer to manage dots blinking
    bool _dots;                 // dots status (on/off)
    uint8_t _animMode;

    // currently displayed time
    uint8_t _curHour;
    uint8_t _curMin;
    
    // --- private methods
    uint8_t _computePercentBrightness( uint8_t );
  
};

#endif /* 0 */

#endif // _SSD1306DISPLAY_H_

