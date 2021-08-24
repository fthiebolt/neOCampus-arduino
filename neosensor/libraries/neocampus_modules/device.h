/*
 * neOCampus operation
 * 
 * Device module for high-level end-device management
 *
 * 
 * Thiebolt F. July 17
 * 
 */


#ifndef _DEVICE_H_
#define _DEVICE_H_

/*
 * Includes
 */

#include <Arduino.h>

// include base for all modules
#include "base.h"





/*
 * Definitions
 */
#define DEVICE_MIN_FREQUENCY    900       // may go up to every 15 minutes ...
#define DEVICE_MAX_FREQUENCY    _MAX_COOLDOWN_DEVICE        // or cool down to every every 6hours
#define DEFL_DEVICE_FREQUENCY   (DEVICE_MIN_FREQUENCY*2)    // end-device status report every 30mn



/*
 * Class
 */
class device : public base {
  public:
    device();

    // MQTT
    bool start( senso * );
    bool process( void );     // process own module's activities
    
    void handle_msg( JsonObject );

    void status( JsonObject ); 

    // Module's config
    bool saveConfig( void );
    bool loadConfig( void );            // load an eventual module'specific config file
    boolean loadSensoConfig( senso * ); // sensOCampus config to load (if any)

  private:
    
    /*
     * private membre functions
     */
    bool _loadConfig( JsonObject );
    bool _processOrder( const char *, int *, const char * );   // an order to process with optional value

};


#endif /* _DEVICE_H_ */


