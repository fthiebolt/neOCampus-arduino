/*
 * neOCampus operation
 * 
 * neOClock module
 * 
 * Note: intended to 7 segments based clock featuring HOURS:MINUTES
 * (i.e not seconds)
 * 
 * F.Thiebolt aug.21  DEPRECATION notice ... will get replaced by the 'display' module
 * F.Thiebolt Jun.18  initial release
 * 
 */


#ifndef _NECLOCK_H_
#define _NECLOCK_H_

/*
 * Includes
 */

#include <Arduino.h>
#include <time.h>       // time() ctime()
#include <Ticker.h>     // ESP8266 software timer library (i.e not a hardware timer)

// include base for all modules
#include "base.h"

// displays drivers
#include "driver_display.h"
#include "TM1637Display.h"





/*
 * Definitions
 */
#define NEOCLOCK_TM1637     (uint8_t)42



/*
 * Class
 */
class neoclock : public base {
  public:
    neoclock();

    // add a display
    bool add_display( uint8_t );
    bool is_empty( void ) { return (_display ? false : true); };
    
    // MQTT
    bool start( senso *, JsonDocument& );   // note: neOClock does not communicate with MQTT
    bool stop( void );
    bool process( void );     // process own module's activities

    void handle_msg( JsonObject root ) {};   // no MQTT message to process
/*
    void status( JsonObject );
    
    // Module's config
    bool saveConfig( void );
    bool loadConfig( void );            // load an eventual module'specific config file
    bool _loadConfig( JsonObject );     // move to private methods
    boolean loadSensoConfig( senso * ); // sensOCampus config to load (if any)
*/
    // Clock source
    // bool setClockSource( uint8_t );  // local, RTC, SNTP

    // Handlers
    // Every second timer call to this method
    static void ICACHE_RAM_ATTR timerHandler( neoclock * );

    // animation
    bool animate( displayAnimate_t mode=displayAnimate_t::demo );
    
  private:
    // only one display at time supported
    driver_display *_display;

    // current time shown on clock
    bool _initialized;
    time_t _curDisplayedTime;     // hour currently displayed on clock, uint32_t format
    struct tm _tm_displayedTime;  // hour currently displayed on clock, struct tm format
    uint8_t _secondsLeft;         // seconds left to next minute change
    bool _displayChange;          // bool to trigger a change in the clock displayed values
    
    // The one second timer
    Ticker _timer1s;            // our 1second timer

    /*
     * private membre functions
     */
//  bool _processOrder( const char *, int * );  // an order to process with optional value
};


#endif /* _NEOCLOCK_H_ */


