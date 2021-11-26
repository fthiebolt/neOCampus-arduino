/**************************************************************************/
/*! 
    @file     pm_serial.h
    @author   F. Thiebolt
	  @license
	
    This is part of a the neOCampus drivers library.
    This driver is intended various particules meters featuring a serial link:
    - PMSx003
    - SDS011
    - [later] Sensirion SPS30
    - CO2 sensor PMH-Z14 (*)
    
    (c) Copyright 2021 Thiebolt F. <thiebolt@irit.fr>

  @note
    As of nov.21, in addition to the various particle meters supported, we decided
    to add CO2 as being part of air quality measurement.

	@section  HISTORY

    nov.21  F.Thiebolt  integration of functionalities from PMS_library sensor
    oct.21  F.Thiebolt  initial release
    
*/
/**************************************************************************/

#ifndef _PM_SERIAL_H_
#define _PM_SERIAL_H_


#include <Arduino.h>
#include <ArduinoJson.h>

#include "sensocampus.h"

// generic sensor driver
#include "generic_driver.h"



/*
 * PM_SERIAL sensor general principle:
 *
 * [PMSx003] serial2 link 8N1 9600 ==> 32bytes frame takes < 34ms
 * [Sensirion SPS30] serial2 link 8N1 115200
 * Feature both a PASSIVE and an ACTIVE modes:
 *  [PASSIVE MODE] sensor sends back data on request
 *  (default)[ACTIVE MODE] continously send PM measurement on serial link
 * [PMSx003] Note we won't make use of PM_ENABLE pin as PMS5003 features command for sleep/wakeup
 * Note: we'll make use of the data driven sending (i.e we'll only send data when they change)
 */

/*
 * SERIAL SENSOR overall data processing:
 *
 * [PMSx003) sends back 3 different values --> PM1.0, PM2.5 and PM10
 * [SPS30] sends back 4 different values --> PM1.0, PM2.5, PM4.0 and PM10
 * 
 * WARNING: PM1.0 is 0.3 to 1.0µm particles (PMSx003 is less than 50% precise on sub 0.5µm ==> forget it!)
 *          PM2.5 is 1.0 to 2.5µm
 *          PM10  is 2.5 to 10µm
 * ... hence we'll only focusing on PM2.5
 */

/*
 * Definitions
 */
#define PM_WAKEUP_DELAY       30    // seconds waking up sensors before starting to read data

/* reading data timeout ms delay
 * Delay for data to arrive within serial buffer.
 */
#define PM_READ_MSTIMEOUT     130   // ms timeout delay for reading data from serial link (use to be < 40ms)

// Finite state machine
enum class pmSensorState_t : uint8_t {
  idle            = 0,
  wakeup,               // only when sensor is sleeping
  measuring,            // reading PM values from serial link
  wait4read             // waiting for triggered values to get read
};
#define PM_FSMSTATE_DEFL      pmSensorState_t::idle

/* default serial speed for PM sensors will be 9600 bauds
 * [PMSx003] 9600 bauds
 * [SCP30] 115200 bauds
 */
#define PM_DEFL_LINK_SPEED    9600  // serial link default speed

// type of sensor
enum class pmSensorType_t : uint8_t { 
  PMSx003       = 0x10,       // PMS5003, PMSA003 ...
  SDS011        = 0x20,
  SPS3X         = 0x30,       // sensiron SPS3X
  IKEA          = 0x40,       // IKEA VINDTRIKNING features a Cubic PM1006K particles sensor
  MHZ1x         = 0x50,       // MH-Z1x CO2 sensors and variants (e.g MH-Z14a, MH-Z19b ...)
  // add additional kind of PM sensors here

  undefined     = (uint8_t)(-1)
};

// single measure data structure
typedef struct {
  bool          _trigger;     // stable official value ought to get sent according to variation constraints and the coolDown/_lastTX value

  float         _currentSum;  // sum of all values read (will get divided by readIter for avg)

  float         value;        // official value (i.e avg)

  float         valueSent;    // latest official value that has been sent
  unsigned long _lastMsSent;  // (ms) time the official value has been sent (starvation avoidance)

  const char    *subID;       // subID string
  const char    *units;       // units string
} serialMeasure_t;

#define PM_MAX_MEASURES         4 // maximum number of single measures

/* PMSx003 measurements
 * very low precision on 1µm particle so we don't take mention of them
 */
enum class pmsx003DataIdx_t : uint8_t {
  PM2_5=0,
  PM10,
  last
};

// SDS011 measurements
enum class sds011DataIdx_t : uint8_t {
  PM2_5=0,
  PM10,
  last
};

/* IKEA measurements
 * [nov.21] Cubic PM1006K only reports PM2.5 (downgraded firmware ?)
 */
enum class ikeaDataIdx_t : uint8_t {
  PM2_5=0,
  PM10,
  last
};

/* MH-Z1x measurements
 * [nov.21] CO2 sensor
 */
enum class mhz1xDataIdx_t : uint8_t {
  CO2=0,
  last
};



/*
 * Class
 */
class pm_serial : public generic_driver {
  public:

    // constructor
    pm_serial( void );

    // destructor
    ~pm_serial( void );
    
    // sensor creation via sensOCampus JSON array 'params'
    boolean begin( JsonVariant );
    void powerON( void );
    void powerOFF( void );

    void process( uint16_t coolDown=0,    // cooldown(seconds)
                  uint8_t decimals=0 );   // override generic:process for our sensor internal processing

    // send back sensor's value, units and subID
    boolean acquire( float* ) { return false; };    // acquire is part of generic::process() we override
    const char *sensorUnits( uint8_t=0 );
    String subID( uint8_t=0 );

    // data integration, override generic_driver
    bool getTrigger( void );                  // local driver trigger that indicates a new official value needs to get sent
    float getValue( uint8_t *idx=nullptr );   // get official value that has gone through the whole integration process
    void setDataSent( void );                 // data has been sent, reset the 'new official data' trigger

  // --- protected methods / attributes ---------------------
  // --- i.e subclass have direct access to
  protected:
    // --- private/protected attributes
    uint8_t _link;                      // serial link number (e.g 2 --> Serial2)
    Stream* _stream;                    // serial link stream (pointer)
    unsigned int _link_speed;           // 9600 to 115200 bauds
    uint8_t _enable_gpio;               // PM_ENABLE gpio

    pmSensorType_t _sensor_type;        // kind of PM sensor (e.g PMSx003, SDS011, SCP30 ...)
    boolean _activeMode;                // only relevant to PM sensors having support for active/passive modes
    boolean _powerSave;                 // power save enabled or not

    pmSensorState_t _FSMstatus;         // FSM
    unsigned long _FSMtimerStart;       // ms system time start of current state;
                                        // Only relevant when timerDelay is not null
    uint16_t _FSMtimerDelay;            // ms delay to cur state timeout
    
    boolean _initialized;
    static const char *units_pm;
    static const char *units_co2;
    static const char *subID_pm2_5;
    static const char *subID_pm10;
    static const char *subID_co2;

    // data integration
    boolean _trigger;                   // when triggered, multiple call to getValue(*idx) will send back all stable values
    serialMeasure_t *_measures;         // array of measurement structs
    uint8_t _nbMeasures;                // size of measurement array
    uint8_t _readCpt;                   // nb read iteration in current reading campaign
    uint8_t _retryCpt;                  // nb reading retries during a measurement cycle
    unsigned long _lastMsRead;          // (ms) last time data has been read from sensor 

    // -- private/protected methods
    boolean _init( void );          // low-level init

    boolean FSMwakeUpStart( uint16_t=PM_WAKEUP_DELAY );
    boolean FSMwakeUpBusy( void );

    boolean FSMmeasureStart( void );
    boolean FSMmeasureBusy( void );

    boolean serialRead_pmsx003( uint16_t=PM_READ_MSTIMEOUT );   // blocking call reading data from serial till data OK or timeout
    boolean serialRead_sds011( uint16_t=PM_READ_MSTIMEOUT );    // blocking call reading data from serial till data OK or timeout
    boolean serialRead_ikea( uint16_t=PM_READ_MSTIMEOUT );      // blocking call reading data from serial till data OK or timeout

    // -- [ll] private/protected methods
    boolean _ll_sleep( void );
    boolean _ll_wakeUp( void );
    boolean _ll_passiveMode( void );
    boolean _ll_activeMode( void );
    boolean _ll_requestRead( void );
};

#endif /* _PM_SERIAL_H_ */
