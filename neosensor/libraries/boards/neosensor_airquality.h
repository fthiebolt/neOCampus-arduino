/*
 * neOCampus operation
 * 
 * neOSensor AirQuality ESP32 board hardware definitions
 * 
 * This board does not feature any additional I2C sensors as all pins
 * are in use !
 * ---
 * F.Thiebolt   aug.20  initial release
 * 
 */


#ifndef _NEOSENSOR_AIRQUALITY_H_
#define _NEOSENSOR_AIRQUALITY_H_

/* Check module */
#if !defined(ESP32)
#error "WRONG TARGET, ought to be an ESP32!"
#endif /* ESP32 */

/*
 * Includes
 */



/*
 * Definitions
 */
#define BOARD_NAME          "AirQuality"    // no more than 10 chars
#define BOARD_REVISION      1.0
#define BOARD_FWREV         201102          // Firmware revision <year><month><day> in 2 digits each


// define what is an invalid gpio
#define INVALID_GPIO        (uint8_t)(-1)

/* system led ... not present on ESP32 ... it is the blue led on ESP8266 near the antenna */
#ifndef SYS_LED
#define SYS_LED               INVALID_GPIO
#endif

/* main LED ... this board does not feature any led :( */
#ifndef LED
#define LED                   INVALID_GPIO
//#define LED                 2   // GPIO2 (our main led)
#endif

// main output led PWM settings (ESP32 ONLY)
#ifdef ESP32
#define LED_CHANNEL           (uint8_t)0  // PWM controller from 0 to 15
#define LED_RESOLUTION        10          // Timer precision from 1 to 16 bits
#define LED_BASE_FREQ         5000
#endif /* ESP32 */

/* Clear switch ... this board does not feature a clear switch :( */
#ifndef CLEAR_SW
#define CLEAR_SW              INVALID_GPIO
//#define CLEAR_SW            LED
#endif

/* i2c bus related definition ... not on this board
 * note: this board already makes use of SDA & SCL pins
 * thus enabling I2C means you know what you're doing ;)
 */
#ifdef DISABLE_I2C
  #define SDA                 INVALID_GPIO
  #define SCL                 INVALID_GPIO
#else
  #define SDA                 21
  #define SCL                 22
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


#endif /* _NEOSENSOR_AIRQUALITY_H_ */

