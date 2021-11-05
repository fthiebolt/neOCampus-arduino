/**************************************************************************/
/*! 
    @file     SHT3x.h
    @author   F. Thiebolt
	  @license
	
    This is part of a the neOCampus drivers library.
    SHT3x is humidity + temperature sensor with an I2C interface
    
    Remember: all transfers are 16bits wide
    
    (c) Copyright 2020 Thiebolt.F <thiebolt@irit.fr>

	@section  HISTORY

    2020-May    - F.Thiebolt Initial release
    
*/
/**************************************************************************/

#ifndef _SHT3X_H_
#define _SHT3X_H_


#include <Arduino.h>

// generic sensor driver
#include "generic_driver.h"



/*
 * Definitions
 */
/* SHT3x sensors send back both T and RH at the same time, but since
 * we'll have both instances of this class ==> we implement a cache to
 * avoid reading the sensors two times one for T then for RH */
#define SHT3X_SENSOR_CACHE_MS       5000  // ms caches values validity

// Enable CRC lookup table (regular computation otherwise)
#ifndef SHT3X_CRC_LOOKUP_TABLE
#define SHT3X_CRC_LOOKUP_TABLE    1
#endif

/* sht3x commands
 * Note:
 * - we won't make use of the 'Periodic Mode' features (i.e continuous measurement)
 * - avoid stretch modes because measuremernt can take up to 16ms
 * - useless heater
 * - Repeatability is resolution
 * - 1ms min. delay between two commands
 * - both temperature and humidity are measured and sent in a single 6bytes frame
 *      16bits T + CRC + 16bits RH + CRC
 * - every 16bits frames are CRC protected
 * - CRC8 0x31, initial=0xFF for 16bits data (first is )
 */
enum class sht3xCmd_t : uint16_t {
  soft_reset                = 0X30A2,   // software reset
  heater_enable             = 0x306D,   // on-chip heater enable
  heater_disable            = 0x3066,   // on-chip heater disable
  get_status                = 0xF32D,   // read Status Register
  clear_status              = 0x3041,   // clear Status Register
  meas_highRes_stretch      = 0x2C06,   // Measurement High Repeatability (resolution) with Clock Stretch Enabled
  meas_medRes_stretch       = 0x2C0D,   // Measurement Medium Repeatability (resolution) with Clock Stretch Enabled
  meas_lowRes_stretch       = 0x2C10,   // Measurement Low Repeatability (resolution) with Clock Stretch Enabled
  meas_highRes              = 0x2400,   // Measurement High Repeatability (resolution) WITHOUT clock stretching
  meas_medRes               = 0x240B,   // Measurement Medium Repeatability (resolution) WITHOUT clock stretching
  meas_lowRes               = 0x2416,   // Measurement Low Repeatability (resolution) WITHOUT clock stretching
};

// status
#define SHT3X_STATUS_REG_MASK   0xFC1F  // removed bit 9:5 because of their unspecified value
#define SHT3X_STATUS_REG_DEFL   0x8010  // expected default value (apply MASK for value checking)

// type of measure
enum class sht3xMeasureType_t : uint8_t {
  humidity      = 0x10,
  temperature   = 0x20
};

// resolution
enum class sht3xResolution_t : uint8_t {
  high_res,
  medium_res,
  low_res
};
#define SHT3X_DEFL_RESOLUTION   sht3xResolution_t::high_res

// integration time (ms)
enum class sht3xIntegration_t : uint8_t {
  ms_integrate_high     = 16,   // integraion time (ms) for high resolution measure
  ms_integrate_medium   = 7,    // integraion time (ms) for medium resolution measure
  ms_integrate_low      = 5     // integraion time (ms) for low resolution measure
};

#define SHT3X_INTEGRATION_TIME_CTE    5 // additionnal ms delay to all timings (total <= 255 ---uint8_t)



/*
 * Class
 */
class SHT3x : public generic_driver {
  public:
    SHT3x( sht3xMeasureType_t );
  
    boolean begin( uint8_t );       // start with an i2c address
    bool setResolution( sht3xResolution_t );
    void powerON( void );       // switch ON
    void powerOFF( void );      // switch OFF

    // send back sensor's value, units and I2C addr
    boolean acquire( float* );
    const char *sensorUnits( uint8_t=0 );
    String subID( uint8_t=0 ) { return String(_i2caddr); };
    
    // read sensor's values
    boolean getRH( float* );    // retrieve humidity
    boolean getTemp( float* );  // retrieve temperature

    // --- static methods / constants -----------------------
    
    // list of possibles I2C addrs
    static const uint8_t i2c_addrs[];

    // device detection
    static boolean is_device( uint8_t );
    
  private:
    // -- private methods
    bool _readSensor( uint16_t* );                // low-level function to read value registers
    static void sw_reset( uint8_t );              // reset sensor via software reset procedure
    static bool crc_check( uint8_t[], uint8_t, uint8_t );  // data array, nb_bytes, checksum
    static bool _check_identity( uint8_t );       // check device is what we expect!
    static void _writeCmd( uint8_t, uint16_t );   // write I2C command

    // --- private attributes
    uint8_t _i2caddr;
    sht3xResolution_t _resolution;
    sht3xMeasureType_t _measureType;
    static const char *_t_units;
    static const char *_rh_units;
    uint8_t _integrationTime;   // ms time to integrate a measure (for non continuous mode)
                                /* both TEMP and RH are read at the same time ==> hence we store them
                                   as shared attributes across all instances */
    static unsigned long _lastMsRead;   // last time data have been read (elapsed ms from beginning)
    static uint16_t _t_sensor;  // shared across all instances (for both humidity and temperature modules)
    static uint16_t _rh_sensor; // shared across all instances (for both humidity and temperature modules)

    // CRC computation
    static const uint8_t _crc8_polynom;    // crc P(x)=x^8+x^5+x^4+1 (0x31) 1.00110001, init=0xFF
#ifdef SHT3X_CRC_LOOKUP_TABLE
    static const uint8_t _crc8_table[256];
#endif /* SHT3X_CRC_LOOKUP_TABLE */
};

#endif /* _SHT3X_H_ */
