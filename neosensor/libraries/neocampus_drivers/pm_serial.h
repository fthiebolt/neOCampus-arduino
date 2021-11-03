/**************************************************************************/
/*! 
    @file     pm_serial.h
    @author   F. Thiebolt
	  @license
	
    This is part of a the neOCampus drivers library.
    This driver is intended various particules meters
    featuring a serial link:
    - PMSx003
    - [later] SDS011
    - [later] Sensirion SCP30
    
    Note: we'll only send back PM2.5 values

    (c) Copyright 2021 Thiebolt F. <thiebolt@irit.fr>

	@section  HISTORY

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
#include "PMS.h"              // PMSx003 low-level driver from Arduino's libraries


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
#define PM_READ_MSTIMEOUT     100   // ms timeout delay for reading data from serial link (use to be < 40ms)

/* define maximum number of measures
 * over a single campaign.
 */
#define _MEASURES_INTERLEAVE_MS     DEFL_READ_MSINTERVAL  // delay between two measures in the 'measuring' state
#define _MAX_MEASURES               (uint8_t)5            // max. is 255

// Finite state machine
enum class pmSensorState_t : uint8_t {
  idle            = 0,
  wakeup,               // only when sensor is sleeping
  measuring             // reading PM values from serial link
};
#define PM_FSMSTATE_DEFL      pmSensorState_t::idle

/* default serial speed for PM sensors will be 9600 bauds
 * [PMSx003] 9600 bauds
 * [SCP30] 115200 bauds
 */
#define PM_DEFL_LINK_SPEED    9600  // serial link default speed

// type of PM sensor
enum class pmSensorType_t : uint8_t {
  PMSx003       = 0x10,
  SDS011        = 0x20,
  SCP30         = 0x30
};



/*
 * Class
 */
class pm_serial : public generic_driver {
  public:

    // constructor
    pm_serial( void );
    
    // sensor creation via sensOCampus JSON array 'params'
    boolean begin( JsonVariant );
    void powerON( void );
    void powerOFF( void );
/*
    void process( uint16_t coolDown=0,
                  uint8_t decimals=0 );  // override generic:process for our sensor internal processing
*/
    // send back sensor's value, units and subID
    boolean acquire( float* );
    const char *sensorUnits( void ) { return units; };
    String subID( void ) { return _subID; };

  // --- protected methods / attributes ---------------------
  // --- i.e subclass have direct access to
  protected:
    // -- private/protected methods
    boolean _init( void );          // low-level init

    boolean wakeUpStart( uint16_t=PMS_WAKEUP_DELAY );
    boolean wakeUpBusy( void );

    boolean measureStart( void );
    boolean measureBusy( void );

    // --- private/protected attributes
    PMS *_psensor;                      // [ll] pointer to low-level PMS serial sensor

    uint8_t _link;                      // serial link number (e.g 2 --> Serial2)
    unsigned int _link_speed;           // 9600 to 115200 bauds
    uint8_t _enable_gpio;               // PM_ENABLE gpio

    pmsSensorState_t _FSMstatus;        // FSM
    unsigned long _FSMtimerStart;       // ms system time start of current state;
                                        // Only relevant when timerDelay is not null
    uint16_t _FSMtimerDelay;            // ms delay to cur state timeout
    
    boolean _initialized;
    static const char *units;
};

#endif /* _PM_SERIAL_H_ */
