/**************************************************************************/
/*! 
    @file     driver_display.h
    @author   F. Thiebolt
	  @license
	
    This is part of a the neOCampus drivers library.
    Implement a generic display to leverage the needs that stems from
    multiple types of displays availability.
    
    (c) Copyright 2018 Thiebolt F. <thiebolt@irit.fr>

	@section  HISTORY

    2018-Jun    - First release, F. Thiebolt
    
*/
/**************************************************************************/

#ifndef _DRIVER_DISPLAY_H_
#define _DRIVER_DISPLAY_H_


#include <Arduino.h>
#include <ArduinoJson.h>
#include <limits.h>



/*
 * Definitions
 */
// animation modes
enum class displayAnimate_t : uint8_t {
  demo      = (uint8_t)(-1),  // demo mode
  stop                  = 0,  // i.e normal mode
  network_connect,            // connecting to a network
  network_connect_alt,        // alternative animation version when connecting to a network
  network_fail,               // quite explicit ...
};

/*
 * Finite State Machine
 */
// Logo delay
#define DISPLAY_LOGO_MS        5000    // ms LOGO will get displayed (max 65535)
#define DISPLAY_TIME_MS       10000    // ms TIME will get displayed (max 65535)
#define DISPLAY_MSG_MS        10000    // ms MSG will get displayed (max 65535)
#define DISPLAY_WEATHER_MS    10000    // ms WEATHER will get displayed (max 65535)

// Finite state machine
enum class displayState_t : uint8_t {
  idle            = 0,
  logo,                 // display logo
  time,                 // display time
  message,              // display message (e.g sensors values or custom message)
  weather,              // display weather
  last
};
#define DISPLAY_FSMSTATE_DEFL     displayState_t::idle



/*
 * Class
 */
class driver_display {
  public:
    // constructor
    driver_display( void );
    
    // destructor
    virtual ~driver_display( void ) { return; };
  
    // Power Modes
    virtual void powerON( void );       // switch ON
    virtual void powerOFF( void );      // switch OFF

    // Detection
    virtual boolean begin( uint8_t );       // i2c detected
    virtual boolean begin( JsonVariant );   // Json senso config

    // Data
    virtual void process( uint16_t coolDown=0 );    // display internal processing

    //virtual boolean acquire( float* )=0;          // pure virtual, acquire sensor value
    //virtual const char *sensorUnits( void )=0;    // pure virtual, retrieve units of actual sensors (e.g celsius, %r.H, lux ...)
  
    // Identity (e.g i2c addr)
    virtual String subID( void )=0;    // pure virtual, retrieve subID (e.g i2c addr)

    // data integration
    inline bool getTrigger( void ) { return _trigger; };  // local driver trigger that indicates a new official value needs to get sent
    inline float getValue( void ) { return value; };      // get official value that has gone through the whole integration process
    void setDataSent( void );                             // data has been sent, reset the 'new official data' trigger

    // Brightness & others
    virtual uint8_t setPercentBrightness( uint8_t );          // 0 -> 100% luminosity
    [[deprecated]] virtual bool setDotsBlinking( bool );             // central dots blinking or not
    
    /* Display methods to get overridden in child classes
     * Each display method feature its 'BUSY' companion
     */
    virtual bool    dispLogo( void );         // display a logo :D
    virtual uint8_t dispMsg( const char * );  // display a text message
    virtual uint8_t dispTime( uint8_t hours, uint8_t minutes, uint8_t seconds=0 );  // display time
    //virtual bool    dispSensors( JsonVariantConst );  // display sensors from shared JSON structure
    virtual bool    dispWeather( const char *city, float temperature=0.0, float hygro=0.0, bool sunny=false, bool rainy=false, bool windy=false );  // display weather

    virtual bool    dispLogoBusy( void );
    virtual bool    dispMsgBusy( void );
    virtual bool    dispTimeBusy( void );
    //virtual bool    dispSensorsBusy( void );
    virtual bool    dispWeatherBusy( void );

    // FSM related
    bool isFSMstatus( displayState_t );

    // various animation modes mainly related to device setup & demo modes
    virtual bool animate( displayAnimate_t mode=displayAnimate_t::demo );

    /* ---- public attributes -----------------------------------------------*/
    // global shared JSON
    JsonVariantConst sharedRoot;    // global shared JsonDocument


  // --- protected methods / attributes ---------------------
  // --- i.e subclass have direct access to
  protected:
    // data sending
    bool            _trigger;       // true means that the corresponding driver needs to send something

    float           value;          // official value
    unsigned long   _lastMsWrite;   // (ms) last time official value has been written

    float           valueSent;      // official value that has been sent
    unsigned long   _lastMsSent;    // (ms) time something has been sent

    // current time
    uint8_t   _hours,_minutes;      // hours:minutes for child class

    // state machine
    displayState_t _FSMstatus;    // FSM
    unsigned long _FSMtimerStart; // ms system time start of current state;
                                  // Only relevant when timerDelay is not null
    uint16_t _FSMtimerDelay;      // ms delay to cur state timeout
    bool _FSMinitialized;

    /* ---- private methods -------------------------------------------------*/
    bool _begin( void );          // low-level part of begin()
    bool _FSMstateBusy( void );   // current FSM state still busy ?
};

#endif /* _DRIVER_DISPLAY_H_ */
