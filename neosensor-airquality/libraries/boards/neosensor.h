/*
 * neOCampus operation
 * 
 * neOSensor ESP8266/ESP32 hardware board definitions
 * 
 * 
 * ---
 * The first version of the neOSensor board features an ESP8266 along
 * with i2c sensors like:
 *  - luminosity (TSL2561)
 *  - temperature (MCP9808)
 *  - noise detector (8bits DAC MCP4706)
 * ---
 *
 * F.Thiebolt   Aug.20  added definitions for future ESP32 version (e.g LED PWM channel)
 * F.Thiebolt   Jun.18  merged neOClock as options to neOSensor
 * F.Thiebolt   May.18  set GPIO2 for embedded led
 * F.Thiebolt   Sep.17  added microswitch on NOISE_LED output to read at startup if a config reset is required
 *                      replaced MCP4706 DAC with MCP47FEB01A0 (because it's impossible to read MCP4706 status via i2c)
 * F.Thiebolt   July17  initial release
 * 
 */


#ifndef _NEOSENSOR_H_
#define _NEOSENSOR_H_

/*
 * Includes
 */



/*
 * Definitions
 */
#define BOARD_NAME          "neOSensor"
#define BOARD_REVISION      1.1
#define BOARD_FWREV         200608          // Firmware revision <year><month><day> in 2 digits each


/* #############################################################################
 * ###                                                                       ###
 * ###             On-board PCB temperature compensation                     ###
 * ###                                                                       ###
 * 
 *      [Mar.18] to enable temperature correction on i2c temperature device
 *      whose i2c addr is the last one, please activate following define.
 * 
 * ############################################################################# */
#define TEMPERATURE_CORRECTION_LASTI2C  (float)(-0.875)

// define what is an invalid gpio
#define INVALID_GPIO        (uint8_t)(-1)

/* system led (on ESP8266 module)
 * - LED_BUILTIN is GPIO16 on nodeMCU boards
 * - on-board blue led of ESP-12 is GPIO 2
 * ... thus it is better to set GPIO2 directly
 */
//#define SYS_LED           LED_BUILTIN   // GPIO2 (ESP8266 embedded led)
#ifndef SYS_LED
#define SYS_LED             2             // GPIO2 (ESP8266 embedded led)
#endif

// main output led
#ifndef LED
#define LED                 5   // GPIO5 (our main led)
#endif

// main output led PWM settings (ESP32 ONLY)
#ifdef ESP32
#define LED_CHANNEL         (uint8_t)0  // PWM controller from 0 to 15
#define LED_RESOLUTION      10          // Timer precision from 1 to 16 bits
#define LED_BASE_FREQ       5000
#endif /* ESP32 */

// [Sep.17] reset microswitch is connected to the NOISE_LED output
#ifndef CLEAR_SW
#define CLEAR_SW            LED
#endif

// i2c bus related definition
/* [Jun.18] we observed some unusual values for temperature
 * ... probably glitches that may get corrected with PCB v4
 * In the meanwhile, we slowdown the i2c clock.
 */
//#define I2C_FREQ            50000   // default is 100KHz
#define SDA                 14      // GPIO14  
#define SCL                 12      // GPIO12

// noise related definitions
#define NOISE_LED           LED     // led that will notify if noise has been detected (same as main led)
#define NOISE_DETECT        13      // pin associated to the noise detection subsystem

// PIR sensor
#define PIR_SENSOR          4       // infrared detector


/* #############################################################################
 * ###                                                                       ###
 * ###                   neOClock extension to neOSensor                     ###
 * ###                                                                       ###
 * 
 *      [Jun.18] we extend neOSensor features with neOClock display support
 * 
 * ############################################################################# */

// TM1637 based neOClock
#define NEOCLOCK_TM1637_DIO     2
#define NEOCLOCK_TM1637_CLK     SCL


#endif /* _NEOSENSOR_H_ */
