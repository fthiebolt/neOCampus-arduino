/*
 * neOCampus operation
 * 
 * Humidity module to manage all hygro sensors
 * 
 * F.Thiebolt   aug.21  added support for analog data integration and cooldown feature
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
 * [aug.21] we're now following a 'cooldown' approach about data sending:
 * - new data (i.e that changed from previous sent) won't get sent before 'cooldown' seconds
 * - if _MAX_COOLDOWN_SENSOR is reached, data will get sent even if it' not a new one
 */
#define _MAX_SENSORS                4
#define HUMIDITY_MIN_FREQUENCY      30      // may go up to every 30 seconds ...
#define HUMIDITY_MAX_FREQUENCY      _MAX_COOLDOWN_SENSOR
#define DEFL_HUMIDITY_FREQUENCY     (HUMIDITY_MIN_FREQUENCY*2)    // message every 60 seconds by default ...
/* [aug.21] now considering COOLDOWN parameters */
#define HUMIDITY_MIN_COOLDOWN       HUMIDITY_MIN_FREQUENCY        // min delay between two consecutives sending
#define HUMIDITY_MAX_COOLDOWN       HUMIDITY_MAX_FREQUENCY        // max delay between two consecutives sending
#define DEFL_HUMIDITY_COOLDOWN      (HUMIDITY_MIN_COOLDOWN*2)     // temperature message 60 seconds cooldown by default ...



/*
 * Class
 */
class humidity : public base {
  public:
    // constructors
    humidity( void );
    humidity( JsonDocument& );

    // destructor
    ~humidity( void );

    // add a sensor whose i2c adress is the parameter
    boolean add_sensor( uint8_t adr );
    boolean is_empty( void );
    
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
    // supported devices (only one of one kind at this time :(
    generic_driver  *_sensor[_MAX_SENSORS];

    /*
     * private membre functions
     */
    bool _loadConfig( JsonObject );
    bool _processOrder( const char *, int * );  // an order to process with optional value
    boolean _sendValues( void );                // send all sensors' values
    void _process_sensors( void );              // sensors internal processing (optional)
    void _constructor( void );                  // low-level constructor
};


#endif /* _HUMIDITY_H_ */
