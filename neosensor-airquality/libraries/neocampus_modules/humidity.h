/*
 * neOCampus operation
 * 
 * Humidity module to manage all hygro sensors
 * 
 * Thiebolt F.  May.20  initial release
 * 
 */


#ifndef _HUMIDITY_H_
#define _HUMIDITY_H_

/*
 * Includes
 */

#include <Arduino.h>

// include base for all modules
#include "base.h"

// chips drivers
#include "generic_driver.h"
#include "SHT2x.h"
#include "SHT3x.h"



/*
 * Definitions
 */
#define _MAX_SENSORS                4
#define HUMIDITY_MIN_FREQUENCY      30      // may go up to every 30 seconds ...
#define HUMIDITY_MAX_FREQUENCY      1800    // or cool down to every every 30mn
#define DEFL_HUMIDITY_FREQUENCY     (HUMIDITY_MIN_FREQUENCY*2)     // message every 60 seconds by default ...



/*
 * Class
 */
class humidity : public base {
  public:
    humidity();

    // add a sensor whose i2c adress is the parameter
    boolean add_sensor( uint8_t adr );
    boolean is_empty( void );
    
    // MQTT
    bool start( senso * );
    bool process( void );     // process own module's activities

    void handle_msg( JsonObject );

    void status( JsonObject );
    
    // Module's config file
    bool saveConfig( void );
    bool loadConfig( void );          // load an eventual module'specific config file
    
  private:
    // supported devices (only one of one kind at this time :(
    generic_driver  *_sensor[_MAX_SENSORS];

    /*
     * private membre functions
     */
    bool _loadConfig( JsonObject );
    bool _processOrder( const char *, int * );  // an order to process with optional value
    boolean _sendValues( void );                // send all sensors' values
};


#endif /* _HUMIDITY_H_ */
