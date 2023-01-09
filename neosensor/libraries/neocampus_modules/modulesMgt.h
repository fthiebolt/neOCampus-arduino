/*
 * neOCampus operation
 * 
 * Modules management class for high-level modules management
 *
 * F.Thiebolt aug.21  added support for shared JSON
 * Thiebolt F. June 18  initial release
 * 
 */


#ifndef _MODULES_MGT_H_
#define _MODULES_MGT_H_

/*
 * Includes
 */

#include <Arduino.h>
#include <ArduinoJson.h>

#include "neocampus.h"
#include "sensocampus.h"
#include "base.h"       // common ops to all modules





/*
 * Definitions
 */
#define MAX_ACTIVE_MODULES          MAX_TCP_CONNECTIONS


/*
 * Class
 */
class modulesMgt {
  public:
    modulesMgt( void );
    ~modulesMgt( void );

    // methods
    bool add( base * );             // add a module to the list
    uint8_t count( void );          // send back number of registered modules
    bool processAll( void );        // process all modules
    bool startAll( senso *, JsonDocument& );  // start all modules with added shared JSON
    bool stopAll( void );           // stop all modules
    
  private:

    base *modulesList[MAX_ACTIVE_MODULES];
    
    /*
     * private member functions
     */
    void _clearList( void );    // clear modules list (does not freed memory allocated for modules)
};


#endif /* _MODULES_MGT_H_ */


