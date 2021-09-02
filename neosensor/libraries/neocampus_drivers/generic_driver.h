/**************************************************************************/
/*! 
  @file     generic_driver.h
  @author   F. Thiebolt
	@license
	
  This is part of a the neOCampus drivers library.
  Sensors generic driver
    
  (c) Copyright 2020 Thiebolt F. <thiebolt@irit.fr>

	@section  HISTORY

    aug.21  F.Thiebolt  added support for data integration
    aug.20  F.Thiebolt  added support for sensors' internal processing
                        used in continuous integration for example.
                        nowadays, only valid data will get sent
    2020-May    - First release, F. Thiebolt
    
*/
/**************************************************************************/

#ifndef _GENERIC_DRIVER_H_
#define _GENERIC_DRIVER_H_


#include <Arduino.h>
#include <ArduinoJson.h>
#include <limits.h>



/*
 * Definitions
 */

/* 
 * ANALOG DATA INTEGRATION
 * 
 * A sensor is read every 1s till value is stable over 'thresholdCpt' count,
 *   then it becomes the new official value;
 * If abs(new value read - _current) < PERCENT% ==> increase _currentCpt;
 * If _currentCPT >= 7 ==> it becomes the new official value
 * If abs(new official value - valueSent) > THRESHOLD ==> activate _trigger
 * _trigger activation will generate a data sending
 */
#define DEFL_READ_MSINTERVAL      1250      // default ms between two consecutives read of sensor
#define DEFL_THRESHOLD_CPT        7         // threshold counter to declare current value the new official one
#define DEFL_THRESHOLD_THOUSANDTH 15        // 1.5 percent variation threshold to consider stable value

#define _MAX_DATA_DECIMALS      3           // we won't support more than X decimals in data from sensors
#define DATA_SENDING_VARIATION_THRESHOLD  (float)(0.15) // new official value ought to differ more than this threshold to get sent



/*
 * Class
 */
class generic_driver {
  public:
    // constructor
    generic_driver( uint16_t read_msinterval=DEFL_READ_MSINTERVAL,
                    uint8_t threshold_cpt=DEFL_THRESHOLD_CPT,
                    uint8_t threshold_thousandth=DEFL_THRESHOLD_THOUSANDTH );
    
    // destructor
    virtual ~generic_driver( void ) { return; };

    // Power Modes
    virtual void powerON( void );         // switch ON
    virtual void powerOFF( void );        // switch OFF
    
    // Detection
    virtual boolean begin( uint8_t );       // i2c detected
    virtual boolean begin( JsonVariant );   // Json senso config

    // Data
    virtual void process( uint16_t coolDown=0,
                          uint8_t decimals=0 );   // sensors internal processing with coolDown parameter (e.g for continuous integration)
                                                  // The 'decimals' parameter is the number of digits after comma ==> 0 means integer !
    virtual boolean acquire( float* )=0;          // pure virtual, acquire sensor value
    virtual const char *sensorUnits( void )=0;    // pure virtual, retrieve units of actual sensors (e.g celsius, %r.H, lux ...)

    // Identity (i.e i2c addr)
    virtual String subID( void )=0;    // pure virtual, retrieve subID (i.e i2c addr)

    // data integration
    inline bool getTrigger( void ) { return _trigger; };  // local driver trigger that indicates a new official value needs to get sent
    inline float getValue( void ) { return value; };      // get official value that has gone through the whole integration process
    void setDataSent( void );                             // data has been sent, reset the 'new official data' trigger

    // public attributes

  // --- protected methods / attributes ---------------------
  // --- i.e subclass have direct access to
  protected:
    uint16_t      _readMsInterval;    // seconds interval between two consective data acquisition
    uint8_t       _thresholdCpt;      // threshold counter for stable data
    uint8_t       _thresholdThousandth;  // max tenth percent (i.e millièmes) data variation to consider as stable

    bool          _trigger;       // stable official value ought to get sent according to variation constraints and the coolDown/_lastTX value

    float         _current;
    uint8_t       _currentCpt;    // nb iteration _current is stable (usually beteween 5 to 10 ---i.e 5s to 10s)
    unsigned long _lastMsRead;    // (ms) last time data has been read from sensor (usually every 1s)

    float         value;          // official value
    unsigned long _lastMsWrite;   // (ms) last time official value has been written

    float         valueSent;      // official value that has been sent
    unsigned long _lastMsSent;    // (ms) time the official value has been sent
};

#endif /* _GENERIC_DRIVER_H_ */
