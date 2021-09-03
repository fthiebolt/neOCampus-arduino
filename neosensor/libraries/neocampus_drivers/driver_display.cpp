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

#include "neocampus.h"
#include "neocampus_debug.h"

#include "driver_display.h"



/******************************************
 * Default constructor
 */
driver_display::driver_display( void ) {

  // data sending related
  _trigger        = false;
  _lastMsWrite    = ULONG_MAX/2;
  _lastMsSent     = ULONG_MAX/2;
  value           = -1.0; // fool guard

  // current time
  _hours          = (uint8_t)(-1);
  _minutes        = (uint8_t)(-1);

  // FSM related
  _FSMinitialized = false;
}


/******************************************
 * Detection
 */
boolean driver_display::begin( uint8_t adr ) {
  return _begin();
}
boolean driver_display::begin( JsonVariant root ) {
  return _begin();
}

// low-level _begin()
bool driver_display::_begin( void ) {

  // set FSM initial state
  _FSMstatus = DISPLAY_FSMSTATE_DEFL;
  _FSMtimerDelay = 0;
  _FSMinitialized = true;

  return true;
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

  /*
   * FSM related
   */
  if( !_FSMinitialized ) return;

  // process according to our FSM
  switch( _FSMstatus ) {

    // IDLE
    case displayState_t::idle:
      log_debug(F("\n\t[driverDisplay]["));log_debug(subID());log_debug(F("] about to start the display ...")); log_flush();

      // going next step ...
      _FSMstatus = displayState_t::logo;
      _FSMtimerDelay = DISPLAY_LOGO_MS;
      _FSMtimerStart = millis();
      if( dispLogo() ) {
        log_debug(F("\n\t[driverDisplay]["));log_debug(subID());log_debug(F("] display LOGO ...")); log_flush();
      }
      else _FSMtimerDelay = 0;
      //yield();
      //break;

    // LOGO
    case displayState_t::logo:
      // still displaying logo ?
      if( dispLogoBusy() ) break;
      log_debug(F("\n\t[driverDisplay]["));log_debug(subID());log_debug(F("] logo display is now over (or not available) ...")); log_flush();

      // going next step ...
      _FSMstatus = displayState_t::time;
      _FSMtimerDelay = DISPLAY_TIME_MS;
      _FSMtimerStart = millis();
      if( dispTime( _hours, _minutes ) ) {
        log_debug(F("\n\t[driverDisplay]["));log_debug(subID());log_debug(F("] display TIME ...")); log_flush();
      }
      else _FSMtimerDelay = 0;
      //yield();
      //break;

    // TIME
    case displayState_t::time:
      // still displaying time ?
      if( dispTimeBusy() ) break;
      log_debug(F("\n\t[driverDisplay]["));log_debug(subID());log_debug(F("] time display is now over (or not availale) ...")); log_flush();

      // going next step ...
      _FSMstatus = displayState_t::message;
      _FSMtimerDelay = DISPLAY_MSG_MS;
      _FSMtimerStart = millis();
      if( dispMsg(nullptr) ) {
        log_debug(F("\n\t[driverDisplay]["));log_debug(subID());log_debug(F("] display message ...")); log_flush();
      }
      else _FSMtimerDelay = 0;
      //yield();
      //break;

    // MESSAGE
    case displayState_t::message:
      // still displaying message ?
      if( dispMsgBusy() ) break;
      log_debug(F("\n\t[driverDisplay]["));log_debug(subID());log_debug(F("] message display is now over (or not available) ...")); log_flush();

      // going next step ...
      _FSMstatus = displayState_t::weather;
      _FSMtimerDelay = DISPLAY_WEATHER_MS;
      _FSMtimerStart = millis();
      if( dispWeather(nullptr) ) {
        log_debug(F("\n\t[driverDisplay]["));log_debug(subID());log_debug(F("] display weather ...")); log_flush();
      }
      else _FSMtimerDelay = 0;
      //yield();
      //break;

    // LAST
    case displayState_t::last:
      // still displaying weather ?
      if( dispWeatherBusy() ) break;
      log_debug(F("\n\t[driverDisplay]["));log_debug(subID());log_debug(F("] weather display is now over (or not available) ...")); log_flush();

      // let's restart to TIME on next loop()
      _FSMstatus = displayState_t::logo;
      _FSMtimerDelay = 0;
      break;

    // default
    default:
      log_error(F("\n\t[driverDisplay]["));log_debug(subID());log_debug(F("] unknown FSM state ?!?! ... resetting !")); log_flush();
      _begin();
  }

#if 0
  /*
   * DATA SENDING related
   */

  // check wether it's time to process or not
  if( _curTime - _lastMsWrite < ((unsigned long)coolDown)*1000 ) return;

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
// display logo
bool driver_display::dispLogo( void ) {
  return false;
}
// return number of bytes displayed
uint8_t driver_display::dispMsg( const char *msg ) {
  return 0;
}

// return number of bytes displayed
uint8_t driver_display::dispTime( uint8_t hour, uint8_t minute, uint8_t seconds ) {
  return 0;
}

// return number of bytes displayed
uint8_t driver_display::dispWeather( const char *city, float temperature, float hygro, bool sunny, bool rainy, bool windy ) {
  return 0;
}

// check FSM LOGO state still budy ?
bool driver_display::dispLogoBusy( void ) {
  return _FSMstateBusy();
}

// check FSM MSG state still budy ?
bool driver_display::dispMsgBusy( void ) {
  return _FSMstateBusy();
}

// check FSM TIME state still budy ?
bool driver_display::dispTimeBusy( void ) {
  return _FSMstateBusy();
}

// check FSM WEATHER state still budy ?
bool driver_display::dispWeatherBusy( void ) {
  return _FSMstateBusy();
}

// check for matching current FSM status
bool driver_display::isFSMstatus( displayState_t s ) {
  if( !_FSMinitialized or s!=_FSMstatus ) return false;
  return true;
}


// animations
// method to get overriden
bool driver_display::animate( displayAnimate_t mode ) {
  return false;
}


/* ------------------------------------------------
 * PRIVATE methods
 * ------------------------------------------------ */
// FSM status to check against busyness
bool driver_display::_FSMstateBusy( void ) {

  if( _FSMtimerDelay==0 ) return false;

  /* reached the delay ?
   * look at https://arduino.stackexchange.com/questions/33572/arduino-countdown-without-using-delay/33577#33577
   * for an explanation about millis() that wrap around!
   */
  if( (millis() - _FSMtimerStart) >= (unsigned long)_FSMtimerDelay ) {
    // end of current FSM state
    _FSMtimerDelay = 0;
    return false;
  }

  // current FSM state still on way
  return true;
}
