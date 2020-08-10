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
#define BOARD_NAME          "neOSensor-AirQuality"
#define BOARD_REVISION      1.0
#define BOARD_FWREV         200901          // Firmware revision <year><month><day> in 2 digits each


// define what is an invalid gpio
#define INVALID_GPIO        (uint8_t)(-1)

/* system led ... not present on ESP32 ... it is the blue led on ESP8266 near the antenna */
#define SYS_LED               INVALID_GPIO

/* main LED ... this board does not feature any led :( */
#define LED                   INVALID_GPIO
//#define LED                 2   // GPIO2 (our main led)

/* Clear switch ... this board does not feature a clear switch :( */
#define CLEAR_SW              INVALID_GPIO
//#define CLEAR_SW            LED


// i2c bus related definition ... not on this board
#define SDA                 INVALID_GPIO
#define SCL                 INVALID_GPIO

#endif /* _NEOSENSOR_AIRQUALITY_H_ */

