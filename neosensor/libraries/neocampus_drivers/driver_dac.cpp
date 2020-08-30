/**************************************************************************/
/*! 
    @file     driver_dac.cpp
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


#include "driver_dac.h"



/******************************************
 * Default constructor
 */
driver_dac::driver_dac() {
}

/******************************************
 * Power modes
 */
void driver_dac::powerON( void ) {
  // switch ON
}
void driver_dac::powerOFF( void ) {
  // switch OFF
}
