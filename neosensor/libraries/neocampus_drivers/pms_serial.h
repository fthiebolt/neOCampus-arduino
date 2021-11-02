/**************************************************************************/
/*! 
    @file     pms_serial.h
    @author   F. Thiebolt
	  @license
	
    This is part of a the neOCampus drivers library.
    This driver is intended to the PMS50XX particules meter
    
    (c) Copyright 2021 Thiebolt F. <thiebolt@irit.fr>

	@section  HISTORY

    oct.21  F.Thiebolt  initial release
    
*/
/**************************************************************************/

#ifndef _PMS_SERIAL_H_
#define _PMS_SERIAL_H_


#include <Arduino.h>
#include <ArduinoJson.h>

#include "sensocampus.h"

// generic sensor driver
#include "generic_driver.h"
#include "PMS.h"              // low-level driver from Arduino's libraries


/*
 * PMS_SERIAL sensor general principle:
 * We'll make use of serial2 link 8N1 9600 bauds ==> 32bytes frame takes < 34ms
 * It features both a PASSIVE and an ACTIVE modes:
 *  [PASSIVE MODE] sensor sends back data on request
 *  (default)[ACTIVE MODE] continously send PM measurement on serial link
 * Note we won't make use of PM_ENABLE pin as PMS5003 features command for sleep/wakeup
 * Note: we'll make use of the data driven sending (i.e we'll only send data when they change)
 */

/*
 * SERIAL SENSOR overall data processing:
 *
 * PMS5003 sends back 3 different values --> PM1.0, PM2.5 and PM10
 * generic_driver::process() is overriden and will implement the Finite State Machine (FSM)
 */

/*
 * Definitions
 */
#define PMS_DEFL_LINK_SPEED   9600  // serial link default speed
#define PMS_WAKEUP_DELAY      30    // seconds waking up sensors before starting to read data

/* reading data timeout ms delay
 * Delay for data to arrive within serial buffer.
 */
#define PMS_READ_MSTIMEOUT    100   // ms timeout delay for reading data from serial link (use to be < 40ms)

/* define maximum number of measures
 * over a single campaign.
 */
#define _MAX_MEASURES               (uint8_t)7            // max. is 255
#define _MEASURES_INTERLEAVE_MS     DEFL_READ_MSINTERVAL  // delay between two measures in the 'measuring' state

// Finite state machine
enum class pmsSensorState_t : uint8_t {
  idle            = 0,
  wakeup,               // only when sensor is sleeping
  measures              // reading PM values from serial link
};
#define PMS_FSMSTATE_DEFL     pmsSensorState_t::idle



/*
 * Class
 */
class pms_serial : public generic_driver {
  public:

    // constructor
    pms_serial( void );
    
    // sensor creation via sensOCampus JSON array 'params'
    boolean begin( JsonVariant );
    void powerON( void );
    void powerOFF( void );

    void process( uint16_t coolDown=0,
                  uint8_t decimals=0 );  // override generic:process for our sensor internal processing

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
    PMS *_psensor;                // [ll] pointer to low-level PMS serial sensor
    PMS::DATA _sensor_data[];       // [ll] low-level PMS sensor deserialized data

    uint8_t _link;                // serial link number (e.g 2 --> Serial2)
    unsigned int _link_speed;     // 9600 to 115200 bauds
    uint8_t _enable_gpio;         // PM_ENABLE gpio

    pmsSensorState_t _FSMstatus;  // FSM
    unsigned long _FSMtimerStart; // ms system time start of current state;
                                  // Only relevant when timerDelay is not null
    uint16_t _FSMtimerDelay;      // ms delay to cur state timeout
    
    boolean _initialized;
    static const char *units;
};

#endif /* _PMS_SERIAL_H_ */
