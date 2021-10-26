/**************************************************************************/
/*! 
    @file     SHT2x.h
    @author   F. Thiebolt
	  @license
	
    This is part of a the neOCampus drivers library.
    SHT2x is humidity + temperature sensor with an I2C interface
    
    Remember: all transfers are 16bits wide
    
    (c) Copyright 2020 Thiebolt.F <thiebolt@irit.fr>

	@section  HISTORY

    2020-May    - F.Thiebolt Initial release
    
*/
/**************************************************************************/

#ifndef _SHT2X_H_
#define _SHT2X_H_


#include <Arduino.h>

// generic sensor driver
#include "generic_driver.h"



/*
 * Definitions
 */

// Enable CRC lookup table (regular computation otherwise)
#define SHT2X_CRC_LOOKUP_TABLE    1

/* enable UUID's CRC check
 * WARNING: some fakes devices does not support it!
 */
#ifndef SHT2X_CRC_UUID_DISABLED
#define SHT2X_CRC_UUID_DISABLED   1
#endif

// sht2x commands
enum class sht2xCmd_t : uint8_t {
  get_temp_hm     = 0XE3, // command temperature measure, hold master
  get_rh_hm       = 0XE5, // command humidity measure, hold master
  get_temp        = 0XF3, // command temperature measure
  get_rh          = 0XF5, // command humidity measure
  set_user_reg    = 0XE6,	// command writing user register
  get_user_reg    = 0XE7,	// command reading user register
  soft_reset      = 0XFE  // command soft reset
};

// type of measure
enum class sht2xMeasureType_t : uint8_t {
  humidity      = 0x10,
  temperature   = 0x20
};

// resolution
enum class sht2xResolution_t : uint8_t {
  res_12_14bits     = 0x00,   // RH:12bits    T:14bits (default)
  res_8_12bits      = 0x01,   // RH:8bits     T:12bits
  res_10_13bits     = 0x80,   // RH:10bits    T:13bits
  res_11_11bits     = 0x81,   // RH:11bits    T:11bits
  res_mask          = 0x81
};
#define SHT2X_DEFL_RESOLUTION   sht2xResolution_t::res_12_14bits

// integration time (ms) for RH sensor
enum class sht2xIntegrationRH_t : uint8_t {
  ms_integrate_8bits    = 4,
  ms_integrate_10bits   = 9,
  ms_integrate_11bits   = 15,
  ms_integrate_12bits   = 29
};

// integration time (ms) for T sensor
enum class sht2xIntegrationT_t : uint8_t {
  ms_integrate_11bits   = 11,
  ms_integrate_12bits   = 22,
  ms_integrate_13bits   = 43,
  ms_integrate_14bits   = 85
};

#define SHT2X_INTEGRATION_TIME_CTE    5 // additionnal ms delay to all timings (total <= 255 ---uint8_t)



/*
 * Class
 */
class SHT2x : public generic_driver {
  public:
    SHT2x( sht2xMeasureType_t );
  
    boolean begin( uint8_t );       // start with an i2c address
    bool setResolution( sht2xResolution_t );
    void powerON( void );       // switch ON
    void powerOFF( void );      // switch OFF

    // send back sensor's value, units and I2C addr
    boolean acquire( float* );
    const char *sensorUnits( void );
    String subID( void ) { return String(_i2caddr); };
    
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
    bool _readSensor( sht2xCmd_t, uint16_t* );    // low-level function to read value registers
    static void sw_reset( uint8_t );                       // reset sensor via software reset procedure
    static bool crc_check( uint8_t[], uint8_t, uint8_t );  // data array, nb_bytes, checksum
    static uint64_t getSerialNumber( uint8_t );   // retrieve sensor's UUID
    static bool _check_identity( uint8_t );       // check device is what we expect!

    // --- private attributes
    uint8_t _i2caddr;
    sht2xResolution_t _resolution;
    sht2xMeasureType_t _measureType;
    static const char *_t_units;
    static const char *_rh_units;
    uint8_t _integrationTime; // ms time to integrate a measure (for non continuous mode)

    // CRC computation
    static const uint8_t _crc8_polynom;     // crc P(x)=x^8+x^5+x^4+1 (0x31) 1.00110001
#ifdef SHT2X_CRC_LOOKUP_TABLE
    static const uint8_t _crc8_table[256];
#endif /* SHT2X_CRC_LOOKUP_TABLE */
};

#endif /* _SHT2X_H_ */
