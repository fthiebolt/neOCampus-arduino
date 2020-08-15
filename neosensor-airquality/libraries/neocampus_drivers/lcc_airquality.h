/**************************************************************************/
/*! 
    @file     lcc_airquality.h
    @author   F. Thiebolt
	  @license
	
    This is part of a the neOCampus drivers library.
    This driver is intended to the CNRS' LCC air quality monitoring board
    
    (c) Copyright 2020 Thiebolt F. <thiebolt@irit.fr>

	@section  HISTORY

    2020-Aug    - First release, F.Thiebolt
    
*/
/**************************************************************************/

#ifndef _LCC_AIRQUALITY_H_
#define _LCC_AIRQUALITY_H_


#include <Arduino.h>
#include <ArduinoJson.h>

#include "neocampus.h"      // for INVALID_I2CADDR

// generic sensor driver
#include "generic_driver.h"



/*
 * Definitions
 */



/*
 * Class
 */
class lcc_airquality : public generic_driver {
  public:
    lcc_airquality( void );
    
    /* Power Modes
     * [aug.20] the board does feature any way to switch power
    void powerON( void );       // switch ON
    void powerOFF( void );      // switch OFF
     */

    // detection JsonConfig
    boolean begin( JsonObject );

    // send back sensor's value, units and subID
    float acquire( void );
    const char *sensorUnits( void );
    // TODO: find a way to send back a string !
    uint8_t subID( void ) { return INVALID_I2CADDR; };
    const char* strSubID( void ) { return nullptr; };



  // --- protected methods / attributes ---------------------
  // --- i.e subclass have direct access to
  protected:

};

#endif /* _LCC_AIRQUALITY_H_ */
