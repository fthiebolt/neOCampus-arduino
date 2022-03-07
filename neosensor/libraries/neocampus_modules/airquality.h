/*
 * neOCampus operation
 * 
 * AirQuality module to manage all kind of air quality sensors that does not
 * fit within the existing sensOCampus classes.
 * 
 * F.Thiebolt   Aug.20  initial release
 * 
 */


#ifndef _AIRQUALITY_H_
#define _AIRQUALITY_H_

/*
 * Includes
 */

#include <Arduino.h>

// include base for all modules
#include "base.h"

// chips & devices drivers
#include "generic_driver.h"
#include "lcc_sensor.h"   // LCC's air quality sensors
#include "pm_serial.h"    // particule meters with serial link: includes PMSx003, SDS011, sensirion SPS30 ...
#include "SCD4x.h"        // Sensirion SCD4X CO2 sensor including temp + hygro



/*
 * Definitions
 */
#define _MAX_SENSORS                 4
#define AIRQUALITY_MIN_FREQUENCY    30      // may go up to every 30 seconds ...
#define AIRQUALITY_MAX_FREQUENCY    _MAX_COOLDOWN_SENSOR
#define DEFL_AIRQUALITY_FREQUENCY   (AIRQUALITY_MIN_FREQUENCY*2)
/* [aug.21] now considering COOLDOWN parameters */
#define AIRQUALITY_MIN_COOLDOWN     AIRQUALITY_MIN_FREQUENCY        // min delay between two consecutives sending
#define AIRQUALITY_MAX_COOLDOWN     AIRQUALITY_MAX_FREQUENCY        // max delay between two consecutives sending
#define DEFL_AIRQUALITY_COOLDOWN    (AIRQUALITY_MIN_COOLDOWN*2)     // default cooldown




/*
 * Class
 */
class airquality : public base {
  public:
    // constructor
    airquality( void );

    // destructor
    ~airquality( void );

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
    // supported devices
    generic_driver *_sensor[_MAX_SENSORS];
    
    /*
     * private membre functions
     */
    boolean _loadConfig( JsonObject );
    boolean _processOrder( const char *, int * );   // an order to process with optional value
    boolean _sendValues( void );                    // send all sensors' values
    void _process_sensors( void );                  // sensors internal processing (optional)
    void _constructor( void );                      // low-level constructor
};


#endif /* _AIRQUALITY_H_ */


