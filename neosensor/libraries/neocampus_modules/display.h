/*
 * neOCampus operation
 * 
 * Display module to manage all kinds of displays ranging from
 *  led strips, 7segments and oled displays.
 * It also manage the time to get displayed through it's 1s ticker
 * 
 * ---
 * Notes:
 * - This class will supersedes the neoclock module. It's TM1637 driver
 *  will become a driver to this new class.
 * ---
 * 
 * F.Thiebolt   aug.21  initial release
 * 
 */


#ifndef _DISPLAY_H_
#define _DISPLAY_H_

/*
 * Includes
 */

#include <Arduino.h>
#include <time.h>       // time() ctime()
#include <Ticker.h>     // ESP8266 software timer library (i.e not a hardware timer)

// include base for all modules
#include "base.h"

// chips drivers
#include "driver_display.h"       // virtual class for all displays drivers
#include "oled1.3inch.h"          // 1.3 inch oled display based on SH1106
//#include "TM1637.h"             // neOClock driver
//#include "SK9822.h"             // strip leds (APA102 compatible)



/*
 * Definitions
 * [aug.21] we're now following a 'cooldown' approach as in all others sensors modules.
 * However, there's a big difference here since displays are ACTUATORS (i.e not sensors),
 *  hence, the cooldown value may get interpreted in a slightly different way.
 */
#define _MAX_DISPLAYS                 4
#define DISPLAY_MIN_COOLDOWN          15            // interval betwwen two subsequent changes in what is displayes
#define DISPLAY_MAX_COOLDOWN          _MAX_COOLDOWN_ACTUATOR
#define DEFL_DISPLAY_COOLDOWN         (DISPLAY_MIN_COOLDOWN*2)



/*
 * Class
 */
class display : public base {
  public:
    // constructors
    display( void );

    // destructor
    ~display( void );


    // add a sensor whose i2c adress is the parameter
    boolean add_display( uint8_t );
    boolean is_empty( void );


    // MQTT
    bool start( senso *, JsonDocument& );
    bool stop( void );
    bool process( void );     // process own module's activities
  
    void handle_msg( JsonObject );

    void status( JsonObject );
    
    // Module's config
    bool saveConfig( void );
    bool loadConfig( void );            // load an eventual module'specific config file
    boolean loadSensoConfig( senso * ); // sensOCampus config to load (if any)


    // Handlers
    // Every second timer call to this method
    static void ICACHE_RAM_ATTR timerHandler( display * );

    // animation
    bool animate( displayAnimate_t mode=displayAnimate_t::demo );

  private:
    // supported devices
    driver_display *_display[_MAX_DISPLAYS];
    uint8_t _displays_count;            // current number of registered displays

    // global shared JSON
    JsonVariant _sharedRoot;            // global shared JsonDocument

    // current time shown on clock
    bool _initialized;
    time_t _curDisplayedTime;     // hour currently displayed on clock, uint32_t format
    struct tm _tm_displayedTime;  // hour currently displayed on clock, struct tm format
    uint8_t _secondsLeft;         // seconds left to next minute change
    bool _displayChange;          // bool to trigger a change in the clock displayed values
    
    // The one second timer
    Ticker _timer1s;              // our 1second timer

    /*
     * private membre functions
     */
    bool _loadConfig( JsonObject );
    bool _processOrder( const char *, int * );  // an order to process with optional value
    boolean _sendValues( void );                // send all sensors' values
    void _process_displays( void );             // displays internal processing (optional)
    void _constructor( void );                  // low-level constructor
};

#endif /* _DISPLAY_H_ */


