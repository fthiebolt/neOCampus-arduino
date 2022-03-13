/**************************************************************************/
/*! 
    @file     SCD4x.h
    @author   F.Thiebolt
	  @license
	
    This is part of a the neOCampus drivers library.
    SCD4x is an ultrasonic CO2 sensor that also features
    both temperature and hygro on an I2C interface :)
    
    Remember: all transfers are 16bits wide
    
    (c) Copyright 2022 Thiebolt.F <thiebolt@irit.fr>

	@section  HISTORY

    2022-March    - F.Thiebolt Initial release
    
*/
/**************************************************************************/

#ifndef _SCD4X_H_
#define _SCD4X_H_


#include <Arduino.h>

// generic sensor driver
#include "generic_driver.h"



/*
 * Definitions
 */

/* To avoid checking for the end of the priodic measurement process too often
 * we set a min delay between two successives check for data readiness */
#define SCD4X_DATA_CHECK_MS       1000  // ms check data measurement ending

/* In normal operation, periodic measurement leads to new data available very 5s or 30s
 * (normal vs low-power periodic measurement). If no new data is available for the
 * specified tiemout, we (soft) reset the end-device */
#define SCD4X_DATA_TIMEOUT        300   // seconds timeout delay before reseting the chip

// Enable CRC lookup table (regular computation otherwise)
#ifndef SCD4X_CRC_LOOKUP_TABLE
#define SCD4X_CRC_LOOKUP_TABLE    1
#endif

/* scd4x commands
 * Note:
 * Measurement is either 5s or 30s (normal vs low-power modes)
 * It does not start a new measurement till you don't readout the values
 * ==> using automatic measurement ought not to consume more power :s
 * - most commands need a 1ms execution time
 * - CO2, temp and humidity are measured and sent in a single 9bytes frame
 *      16bits C02 + CRC + 16bits T + CRC + 16bits RH + CRC
 * - every 16bits frames are CRC protected
 * - CRC8 0x31, initial=0xFF, final xor=0x00 for 16bits data (first is )
 */
enum class scd4xCmd_t : uint16_t {

  // basic commands
  start_periodic_measurement              = 0x21b1, // execution time: none. 5000ms measurement delay (i.e read_measurement)
  read_measurement                        = 0xec05, // execution time: 1ms
  stop_periodic_measurement               = 0x3f86, // execution time: 500ms

  // output signal compensation
  set_temperature_offset                  = 0x241d, // execution time: 1ms
  get_temperature_offset                  = 0x2318, // execution time: 1ms
  set_sensor_altitude                     = 0x2427, // execution time: 1ms
  get_sensor_altitude                     = 0x2322, // execution time: 1ms
  set_ambiant_pressure                    = 0xe000, // execution time: 1ms

  // field calibration
  perform_forced_calibration              = 0x362f, // execution time: 400ms
  set_automatic_self_calibration_enabled  = 0x2416, // execution time: 1ms
  get_automatic_self_calibration_enabled  = 0x2313, // execution time: 1ms
  
  // low-power operations
  start_low_power_periodoc_measurement    = 0x21ac, // 30s measurement delay
  get_data_ready_status                   = 0xe4b8, // execution time: 1ms
  // stop_periodic_measurement to stop low_power periodic measurement

  // advanced features
  persist_settings                        = 0x3615, // execution time: 800ms
  get_serial_number                       = 0x3682, // execution time: 1ms
  perform_self_test                       = 0x3639, // execution time: 10000ms
  perform_factory_reset                   = 0x3632, // execution time: 1200ms
  reinit                                  = 0x3646, // execution time: 20ms

  // low-power single shot - SCD41 only
  measure_single_shot                     = 0x219d, // execution time: 5000ms
  measure_single_shot_rht_only            = 0x2196, // execution time: 50ms
};

// type of measure
enum class scd4xMeasureType_t : uint8_t {
  co2           = 0x00,
  humidity      = 0x10,
  temperature   = 0x20
};



/*
 * Class
 */
class SCD4x : public generic_driver {
  public:
    SCD4x( scd4xMeasureType_t );
  
    boolean begin( uint8_t );   // start with an i2c address
    void powerON( void );       // switch ON
    void powerOFF( void );      // switch OFF

    // send back sensor's value, units and I2C addr
    boolean acquire( float* );
    const char *sensorUnits( uint8_t=0 );
    String subID( uint8_t=0 ) { return String(_i2caddr); };
    
    // read sensor's values
    boolean getCO2( float* );   // retroieve co2
    boolean getRH( float* );    // retrieve humidity
    boolean getTemp( float* );  // retrieve temperature

    // --- static methods / constants -----------------------

    // list of possibles I2C addrs
    static const uint8_t i2c_addrs[];

    // device detection
    static boolean is_device( uint8_t );

  private:
    // -- private methods
    bool _readSensor( void );                     // low-level function to read sensors values
    bool _isDataReady( void );                    // check is read measurement reached its end
    static void sw_reset( uint8_t );              // reset sensor via software reset procedure
    static bool crc_check( uint8_t[], uint8_t, uint8_t );  // data array, nb_bytes, checksum
    static bool _check_identity( uint8_t );       // check device is what we expect!
    static void _writeCmd( uint8_t, uint16_t );   // write I2C command

    // --- private attributes
    uint8_t _i2caddr;
    scd4xMeasureType_t _measureType;
    static const char *units_co2;
    static const char *units_temp;
    static const char *units_rh;
                                /* CO2, TEMP and RH are read at the same time ==> hence we store them
                                   as shared attributes across all instances */
    static boolean _periodic_measure; // automatic periodic measurement already started or not
    static unsigned long _lastCheck;  // last time we check for data readiness (@periodic measurement)
    static unsigned long _lastData;   // last time a new data arrived from SCD4x device
    static uint16_t _co2_sensor;      // shared across all instances
    static boolean _co2_sensor_valid; // shared across all instances
    static uint16_t _t_sensor;        // shared across all instances
    static boolean _t_sensor_valid;   // shared across all instances
    static uint16_t _rh_sensor;       // shared across all instances
    static boolean _rh_sensor_valid;  // shared across all instances

    // CRC computation
    static const uint8_t _crc8_polynom;    // crc P(x)=x^8+x^5+x^4+1 (0x31) 1.00110001, init=0xFF
#ifdef SCD4X_CRC_LOOKUP_TABLE
    static const uint8_t _crc8_table[256];
#endif /* SCD4X_CRC_LOOKUP_TABLE */
};

#endif /* _SCD4X_H_ */
