/**************************************************************************/
/*! 
    @file     driver_dac.h
    @author   F. Thiebolt
	  @license
	
    This is part of a the neOCampus drivers library.
    Implement a generic DAC to leverage the needs that stems from
    multiple DACs types availability.
    
    (c) Copyright 2017 Thiebolt F. <thiebolt@irit.fr>

	@section  HISTORY

    2017-Oct    - First release, F. Thiebolt
    
*/
/**************************************************************************/

#ifndef _DRIVER_DAC_H_
#define _DRIVER_DAC_H_


#include <Arduino.h>



/*
 * Definitions
 */



/*
 * Class
 */
class driver_dac {
  public:
    driver_dac( void );
    
    // Power Modes
    virtual void powerON( void );     // switch ON
    virtual void powerOFF( void );    // switch OFF
    
    // Set DAC output [0..100%]
    virtual uint8_t setPercentOutput( uint8_t )=0;    // pure virtual, DAC output between 0 and 100%   
    
    // MQTT retrieve subID (i.e i2c addr)
    virtual String subID( void )=0;                   // pure virtual

  // --- protected methods / attributes ---------------------
  // --- i.e subclass have direct access to
  protected:
};

#endif /* _DRIVER_DAC_H_ */
