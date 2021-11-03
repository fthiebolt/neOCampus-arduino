/**************************************************************************/
/*! 
    @file     lcc_sensor.h
    @author   F. Thiebolt
	  @license
	
    This is part of a the neOCampus drivers library.
    This driver is intended to the CNRS' LCC air quality monitoring board
    
    (c) Copyright 2020 Thiebolt F. <thiebolt@irit.fr>

	@section  HISTORY

    2020-Aug    - First release, F.Thiebolt
    
*/
/**************************************************************************/

#ifndef _LCC_SENSOR_H_
#define _LCC_SENSOR_H_


#include <Arduino.h>
#include <ArduinoJson.h>

#include "sensocampus.h"

// generic sensor driver
#include "generic_driver.h"


/*
 * LCC_SENSOR general principle:
 * - an AOP provides an analog value to enable retrieval of the data
 * - the value from the micro-electronic sensor needs to get amplified: 4 possibles gains
 *    through 4 resistors activated by 4 outputs will allow us to reach a correct voltage
 *    read.
 * - we'll iterate over the 4 possibles gains to maximize the relevance of the data while
 *    avoiding at the same time to go further a voltage threshold to avoid non-linearity.
 * - each time you change the amplification, you need to wait for a delay (e.g 1000ms);
 *    such delay is not acceptable regarding the others sensors that will get stuck so we
 *    need to set a FSM.
 * - 2 sensors feature a 'pulse' mode measurement: through a dedicated output driving a
 *    MOS transistor, we'll heat the activa surface of the sensor for 30 to 60 seconds!!
 *    then we also need another FSM.
 * 
 * Regarding the huge delays between each operation, we need to implement a continuous
 *  measurement system driven the loop delay itself.
 */

/*
 * Definitions
 */
/* define GPIO INPUTS' roles associated with _inputs[]
 * inputs[0-3] gpio to drive the resistors to select the proper gain to the AOP
 * inputs[4] is the AOP to read
 */
enum {
  LCC_SENSOR_10K = 0,      // minimum GAIN
  LCC_SENSOR_100K,
  LCC_SENSOR_1M,
  LCC_SENSOR_10M,          // maximum GAIN
  LCC_SENSOR_ANALOG,
  LCC_SENSOR_LAST_INPUT
};
#define LCC_SENSOR_GAIN_NONE        (uint8_t)(-1)
#define LCC_SENSOR_GAIN_MIN         (uint8_t)LCC_SENSOR_10K
#define LCC_SENSOR_GAIN_MAX         (uint8_t)LCC_SENSOR_10M
/* to compute Resistor associated to current gain */
#define LCC_SENSOR_RBASE            10000
#define LCC_SENSOR_RFACTOR          10

/* integration time:
 * Whenever GAIN is changing, there's 'integration delay' to be taken
 * into account.
 */
#ifndef LCC_SENSOR_INTEGRATION_MS
#define LCC_SENSOR_INTEGRATION_MS   1000  // ms
#endif /* LCC_SENSOR_INTEGRATION_MS */

/* Note about ESP32 ADC linearity: we'll activate various gains from
 * LCC_SENSOR_GAIN_MAX downto LCC_SENSOR_GAIN_MIN till the retrieved value
 * is below the voltage threshold.
 */
#ifndef LCC_SENSOR_MVTH
#define LCC_SENSOR_MVTH             (uint32_t)3000  // ADC mv threshold
#endif /* LCC_SENSOR_MVTH */

/* heater delay */
#ifndef LCC_SENSOR_HEATER_MS
#define LCC_SENSOR_HEATER_MS        30000   // ms. max is 65535
#endif /* LCC_SENSOR_HEATER_MS */

// Finite state machine
enum class lccSensorState_t : uint8_t {
  idle            = 0,
  heating,              // only if heater_gpio available
  auto_gain,            // auto select proper AOP amplification
  measuring,            // adc reading
  wait4read             // waiting for data to get sent to neOCampus
};
#define LCC_SENSOR_STATE_DEFL       lccSensorState_t::idle


/* define maximum number of measures
 * over a single campaign.
 */
#define LCC_MAX_MEASURES            (uint8_t)5    // max. is 255
#define LCC_MEASURES_INTERLEAVE_MS  450           // delay between two measures in the 'measuring' state



/*
 * Class
 */
class lcc_sensor : public generic_driver {
  public:

    // constructor
    lcc_sensor( void );
    
    // sensor creation via sensOCampus JSON array 'params'
    boolean begin( JsonVariant );
    //void powerON( void );       // [aug.20] the board does feature any way to switch power
    //void powerOFF( void );      // [aug.20] the board does feature any way to switch power
    void process( void );         // sensor internal processing

    // send back sensor's value, units and subID
    boolean acquire( float* );
    const char *sensorUnits( uint8_t *idx=nullptr ) { return units; };
    String subID( uint8_t *idx=nullptr ) { return _subID; };

  // --- protected methods / attributes ---------------------
  // --- i.e subclass have direct access to
  protected:
    // -- private/protected methods
    boolean heaterStart( uint16_t=LCC_SENSOR_HEATER_MS );
    boolean heaterBusy( void );

    boolean autoGainStart( uint16_t=LCC_SENSOR_INTEGRATION_MS );
    boolean autoGainBusy( uint16_t=LCC_SENSOR_INTEGRATION_MS );

    boolean measureStart( void );
    boolean measureBusy( void );

    boolean readSensor_mv( uint32_t* );   // internal ADC read; sends back voltage_mv
    float calculatePPM( uint32_t );       // convert mv voltage to PPM concentration
  
    boolean _init( void );          // low-level init
    void _reset_gpio( void );       // set GPIOs at initial state
    boolean _decreaseGain( void );  // decrease current gain

    // --- private/protected attributes
    char _subID[SENSO_SUBID_MAXSIZE];
    uint8_t _adc_resolution;      // nb bits current ADC resolution
    uint16_t _adc_voltageRef;
    uint8_t _inputs[LCC_SENSOR_LAST_INPUT];
    uint8_t _heater_gpio;         // GPIO PIN to start heating the sensor
    uint8_t _cur_gain;            // currently selected Resistor to AOP input

    uint8_t _nb_measures;                 // current number of measures
    uint32_t _measures[LCC_MAX_MEASURES]; // currently measured mv

    lccSensorState_t _FSMstatus;  // FSM
    unsigned long _FSMtimerStart; // ms system time start of current state;
                                  // Only relevant when timerDelay is not null
    uint16_t _FSMtimerDelay;      // ms delay to cur state timeout
    
    boolean _initialized;
    static const char *units;
};

#endif /* _LCC_SENSOR_H_ */
