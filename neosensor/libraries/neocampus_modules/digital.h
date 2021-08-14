/*
 * neOCampus operation
 * 
 * Digital module to manage all kind of digital inputs/outputs.
 * Unlike others module, this one does not feature automatic sending
 * of sensors' data on a time interval basis
 * 
 * F.Thiebolt   Aug.21  initial release
 * 
 */


#ifndef _DIGITAL_H_
#define _DIGITAL_H_

/*
 * Includes
 */

#include <Arduino.h>

// include base for all modules
#include "base.h"

// chips drivers
//#include "driver_gpio"




/*
 * Definitions
 */
#define _MAX_GPIOS    8   // maximum number of managed GPIOs

// Types of connected devices
enum class digitalInputType_t : uint8_t {
  undefined,
  presence,               // PIR sensors
  on_off                  // switches
};

// Fronts detection delcaration
enum class digitalFrontDetect_t : int8_t {
  none            = -1,
  falling         = 0,    // only falling edges (i.e 1 --> 0)
  rising,                 // only rising edges (i.e 0 --> 1)
  both                    // both falling and rising edges
};

// digital_gpio typedef
typedef struct {
  uint8_t   pin;          // pin number
  digitalInputType_t    type;           // type of sensor (e.g presence, on_off...)
  digitalFrontDetect_t  front;          // kind of fronts to detect
  bool      _trigger;     // detected front according to the 'front' field and the coolDown/_lastTX value
  bool      _current;
  bool      _previous;
  bool      value;        // official value
  uint16_t  coolDown;     // seconds to wait between two consecutives events
  unsigned long _lastTX;  // elapsed ms since last message sent
} digitalGPIO_t;



/*
 * Class
 */
class digital : public base {
  public:
    digital();

    // add a gpio
    boolean add_gpio( uint8_t pin, digitalInputType_t type, digitalFrontDetect_t front = digitalFrontDetect_t::both, uint16_t coolDown = 0 );
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
    // array of GPIOs
    digitalGPIO_t *_gpio[_MAX_GPIOS];
    uint8_t _gpio_count;    // current number of registered digital gpios

    // flag to specifiy that at least one digital input trigger has been activated
    boolean _trigger;

    /*
     * private member functions
     */
    boolean _loadConfig( JsonObject );
    boolean _processOrder( const char *, int * );   // an order to process with optional value
    boolean _sendValues( void );                    // send all sensors' values
    void _process_sensors( void );                  // sensors internal processing (optional)
};


#endif /* _DIGITAL_H_ */
