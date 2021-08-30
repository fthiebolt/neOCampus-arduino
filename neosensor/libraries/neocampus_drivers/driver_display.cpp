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

  _trigger        = false;

  _lastMsWrite    = ULONG_MAX/2;
  _lastMsSent     = ULONG_MAX/2;

  value           = -1.0; // fool guard
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
 * Display(s) internal processing
 * 
 */
void driver_display::process( uint16_t coolDown ) {
  // same time ref for all
  unsigned long _curTime = millis();

  // check wether it's time to process or not
  if( _curTime - _lastMsWrite < ((unsigned long)coolDown)*1000 ) return;

#if 0
TO BE CONTINUED

  // acquire data
  float val;
  if( acquire(&val)==false ) return;   // data was not ready
  _lastMsRead   = _curTime;

  // round acquired value
  decimals = ( decimals > _MAX_DATA_DECIMALS ? _MAX_DATA_DECIMALS : decimals );
  if( decimals==0 ) {
    val = (float)round(val);
  }
  else {
    float _pow = pow(10,decimals);
    val = (float)round(val*_pow)/_pow;
  }

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

  /* We need to send the new value either because:
   * - its own value evolved above threshold
   * - we reached the _MAX_COOLDOWN_SENSOR delay regarding our last sending
   */
  if( abs(value - valueSent) > DATA_SENDING_VARIATION_THRESHOLD ||
      (_curTime - _lastMsSent >= ((unsigned long)_MAX_COOLDOWN_SENSOR)*1000) ) {
    _trigger = true;
    return;
  }

  // new official value does not differ so much from the previously sent
  // ... continuing after next cooldown period
#endif /* 0 */
}


/******************************************
 * DATA integration related methods
 */
void driver_display::setDataSent( void ) {
  _trigger = false;
  valueSent = value;
  _lastMsSent = millis();
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
