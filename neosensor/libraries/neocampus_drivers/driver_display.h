/**************************************************************************/
/*! 
    @file     driver_display.h
    @author   F. Thiebolt
	  @license
	
    This is part of a the neOCampus drivers library.
    Implement a generic display to leverage the needs that stems from
    multiple types of displays availability.
    
    (c) Copyright 2018 Thiebolt F. <thiebolt@irit.fr>

	@section  HISTORY

    2018-Jun    - First release, F. Thiebolt
    
*/
/**************************************************************************/

#ifndef _DRIVER_DISPLAY_H_
#define _DRIVER_DISPLAY_H_


#include <Arduino.h>



/*
 * Definitions
 */
#define DRIVER_DISPLAY_ANIMMODE_DISABLED    (uint8_t)(-1)


/*
 * Class
 */
class driver_display {
  public:
    driver_display( void );
    
    // Power Modes
    virtual void powerON( void );       // switch ON
    virtual void powerOFF( void );      // switch OFF
    
    // Brightness & others
    virtual uint8_t setPercentBrightness( uint8_t );  // 0 -> 100% luminosity
    virtual bool setDotsBlinking( bool );             // central dots blinking or not
    
    // Display methods to get overridden in child classes
    virtual uint8_t dispMsg( const char * );   // display a text message
    virtual uint8_t dispTime( uint8_t hours, uint8_t minutes, uint8_t seconds=0 ); // display time
    
    // Identity (e.g i2c addr)
    virtual String subID( void )=0;    // pure virtual, retrieve subID (e.g i2c addr)

    // animation
    virtual bool animate( bool activate=true, uint8_t mode=1 );

  // --- protected methods / attributes ---------------------
  // --- i.e subclass have direct access to
  protected:
};

#endif /* _DRIVER_DISPLAY_H_ */
