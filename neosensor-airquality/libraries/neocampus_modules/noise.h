/*
 * neOCampus operation
 * 
 * Noise module to detect noise according to parameters
 * 
 * Note: this module behave a bit differently from the others in a way that you don't
 * have to ask it whether there is noise or not: it will directly act on its own! 
 * 
 * Note: whatever the DAC used behind the scent, sensitivity will range from 0 to 100
 * 
 * Thiebolt F. July 17
 * 
 */


#ifndef _NOISE_H_
#define _NOISE_H_

/*
 * Includes
 */

#include <Arduino.h>
#include <Ticker.h>     // ESP8266 software timer library (i.e not a hardware timer)

// include base for all modules
#include "base.h"

// chips drivers
#include "driver_dac.h"
#include "MCP47X6.h"
#include "MCP47FEB.h"




/*
 * Definitions
 */
#define NOISE_MIN_FREQUENCY       300     // may go up to every 5mn ...
#define NOISE_MAX_FREQUENCY       3600    // or cool down to every hour
#define DEFL_NOISE_FREQUENCY      2700    // at least every 45mn, a message is sent, event without noise detected


// noise related definitions
#define DEFL_TIME_WINDOW          5     // (min.nb seconds of the sliding window that will lead to an evaluation of the number of pulses within
#define THRESHOLD_MIN             100   // minimum number of pulses across the DEFL_TIME_WINDOW seconds to declare that noise has been detected
#define THRESHOLD_MAX             1000
#define DEFL_PULSES_THRESHOLD     THRESHOLD_MIN // (uint16_t) pulses count threshold for a specified window of time


// TODO: these parameters OUGHT to get adjusted with real experiments
#define DEFL_SENSITIVITY          50    // percent of sensitivity (0 disabled, 100% is maximum sensitivity)
#define SENSITIVITY_MIN           0     // will be the minimum
#define SENSITIVITY_MAX           95    // ... will be the maximum




/*
 * Class
 */
class noise : public base {
  public:
    //noise( uint8_t pinSensor );
    noise( uint8_t pinSensor, voidFuncPtr isr, uint8_t pinLed=INVALID_GPIO );

    // add a DAC whose i2c adress is the parameter
    boolean add_dac( uint8_t adr );

    // check module's wealthness
    boolean is_empty();

    // noise detection parametrisation
    uint8_t setSensitivity( uint8_t );    // set percent sensitivity
    bool setThreshold( uint16_t );        // set pulse count threshold across the whole sliding window

    // Every second timer call to this method
    static void ICACHE_RAM_ATTR timerHandler( noise * );
    void ICACHE_RAM_ATTR noiseDetectISR( void );
    
    // MQTT
    bool start( senso * );
    bool process( void );     // process own module's activities

    void handle_msg( JsonObject );

    void status( JsonObject );
    void noiseDetectedMsg( JsonObject );
        
    // Module's config file
    bool saveConfig( void );


    bool loadConfig( void );          // load an eventual module'specific config file
    bool _loadConfig( JsonObject );

    /*
     * public attributes
     */
    uint8_t sensitivity;    // sensitivity of the noise module (0 --> 100%)

    
  private:
    // supported DAC (only one of one kind at this time :(
    driver_dac *_dac;
    uint8_t _pinSensor;     // pin connected to the comparator's output
    uint8_t _pinLed;        // pin connected to the onboard led signaling that noise has been detected

    uint16_t _pulseCountThreshold; // threshold above whose we signal that noise has been detected

    Ticker pulseT;                                  // 1 second timer
    volatile uint8_t noiseTab[DEFL_TIME_WINDOW+1];  // contains noise pulse count for each second
    volatile uint8_t curNoiseEntry;                 // tells to pulse counter ISR which cell of the noiseTab to increment
    volatile uint16_t _SumNoisePulseCount;          // sum of noise pulse count across the Time Window (5s default)
    volatile bool noiseDetected;
    
    voidFuncPtr _noiseDetectISR;                    // pointer to ISR (declared outside)
    
    /*
     * private membre functions
     */
    void inline _ledON( void );
    void inline _ledOFF( void );
    bool _processOrder( const char *, int * );      // an order to process with optional value

};


#endif /* _NOISE_H_ */


