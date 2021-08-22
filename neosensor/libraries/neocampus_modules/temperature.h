/*
 * neOCampus operation
 * 
 * Temperature module to manage all temperature sensors
 * 
 * F.Thiebolt   aug.21  added support for analog data integration and cooldown feature
 * Thiebolt F.  dec.17  added polymorphism with support for multiple sensors
 * Thiebolt F.  jul.17  initial release
 * 
 */


#ifndef _TEMPERATURE_H_
#define _TEMPERATURE_H_

/*
 * Includes
 */

#include <Arduino.h>

// include base for all modules
#include "base.h"

// chips drivers
#include "generic_driver.h"
#include "Adafruit_MCP9808.h"
#include "SHT2x.h"
#include "SHT3x.h"
//#include "TCN75A.h" later maybe




/*
 * Definitions
 */
#define _MAX_SENSORS                  4
#define TEMPERATURE_MIN_FREQUENCY     30      // (i.e coolDown) may go down to 30 seconds ...
#define TEMPERATURE_MAX_FREQUENCY     1800    // or cool down to every every 30mn
#define DEFL_TEMPERATURE_FREQUENCY    (TEMPERATURE_MIN_FREQUENCY*2)     // temperature message every 60 seconds by default ...
/* [aug.21] now considering COOLDOWN parameters */
#define TEMPERATURE_MIN_COOLDOWN      TEMPERATURE_MIN_FREQUENCY         // min delay between two consecutives sending
#define TEMPERATURE_MAX_COOLDOWN      TEMPERATURE_MAX_FREQUENCY         // max delay between two consecutives sending
#define DEFL_TEMPERATURE_COOLDOWN     (TEMPERATURE_MIN_COOLDOWN*2)      // temperature message 60 seconds cooldown by default ...



/*
 * Class
 */
class temperature : public base {
  public:
    // constructors
    temperature( void );
    temperature( JsonDocument& );

    // destructor
    ~temperature( void );

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
    generic_driver *_sensor[_MAX_SENSORS];

    /*
     * private membre functions
     */
    bool _loadConfig( JsonObject );
    bool _processOrder( const char *, int * );  // an order to process with optional value
    boolean _sendValues( void );                // send all sensors' values
    void _process_sensors( void );              // sensors internal processing (optional)
    void _temperature( void );                  // low-level constructor
};

#endif /* _TEMPERATURE_H_ */


