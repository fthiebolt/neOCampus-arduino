/*
 * neOCampus operation
 * 
 * Modules management class for high-level modules management
 *
 * 
 * Thiebolt F. June 18
 * 
 */


#ifndef _MODULES_MGT_H_
#define _MODULES_MGT_H_

/*
 * Includes
 */

#include <Arduino.h>

#include "neocampus.h"
#include "sensocampus.h"
#include "base.h"       // common ops to all modules





/*
 * Definitions
 */
#ifndef MAX_TCP_CONNECTIONS
#define MAX_ACTIVE_MODULES          5   // as a default, ESP8266 features max 5 TCP concurrent sockets
#else
#define MAX_ACTIVE_MODULES          MAX_TCP_CONNECTIONS
#endif


/*
 * Class
 */
class modulesMgt {
  public:
    modulesMgt( void );

    // methods
    bool add( base * );             // add a module to the list
    uint8_t count( void );          // send back number of registered modules
    bool processAll( void );        // process all modules
    bool startAll( senso * ); // start all modules
    bool stopAll( void );           // stop all modules
    
  private:

    base *modulesList[MAX_ACTIVE_MODULES];
    
    /*
     * private member functions
     */
    void _clearList( void );    // clear modules list (does not freed memory allocated for modules)
};


#endif /* _MODULES_MGT_H_ */


