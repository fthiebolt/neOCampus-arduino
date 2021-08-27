/*
 * neOCampus operation
 * 
 * Luminosity module to manage all luminosity sensors
 * 
 * F.Thiebolt   aug.21  added support for analog data integration and cooldown feature
 * Thiebolt F. Dec.17   added polymorphism with support for multiple sensors
 * Thiebolt F. July 17  initial release
 * 
 * Thiebolt F. July 17
 * 
 */


#ifndef _LUMINOSITY_H_
#define _LUMINOSITY_H_

/*
 * Includes
 */

#include <Arduino.h>

// include base for all modules
#include "base.h"

// chips drivers
#include "generic_driver.h"
#include "TSL2561.h"
#include "MAX44009.h"




/*
 * Definitions
 * [aug.21] we're now following a 'cooldown' approach about data sending:
 * - new data (i.e that changed from previous sent) won't get sent before 'cooldown' seconds
 * - if _MAX_COOLDOWN_SENSOR is reached, data will get sent even if it' not a new one
 */
#define _MAX_SENSORS                  4
#define LUMINOSITY_MIN_FREQUENCY      15      // may go up to every 15 seconds ...
#define LUMINOSITY_MAX_FREQUENCY      _MAX_COOLDOWN_SENSOR
#define DEFL_LUMINOSITY_FREQUENCY     (LUMINOSITY_MIN_FREQUENCY*2)    // luminosity message every 20 seconds by default ...
/* [aug.21] now considering COOLDOWN parameters */
#define LUMINOSITY_MIN_COOLDOWN       LUMINOSITY_MIN_FREQUENCY        // min delay between two consecutives sending
#define LUMINOSITY_MAX_COOLDOWN       LUMINOSITY_MAX_FREQUENCY        // max delay between two consecutives sending
#define DEFL_LUMINOSITY_COOLDOWN      (LUMINOSITY_MIN_COOLDOWN*2)     // luminosity message 60 seconds cooldown by default ...




/*
 * Class
 */
class luminosity : public base {
  public:
    luminosity( void );

    // destructor
    ~luminosity( void );

    // add a sensor whose i2c adress is the parameter
    boolean add_sensor( uint8_t adr );
    boolean is_empty();

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
    // supported devices (only one of one kind at this time :(
    generic_driver *_sensor[_MAX_SENSORS];
    
    /*
     * private membre functions
     */
    bool _loadConfig( JsonObject );
    bool _processOrder( const char *, int * );  // an order to process with optional value
    boolean _sendValues( void );                // send all sensors' values
    void _process_sensors( void );              // sensors internal processing (optional)
    void _constructor( void );                  // low-level constructor
};


#endif /* _LUMINOSITY_H_ */


