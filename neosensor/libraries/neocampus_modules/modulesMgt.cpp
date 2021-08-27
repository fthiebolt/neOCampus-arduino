/*
 * neOCampus operation
 * 
 * Modules management class for high-level modules management
 *
 * 
 * F.Thiebolt aug.21  added support for shared JSON
 * Thiebolt F. Nov.19   cancel modules startALL if need2reboot flag is active
 * Thiebolt F. June 18
 * 
 */


/*
 * Includes
 */
#include "neocampus_debug.h"

#include "modulesMgt.h"


/*
 * Global shared variables/functions
 */
extern bool _need2reboot;


/*
 * Definitions
 */



// constructor
modulesMgt::modulesMgt( void ) {
  
  // clean modules list
  _clearList();
}

// destructor
modulesMgt::~modulesMgt( void ) {
  // nothing todo as we do not allocate anything on our own
}


/*
 * add a module to the list
 */
bool modulesMgt::add( base *mod ) {
  if( mod == nullptr ) return false;

  // try to find a free slot
  for( uint8_t i=0; i < sizeof(modulesList)/sizeof(modulesList[0]); i++ ) {
    if( not modulesList[i] ) {
      modulesList[i] = mod;
      return true;
    }
  }
  log_error(F("\n[modulesMgt] unable to add a module to the list :|")); log_flush();
  return false;
}

/*
 * Count number of modules
 */
uint8_t modulesMgt::count( void ) {
  uint8_t nb = 0;
  for( uint8_t i=0; i < sizeof(modulesList)/sizeof(modulesList[0]); i++ ) {
    nb = ( modulesList[i] != nullptr ? nb+1 : nb );
  }
  return nb;
}

/*
 * Start ALL modules
 */
bool modulesMgt::startAll( senso *sensocampus, JsonDocument &sharedRoot ) {

  if( _need2reboot or not sensocampus ) return false;
  
  bool _ret = true;
  
  for( uint8_t i=0; i < sizeof(modulesList)/sizeof(modulesList[0]); i++ ) {
    
    // [nov.19] cancel modules startup if _need2reboot is active
    if( _need2reboot ) {
      _ret = false;
      break;
    }
    
    if( modulesList[i] ) {
      if( not modulesList[i]->start(sensocampus,sharedRoot) ) _ret=false;
    }
    yield();
  }
  return _ret;
}


/*
 * stop ALL modules
 */
bool modulesMgt::stopAll( void ) {
  bool _ret = false;

  for( uint8_t i=0; i < sizeof(modulesList)/sizeof(modulesList[0]); i++ ) {
    if(  modulesList[i] ) {
      // at least a module is active
      if( not _ret ) {
        _ret = true;
        log_info(F("\n[modulesMgt] STOPPING all modules ..."));
      }
      modulesList[i]->stop();
    }
    yield();
  }

  return _ret;
}

/*
 * process module (MQTT publish & subscribe)
 */
bool modulesMgt::processAll( void ) {
  if( _need2reboot ) return false;
  
  bool _ret = true;
  
  // parse all modules and process each of them
  for( uint8_t i=0; i < sizeof(modulesList)/sizeof(modulesList[0]); i++ ) {
    if(  modulesList[i] ) {
      _ret = ( modulesList[i]->process() ? _ret : false );
    }
    yield();
  }
  
  return _ret;
}



/* ------------------------------------------------------------------------------
 * Private methods 
 */

/*
 * clear modules list
 */
void modulesMgt::_clearList( void ) {
  for( uint8_t i=0; i < sizeof(modulesList)/sizeof(modulesList[0]); i++ ) {
    modulesList[i] = nullptr;
  }
}

