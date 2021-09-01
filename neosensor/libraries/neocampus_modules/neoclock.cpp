/*
 * neOCampus operation
 * 
 * Clock module to send time to display
 * 
 * Thiebolt.F jun.18  initial release
 * 
 */


/*
 * Includes
 */
#include "neocampus.h"
#include "neocampus_debug.h"

#include "neoclock.h"


/* 
 * Definitions
 */



// constructor
neoclock::neoclock(): base() 
{
  _freq = -1;   // no need to send anything through MQTT
  _display = nullptr;
  _displayChange = false;
  _secondsLeft = -1;
  _initialized = false;

  // load json config file (if any)
  //loadConfig( );
}


/*
 * add a display
 */
bool neoclock::add_display( uint8_t id ) {
  
  if( _display ) {
    log_error(F("\n[neoclock] error, display already defined!")); log_flush();
    return false;
  }
  
  switch( id ) {
    case NEOCLOCK_TM1637:
      // instantiate a TM1637 display
#if !defined(NEOCLOCK_TM1637_CLK) || !defined(NEOCLOCK_TM1637_DIO)
      log_error(F("\n[neoclock] display id ")); log_error(id,DEC); log_error(F(" not supported!")); log_flush();
      return false;
#else
      _display = new TM1637Display( NEOCLOCK_TM1637_CLK, NEOCLOCK_TM1637_DIO );   // default bitdelay=25µs
      _display->powerOFF();
#endif
      break;
    default:
      log_error(F("\n[neoclock] unknown display id ")); log_error(id,DEC); log_flush();
      return false;
  };

  log_info(F("\n[neoclock] successfully added display_id ")); log_info(id,DEC); log_flush();
}



/*
 * Module startup (MQTT)
 * Note: neOClock module is not involved with MQTT protocol
 */
bool neoclock::start( senso *sensocampus, JsonDocument &sharedRoot ) {

  log_info(F("\n[neoclock] starting module ..."));
  /*
  // create module's JSON structure to hold all of our data
  // [aug.21] we create a dictionnary
  variant = sharedRoot.createNestedObject(MQTT_MODULE_NAME);

  // initialize module's publish & subscribe topics
  snprintf( pubTopic, sizeof(pubTopic), "%s/%s", sensocampus->getBaseTopic(), MQTT_MODULE_NAME);
  snprintf( subTopic, sizeof(subTopic), "%s/%s", pubTopic, "command" );
  */
  
  // stop animes (if any)
  _display->animate( displayAnimate_t::stop );
  
  // switch Power ON full brightness;
  _display->setDotsBlinking( true );
  _display->setPercentBrightness( 100 );
  _display->powerON();
  
  // first time
  _secondsLeft = 0;
  _initialized = true;

  // start timer with associated 1s interrupt handler 
  _timer1s.attach( 1, timerHandler, this );

  // return base::start( sensocampus, sharedRoot );   // not MQTT communications !
  return true;
}



/*
 * Module shutdown (MQTT)
 * Note: neOClock module is not involved with MQTT protocol
 */
bool neoclock::stop( void ) {

  log_info(F("\n[neoclock] module shutdown ..."));

  // detach timer  
  _timer1s.detach();

  if( _display ) {

    // switch Power OFF
    //_display->setPercentBrightness( 0 );
    _display->powerOFF();
    
    // de-allocate memory for _display
    free(_display);
    _display = nullptr;
  }


  /*
   * shutdown MQTT subscription and stop client ...
   */
  
  // return base::stop( );
  return true;
}



/*
 * Module processing
 */
bool neoclock::process( void ) {

  if( !_initialized ) return false;
  
  // do we need to update the HOURS:MINUTES displayed ?
  if( _displayChange ) {

    // display current time
    uint8_t bytes = _display->dispTime( _tm_displayedTime.tm_hour, _tm_displayedTime.tm_min );

    if( !bytes ) {
      log_warning(F("\n[neoclock] no bytes writtent in display ?!?!"));log_flush();
    }
    
    _displayChange = false;

    log_debug(F("\n[neoclock] updated time displayed to "));
    log_debug(_tm_displayedTime.tm_hour,DEC);log_debug(F(":"));log_debug(_tm_displayedTime.tm_min);
    log_debug(F(" with tm_sec= ")); log_debug(_tm_displayedTime.tm_sec,DEC);
    log_flush();
  }

  return true;
}


/*
 * 1s timer handler
 * Note: interrupt handler, no serial debug messages !
 */
void ICACHE_RAM_ATTR neoclock::timerHandler( neoclock *p ) {

  if( p->_secondsLeft-- ) return;

  // timeout reached ...
  time( &(p->_curDisplayedTime) );
  localtime_r( &(p->_curDisplayedTime), &(p->_tm_displayedTime) );   // Weird part ... localtime function *corrects* time to match timezone ... :s

  // restart count-down to next minute
  p->_secondsLeft = 59 - p->_tm_displayedTime.tm_sec;

  // display current time
  p->_displayChange = true;
}



/*
 * Animation modes (i.e demo & others )
 */
bool neoclock::animate( displayAnimate_t mode ) {
  if( _display ) return _display->animate( mode );
  return false;
}
