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

// chips drivers
#include "generic_driver.h"
#include "lcc_airquality.h"




/*
 * Definitions
 */
#define _MAX_SENSORS                  4
#define AIRQUALITY_MIN_FREQUENCY     30      // may go up to every 30 seconds ...
#define AIRQUALITY_MAX_FREQUENCY     1800    // or cool down to every every 30mn
#define DEFL_AIRQUALITY_FREQUENCY    (AIRQUALITY_MIN_FREQUENCY*2)     // air-quality message every 30 seconds by default ...




/*
 * Class
 */
class airquality : public base {
  public:
    airquality();

    // add a sensor whose i2c adress is the parameter
    boolean add_sensor( uint8_t adr );
    boolean is_empty();

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
    boolean _loadConfig( JsonObject );
    boolean _processOrder( const char *, int * );   // an order to process with optional value
    boolean _sendValues( void );                    // send all sensors' values
};


#endif /* _AIRQUALITY_H_ */


