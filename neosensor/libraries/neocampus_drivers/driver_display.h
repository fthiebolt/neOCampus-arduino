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
#include <ArduinoJson.h>
#include <limits.h>



/*
 * Definitions
 */
#define DRIVER_DISPLAY_ANIMMODE_DISABLED    (uint8_t)(-1)


/*
 * Class
 */
class driver_display {
  public:
    // constructor
    driver_display( void );
    
    // destructor
    //~driver_display( void );
  
    // Power Modes
    virtual void powerON( void );       // switch ON
    virtual void powerOFF( void );      // switch OFF

    // Detection
    virtual boolean begin( uint8_t );       // i2c detected
    virtual boolean begin( JsonVariant );   // Json senso config

    // Data
    virtual void process( uint16_t coolDown=0 );    // actuators internal processing

    //virtual boolean acquire( float* )=0;          // pure virtual, acquire sensor value
    //virtual const char *sensorUnits( void )=0;    // pure virtual, retrieve units of actual sensors (e.g celsius, %r.H, lux ...)
  
    // Identity (e.g i2c addr)
    virtual String subID( void )=0;    // pure virtual, retrieve subID (e.g i2c addr)

    // data integration
    inline bool getTrigger( void ) { return _trigger; };  // local driver trigger that indicates a new official value needs to get sent
    inline float getValue( void ) { return value; };      // get official value that has gone through the whole integration process
    void setDataSent( void );                             // data has been sent, reset the 'new official data' trigger


    // --- DEPRECATED methods / attributes ---------------------
    // Brightness & others
[[deprecated("will get obsolted soon by the new display module")]]
    virtual uint8_t setPercentBrightness( uint8_t );  // 0 -> 100% luminosity
[[deprecated("will get obsolted soon by the new display module")]]
    virtual bool setDotsBlinking( bool );             // central dots blinking or not
    
    // Display methods to get overridden in child classes
[[deprecated("will get obsolted soon by the new display module")]]
    virtual uint8_t dispMsg( const char * );   // display a text message
[[deprecated("will get obsolted soon by the new display module")]]
    virtual uint8_t dispTime( uint8_t hours, uint8_t minutes, uint8_t seconds=0 ); // display time

    // animation
[[deprecated("will get obsolted soon by the new display module")]]
    virtual bool animate( bool activate=true, uint8_t mode=1 );

  // --- protected methods / attributes ---------------------
  // --- i.e subclass have direct access to
  protected:
    bool            _trigger;       // true means that the corresponding driver needs to send something

    float           value;          // official value
    unsigned long   _lastMsWrite;   // (ms) last time official value has been written

    float           valueSent;      // official value that has been sent
    unsigned long   _lastMsSent;    // (ms) time something has been sent
};

#endif /* _DRIVER_DISPLAY_H_ */
