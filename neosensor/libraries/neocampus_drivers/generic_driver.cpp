/**************************************************************************/
/*! 
    @file     generic_driver.cpp
    @author   F. Thiebolt
	  @license

    This is part of a the neOCampus drivers library.
    Sensors generic driver
    
    (c) Copyright 2020 Thiebolt F. <thiebolt@irit.fr>

	@section  HISTORY

    F.Thiebolt  aug.21  added support for analog data integration
    2020-May    - First release, F. Thiebolt
    
    
*/
/**************************************************************************/


#include "generic_driver.h"



/******************************************
 * Default constructor
 */
generic_driver::generic_driver( uint16_t read_msinterval,
                                uint8_t threshold_cpt,
                                uint8_t threshold_percent ) {

  _readMsInterval   = read_msinterval;
  _thresholdCpt     = threshold_cpt;
  _thresholdPercent = threshold_percent;

  _trigger        = false;
  _currentCpt     = (uint8_t)(-1);
  _lastMsRead     = ULONG_MAX/2;
  _lastMsWrite    = ULONG_MAX/2;
  _lastMsSent     = ULONG_MAX/2;

  value           = -1.0; // fool guard
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
 * 
 */
void generic_driver::process( uint16_t coolDown ) {
  // same time ref for all
  unsigned long _curTime = millis();

  // check wether it's time to process or not
  if( _curTime - _lastMsWrite < ((unsigned long)coolDown)*1000 ) return;
  if( _curTime - _lastMsRead < _readMsInterval ) return;

  // acquire data
  float val;
  if( acquire(&val)==false ) return;   // data was not ready
  _lastMsRead   = _curTime;

  // data has been acquired :)
  if( _currentCpt==(uint8_t)(-1) or
      abs(_current - val) > abs((_current*(float)_thresholdPercent)/100.0) ) {
    // (re)initializing either because it's first time or unstable value
    _current    = val;
    _currentCpt = 0;
    return;
  }

  // value read from sensor is stable :)
  if( ++_currentCpt < _thresholdCpt-1 ) return;

  // new stable value :)
  _currentCpt   = -1;
  value         = _current;
  _lastMsWrite  = _curTime;

  // do we need to sent the new value ?
  if( abs(value - valueSent) > DATA_SENDING_VARIATION_THRESHOLD ) {
    _trigger = true;
    return;
  }

// TODO: add sending if it has not been sent for a long time

  // new official value does not differ so much from the previously sent
  // ... continuing after next cooldown period
}


/******************************************
 * DATA integration related methods
 */
void generic_driver::setDataSent( void ) {
  _trigger = false;
  valueSent = value;
  _lastMsSent = millis();
}
