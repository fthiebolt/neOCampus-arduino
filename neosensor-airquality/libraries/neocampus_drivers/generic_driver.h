/**************************************************************************/
/*! 
    @file     generic_driver.h
    @author   F. Thiebolt
	  @license
	
    This is part of a the neOCampus drivers library.
    Sensors generic driver
    
    (c) Copyright 2020 Thiebolt F. <thiebolt@irit.fr>

	@section  HISTORY

    2020-May    - First release, F. Thiebolt
    
*/
/**************************************************************************/

#ifndef _GENERIC_DRIVER_H_
#define _GENERIC_DRIVER_H_


#include <Arduino.h>
#include <ArduinoJson.h>



/*
 * Definitions
 */



/*
 * Class
 */
class generic_driver {
  public:
    generic_driver( void );
    
    // Power Modes
    virtual void powerON( void );         // switch ON
    virtual void powerOFF( void );        // switch OFF
    
    // Detection
    virtual boolean begin( uint8_t );     // i2c detected
    virtual boolean begin( JsonObject );  // JsonConfig file (if any)

    // Data
    virtual float acquire( void )=0;    // pure virtual, acquire sensor value
    virtual const char *sensorUnits( void )=0;  // pure virtual, retrieve units of actual sensors (e.g celsius, %r.H, lux ...)

    // Identity (i.e i2c addr)
    virtual uint8_t subID( void )=0;    // pure virtual, retrieve subID (i.e i2c addr)

  // --- protected methods / attributes ---------------------
  // --- i.e subclass have direct access to
  protected:
};

#endif /* _GENERIC_DRIVER_H_ */
