/**************************************************************************/
/*! 
    @file     driver_display.cpp
    @author   F. Thiebolt
    @license

    This is part of a the neOCampus drivers library.
    Implement a generic DAC to leverage the needs that stems from
    multiple DACs types availability.
    
    (c) Copyright 2017 Thiebolt F. <thiebolt@irit.fr>

	  @section  HISTORY

    2018-Jun    - First release, F. Thiebolt
    
    
*/
/**************************************************************************/


#include "driver_display.h"



/******************************************
 * Default constructor
 */
driver_display::driver_display( void ) {
}


/******************************************
 * Detection
 */
boolean driver_display::begin( uint8_t adr ) {
  return false;
}
boolean driver_display::begin( JsonVariant root ) {
  return false;
}


/******************************************
 * Power modes
 */
void driver_display::powerON( void ) {
  // switch ON
}
void driver_display::powerOFF( void ) {
  // switch OFF
}



/******************************************
 * DEPRECATION NOTICE
 * ALL METHODS BELOW WILL BECOME OBSOLETE
 * ONCE DISPLAY MODULE WILL BE AVAILABLE
 ******************************************/

/******************************************
 * Luminosity, contrast etc
 * Methods to get overriden in child classes
 */
// set brightness
uint8_t driver_display::setPercentBrightness( uint8_t val ) {
  // set brightness in percent
  return (uint8_t)-1;
}

// toggle for central dots blinking
bool driver_display::setDotsBlinking( bool val ) {
  return false;
}


/******************************************
 * Display things
 */
// return number of bytes displayed
uint8_t driver_display::dispMsg( const char *msg ) {
  return -1;
}

// return number of bytes displayed
uint8_t driver_display::dispTime( uint8_t hour, uint8_t minute, uint8_t seconds ) {
  return -1;
}

// animations
// method to get overriden
bool driver_display::animate( bool activate, uint8_t mode ) {
  return false;
}
