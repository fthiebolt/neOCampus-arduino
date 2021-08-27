/*
 * neOCampus operation
 * 
 * Device module for high-level end-device management
 *
 * 
 * F.Thiebolt aug.21  added JsonDocument to enable global shared JSON
 * Thiebolt F. July 17  initial release
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
// Device's status
enum class deviceStatus_t : uint8_t {
  start       = 0,  // we're in the setup()
  run,              // we're in the loop()
  upgrade,          // firmware upgrade
  reboot,           // on way to reboot
  
  undefined   = (uint8_t)(-1)
};

#define DEVICE_MIN_FREQUENCY    900       // may go up to every 15 minutes ...
#define DEVICE_MAX_FREQUENCY    _MAX_COOLDOWN_DEVICE        // or cool down to every every 6hours
#define DEFL_DEVICE_FREQUENCY   (DEVICE_MIN_FREQUENCY*2)    // end-device status report every 30mn



/*
 * Class
 */
class device : public base {
  public:
    // constructors
    device( void );

    // destructor
    ~device( void );

    // MQTT
    bool start( senso *, JsonDocument& );
    bool process( void );     // process own module's activities
    
    void handle_msg( JsonObject );

    void status( JsonObject ); 

    // Module's config
    bool saveConfig( void );
    bool loadConfig( void );            // load an eventual module'specific config file
    boolean loadSensoConfig( senso * ); // sensOCampus config to load (if any)

  private:
    deviceStatus_t _status;
    
    /*
     * private membre functions
     */
    bool _loadConfig( JsonObject );
    bool _processOrder( const char *, int *, const char * );  // an order to process with optional value
    void _constructor( void );                                // low-level constructor
};


#endif /* _DEVICE_H_ */


