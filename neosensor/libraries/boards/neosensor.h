/*
 * (c) neOCampus operation / Laboratoire IRIT / Université Toulouse 3
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
 * F.Thiebolt   Nov.21  esp32 added serial link definition
 * F.Thiebolt   Jul.21  esp32 added PIR sensor @ GPIO4
 *                      esp32 added switch INCR_SW @ GPIO18
 *                      esp32 added switch DECR_SW @ GPIO23
 * F.Thiebolt   Mar.21  corrected leds definitions for neOSensor V5 :)
 * F.Thiebolt   Aug.20  added definitions for future ESP32 version (e.g LED PWM channel)
 *                      set default ADC resolution according to architecture
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
#define BOARD_REVISION      1.2
#define BOARD_FWREV         220316          // Firmware revision <year><month><day> in 2 digits each


/* #############################################################################
 * ###                                                                       ###
 * ###             On-board PCB temperature compensation                     ###
 * ###                                                                       ###
 *      [Oct.21] REMOVING TEMPERATURE CORRECTION FOR LATEST I2C temp. sensor
 *      [Mar.18] to enable temperature correction on i2c temperature device
 *      whose i2c addr is the last one, please activate following define.
 * 
 * #############################################################################
#ifndef TEMPERATURE_CORRECTION_LASTI2C
#define TEMPERATURE_CORRECTION_LASTI2C  (float)(-0.875)
#endif // TEMPERATURE_CORRECTION_LASTI2C
 */

// define what is an invalid gpio
#define INVALID_GPIO        (uint8_t)(-1)
// define what is an invalid serial link
#define INVALID_SERIAL_LINK (uint8_t)(-1)


/* system led (on ESP8266 module)
 * - LED_BUILTIN is GPIO16 on nodeMCU boards
 * - on-board blue led of ESP-12 is GPIO 2
 * ... thus it is better to set GPIO2 directly
 */
//#define SYS_LED           LED_BUILTIN   // GPIO2 (ESP8266 embedded led)
#ifndef SYS_LED
  #ifdef ESP8266
    #define SYS_LED         2             // GPIO2 (ESP8266 embedded led)
  #elif defined(ESP32)
    #define SYS_LED         2             // GPIO2 (ESP32 30pins & green devkit)
//  #define SYS_LED         INVALID_GPIO  // no system led on ESP32 38 pins
  #endif
#endif

// main output led
#ifndef LED
  #ifdef ESP8266
    #define LED             5   // GPIO5 (our main led + clear switch)
  #elif defined(ESP32)
    #define LED             15  // GPIO15 (our main led + clear switch)
                                // WARNING: GPIO5 on esp32 devkit V1 has 10k pull-up on 3v3
//  #define LED             INVALID_GPIO
  #endif
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

// [jul.21] neOSensor V5 (esp32) and up feature two additional switches
#ifdef ESP32
#define INCR_SW             18
#define DECR_SW             23
#endif /* ESP32 */

// i2c bus related definition
/* [Jun.18] we observed some unusual values for temperature
 * ... probably glitches that may get corrected with PCB v4
 * In the meanwhile, we slowdown the i2c clock.
 */
#ifdef DISABLE_I2C
  #error "I2C bus cannot be disabled on this board, check your compilation flags!"
#elif defined(ESP8266)
  //#define I2C_FREQ          50000   // default is 100KHz
  #define SDA               14      // GPIO14  
  #define SCL               12      // GPIO12
#elif defined(ESP32)
  //#define I2C_FREQ          50000   // default is 100KHz
  #define SDA               21
  #define SCL               22
#endif

// noise related definitions
#ifndef NOISE_LED
#define NOISE_LED           LED     // led that will notify if noise has been detected (same as main led)
#endif
#ifndef NOISE_DETECT
  #ifdef ESP8266
    #define NOISE_DETECT    13      // pin associated to the noise detection subsystem
  #elif defined(ESP32)
    #define NOISE_DETECT    INVALID_GPIO
  #endif
#endif

/* PIR sensor
 * [aug.20] TODO: configure PIR sensor through sensOCampus JSON config
 */
#ifndef PIR_SENSOR
  #ifdef ESP8266
    #define PIR_SENSOR      4       // infrared detector
  #elif defined(ESP32)
    #define PIR_SENSOR      4       // [jul.21] neOSensor V5 added PIR
  #endif
#endif

/* Sensors/Actuators serial link
 * Note: added neOSensors V5, esp8266 not available
 */
#ifndef SENSORS_SERIAL_LINK
  #ifdef ESP32
    #define SENSORS_SERIAL_LINK     (uint8_t)2   // [jul.21] neOSensor V5 added (i.e Serial2)
  #else
    #define SENSORS_SERIAL_LINK     INVALID_SERIAL_LINK
  #endif
#endif

/* PM_ENABLE
 * this gpio may give additional control over serially connected sensors
 */
#ifndef PM_ENABLE
  #ifdef ESP8266
    #define PM_ENABLE       INVALID_GPIO
  #elif defined(ESP32)
    #define PM_ENABLE       5       // [jul.21] neOSensor V5 added
  #endif
#endif

/* ADC resolution:
 * noisy ADC on ESP32 thus downgrading to 11 bits
 */
#ifndef ADC_RESOLUTION
#if ESP32
 #ifndef DISABLE_ADC_CAL
  #define ADC_RESOLUTION      ADC_WIDTH_BIT_11
 #else
  #define ADC_RESOLUTION      11
 #endif /* DISABLE_ADC_CAL */
#elif ESP8266
  #define ADC_RESOLUTION      10  // default on esp8266
#endif
#endif /* ADC_RESOLUTION */


/* #############################################################################
 * ###                                                                       ###
 * ###                   neOClock extension to neOSensor                     ###
 * ###                                                                       ###
 * 
 *      [Jun.18] we extend neOSensor features with neOClock display support
 * 
 * ############################################################################# */

/* TM1637 based neOClock
 * [aug.20] TODO: remove such hard defs and set config through sensOCampus JSON
 */
#ifdef ESP8266
  #define NEOCLOCK_TM1637_DIO   2
  #define NEOCLOCK_TM1637_CLK   SCL
#endif

#endif /* _NEOSENSOR_H_ */

