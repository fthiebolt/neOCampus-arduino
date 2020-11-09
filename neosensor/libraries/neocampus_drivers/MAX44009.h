/**************************************************************************/
/*! 
    @file     MAX44009.h
    @author   L.Jeanmougin & F.Thiebolt
    @section LICENSE
	  @license
	
    This is part of a the neOCampus drivers library.
    MAX44009 is a luminosity sensor featuring an I2C interface
    It allows measures up to 188.000 lux !!
        
    (c) Copyright 2020 Thiebolt.F <thiebolt@irit.fr>

	@section  HISTORY

    2020-Nov  - F.Thiebolt    integration
    2020-Oct  - L.Jeanmougin  Initial release
*/
/**************************************************************************/

#ifndef _MAX44009_H_
#define _MAX44009_H_

/*
 * Includes
 */
#include <Arduino.h>

// generic sensors driver
#include "generic_driver.h"



/*
 * Definitions
 */
// Registers definition
enum class max44009Regs_t : uint8_t {
  interrupt_status                = 0x00,
  interrupt_enable                = 0x01,
  config                          = 0x02,
  lux_upper                       = 0x03,
  lux_lower                       = 0x04,
  threshold_upper                 = 0x05,
  threshold_lower                 = 0x06,
  threshold_timer                 = 0x07
};

// Register: MAX44009_REG_INTERRUPT_ENABLE
#define MAX44009_INTR_ENABLE        0x01
#define MAX44009_INTR_DISABLE       0x00
#define MAX44008_INTR_MASK          0x01

/* Register: MAX44009_REG_CONFIG
 * Note: when using automatic ranging mode (prefered), no hassle about integration
 * time BUT to properly read LUX high and low bytes BOTH at the SAME TIME, we need
 * i2c with Repeated START command (i.e NO STOP in between) to avoid inconsistency
 * ==> i2c readList( ... ) function
 */
#define MAX44009_AUTO_RANGING       0x03    // automatic mode for ranging and integration :)
#define MAX44009_MANUAL_RANGING     0x43    // manual mode --> requires additional setup (integration time, divider)

enum class max44009IntegrationT_t : uint8_t {
  ms_integrate_800                = 0x00, // 800ms (default) (Lowest power usage)
  ms_integrate_400                = 0x01, // 400ms
  ms_integrate_200                = 0x02, // 200ms
  ms_integrate_100                = 0x03, // 100ms
  ms_integrate_50                 = 0x04, // 50ms
  ms_integrate_25                 = 0x05, // 25ms
  ms_integrate_12                 = 0x06, // 12.5ms
  ms_integrate_6                  = 0x07, // 6.25ms

  ms_integrate_auto               = 0xff
};
#define MAX44009_INTEGRATION_MASK   0x07  //bits [2:0] of config register


/*
 * Remarquable values used in detection mode:
 * typical registers values after Power On Reset
 */
#define MAX44009_REG_CONFIG_DEFL        0x03  // [POR] control register default value
#define MAX44009_REG_CONFIG_DEFL_ALT    0x01  // [POR] control register default alternate value (fakes ??)
#define MAX44009_REG_THRESHOLD_DEFL     0xFF00  // [POR] threshold register default value
#define MAX44009_REG_THRESHOLD_DEFL_ALT 0xEFEF  // [POR] threshold  register default alternate value (fakes ??)



/*
 * Class
 */
class MAX44009 : public generic_driver {
  public:
    MAX44009( void );

    boolean begin( uint8_t );       // start with an i2c address
    // no need to set resolution as it is automated as default

    void powerON( void );       // switch ON
    void powerOFF( void );      // switch OFF

    // send back sensor's value and units (e.g "32,5" "°C", <i2c_addr> )
    boolean acquire( float* );
    const char *sensorUnits( void ) { return units; };
    String subID( void ) { return String(_i2caddr); };

    // MAX44009 specific methods
    boolean setIntegration( max44009IntegrationT_t );

    // --- static methods / constants -----------------------
    
    // list of possibles I2C addrs
    static const uint8_t i2c_addrs[];

    // device detection
    static boolean is_device( uint8_t );

  private:
    // --- private methods
    boolean _getLux( float* );
    static bool _check_identity( uint8_t );   // check device is what we expect!

    // --- private attributes
    uint8_t _i2caddr;
    static const char *units;
    max44009IntegrationT_t _integrationTime;
};

#endif /* _MAX44009_H_ */