/**************************************************************************/
/*! 
    @file     generic_driver.cpp
    @author   F. Thiebolt
	  @license

    This is part of a the neOCampus drivers library.
    Sensors generic driver
    
    (c) Copyright 2020 Thiebolt F. <thiebolt@irit.fr>

	@section  HISTORY

    2020-May    - First release, F. Thiebolt
    
    
*/
/**************************************************************************/


#include "generic_driver.h"



/******************************************
 * Default constructor
 */
generic_driver::generic_driver() {
}

/******************************************
 * Detection
 */
boolean generic_driver::begin( uint8_t adr ) {
  return false;
}
boolean generic_driver::begin( JsonVariant root ) {
  return false;
}

/******************************************
 * Power modes
 */
void generic_driver::powerON( void ) {
  // switch ON
}
void generic_driver::powerOFF( void ) {
  // switch OFF
}

/******************************************
 * Sensors internal processing
 * used for continuous integration for example
 */
void generic_driver::process( void ) {
}

