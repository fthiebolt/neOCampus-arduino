/**************************************************************************/
/*! 
    @file     Adafruit_MCP9808.h
    @author   K. Townsend (Adafruit Industries)
	@license  BSD (see license.txt)
	
	This is a library for the Adafruit MCP9808 Temp Sensor breakout board
	----> http://www.adafruit.com/products/1782
	
	Adafruit invests time and resources providing this open source code, 
	please support Adafruit and open-source hardware by purchasing 
	products from Adafruit!

	@section  HISTORY

    2017  - adapter for neOCampus
	  v1.2  - Add support for low power operations
    v1.1  - Added list of possible I2C address
    v1.0  - First release
  ----------------------------------------------------------------------

*/

/**************************************************************************/

#ifndef _ADAFRUIT_MCP9808_H
#define _ADAFRUIT_MCP9808_H

/*
 * Includes
 */
#include <Arduino.h>

// generic sensors driver
#include "generic_driver.h"



/*
 * Definitions
 */
//#define MCP9808_I2CADDR_DEFAULT         0x18
#define MCP9808_REG_CONFIG              0x01
#define MCP9808_REG_UPPER_TEMP          0x02
#define MCP9808_REG_LOWER_TEMP          0x03
#define MCP9808_REG_CRIT_TEMP           0x04
#define MCP9808_REG_AMBIENT_TEMP        0x05
#define MCP9808_REG_MANUF_ID            0x06
#define MCP9808_REG_DEVICE_ID           0x07
#define MCP9808_REG_RESOLUTION          0x08

#define MCP9808_REG_CONFIG_SHUTDOWN     0x0100
#define MCP9808_REG_CONFIG_CRITLOCKED   0x0080
#define MCP9808_REG_CONFIG_WINLOCKED    0x0040
#define MCP9808_REG_CONFIG_INTCLR       0x0020
#define MCP9808_REG_CONFIG_ALERTSTAT    0x0010
#define MCP9808_REG_CONFIG_ALERTCTRL    0x0008
#define MCP9808_REG_CONFIG_ALERTSEL     0x0004
#define MCP9808_REG_CONFIG_ALERTPOL     0x0002
#define MCP9808_REG_CONFIG_ALERTMODE    0x0001

#define MCP9808_MANUFACTURER_ID         0x0054  // 16 bits value
#define MCP9808_DEVICE_ID               0x04    // 8 bits value (next 8bits are revision number)

#define MCP9808_INTEGRATIONTIME_05      30      // ms for 0.5°c resolution
#define MCP9808_INTEGRATIONTIME_025     65      // ms for 0.25°c resolution
#define MCP9808_INTEGRATIONTIME_0125    130     // ms for 0.125°c resolution
#define MCP9808_INTEGRATIONTIME_00625   250     // ms for 0.0625°c resolution
#define MCP9808_INTEGRATION_TIME_CTE    5       // additionnal ms delay to all timings (total <= 255 ---uint8_t)

typedef enum {
  MCP9808_RESOLUTION_05DEG              = 0x00,
  MCP9808_RESOLUTION_025DEG             = 0x01,
  MCP9808_RESOLUTION_0125DEG            = 0x02,
  MCP9808_RESOLUTION_00625DEG           = 0x03
} mcp9808Resolution_t;

// set MCP9808 default resolution
#define MCP9808_DEFL_RESOLUTION         MCP9808_RESOLUTION_0125DEG



/*
 * Class
 */
class Adafruit_MCP9808 : public generic_driver {
  public:
    Adafruit_MCP9808( void );
    
    boolean begin( uint8_t );       // start with an i2c address
    bool setResolution( mcp9808Resolution_t );
    void powerON( void );       // switch ON
    void powerOFF( void );      // switch OFF

    // send back sensor's value and units (e.g "32,5" "°C", <i2c_addr> )
    boolean acquire( float* );
    const char *sensorUnits( void ) { return units; };
    String subID( void ) { return String(_i2caddr); };
    
    // --- static methods / constants -----------------------
    
    // list of possibles I2C addrs
    static const uint8_t i2c_addrs[];

    // device detection
    static boolean is_device( uint8_t );
    
  private:
    // attributes
    uint8_t _i2caddr;
    mcp9808Resolution_t _resolution;
    uint8_t _integrationTime; // time to integrate a measure (for non continuous mode)
    static const char *units;

    // methods ...
    static bool _check_identity( uint8_t );   // check device is what we expect!
    void _shutdown_wake( uint8_t );
};

#endif /* _ADAFRUIT_MCP9808_H */
