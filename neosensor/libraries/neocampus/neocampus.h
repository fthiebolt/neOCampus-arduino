/*
 * neOCampus operation
 * 
 * neOCampus *all-in-one* base definitions
 * 
 * ---
 * TODO:
 * ---
 * F.Thiebolt   jan.23  MAX listening TCP connexion is defined at lwip compile time
 * F.Thiebolt   aug.21  added support for COOLDOWN approach
 * F.Thiebolt   Aug.20  automatic board selection according to the compilation flag
 *                      named XXX_BOARD (e.g NEOSENSOR_BOARD)
 * F.Thiebolt   Nov.19  add MQTT socket timeout definition
 *                      add WIFI_AP_MODE_CREDENTIALS
 * F.Thiebolt   Jun.18  start to merge boards types
 *                      manage sensocampus sandbox through wifi parameters options
 * Thiebolt F. July 17  initial release
 * 
 */


#ifndef _NEOCAMPUS_H_
#define _NEOCAMPUS_H_

/*
 * Includes
 */

#include <Arduino.h>

#include "log_levels.h"


/*
 * Generic types definitions
 */
typedef void (*voidFuncPtr)(void);
typedef uint32_t firmwareRev_t;


/*
 * Get max simultaneous listening TCP connections
 */
#if defined(ESP8266)
  #define MAX_TCP_CONNECTIONS       MEMP_NUM_TCP_PCB_LISTEN
#elif defined(ESP32)
  #define MAX_TCP_CONNECTIONS       CONFIG_LWIP_MAX_LISTENING_TCP
#endif


/*
 * Main loop() delay:
 * each loop() iteration is delayed for such value
 */
#ifndef MAIN_LOOP_DELAY
#define MAIN_LOOP_DELAY           250   // ms
#endif /* MAIN_LOOP_DELAY */


/*
 * Cooldown settings (seconds)
 *  is the maximum delay between two consecutive sending event if there's 
 *  nothing new to send: this is a health check of the whole sensor
 */
#ifndef _MAX_COOLDOWN_DEVICE
#define _MAX_COOLDOWN_DEVICE      (3600*6)  // seconds --> 6 hours
#endif /* _MAX_COOLDOWN_DEVICE */

#ifndef _MAX_COOLDOWN_SENSOR
#define _MAX_COOLDOWN_SENSOR      1800      // seconds --> 30mn
#endif /* _MAX_COOLDOWN_SENSOR */

#ifndef _MAX_COOLDOWN_ACTUATOR
#define _MAX_COOLDOWN_ACTUATOR    1800      // seconds --> 30mn
#endif /* _MAX_COOLDOWN_ACTUATOR */



/*
 * >>>                 <<<
 * >>> BOARD SELECTION <<<
 * >>>                 <<<
 * 
 * Select proper board definition to include
 */
#if defined(NEOSENSOR_BOARD)
    #include "neosensor.h"  // for BOTH esp8266, esp32
#elif defined(NEOSENSOR_AIRQUALITY_BOARD)
  #include "neosensor_airquality.h" // only for esp32
  #error "[nov.21] LCC_sensor OUGHT to implement advanced data retrieval with _dataIdx pointer !!";
#else
  #error "unknown specified board ?!?!"
#endif /* NEOSENSOR_BOARD */




/*
 * LOGS definitions
 */
// set neOCampus logging level (min. is 1)
#ifndef LOG_LEVEL
#define LOG_LEVEL               LOG_LVL_DEBUG
#endif /* LOG_LEVEL */

/* I2C low-level msg debug
 * WARNING: display a lot of information about I2C messages! */
//#define DEBUG_I2C

// WiFi Manager debug mode on/off (default is on)
#define DEBUG_WIFI_MANAGER

/* MQTT low-level msg debug
 * WARNING: display a lot of information about MQTT messages! */
#define MQTT_LOWLEVEL_DEBUG




/*
 * SensOCampus
 */
/*
 * WARNING: TESTING PURPOSES ONLY
 * Activating definition below means that sensOCampus 
 * won't get contacted thus using defaults (i.e neOCampus sandbox)
 *
 * [Jun.18] removed in favour of customizable WiFiManager option :)
 * 
 * #define DISABLE_SENSOCAMPUS             // comment out to activate sensOCampus (normal mode)
 *
 */
// sensOCampus credentials & config related definitions
#define MQTT_SERVER_NAME_LENGTH         64    // max length of MQTT server name
#define MQTT_BASE_TOPIC_LENGTH          64    // max length of MQTT base topic (i.e building/room)
#define SENSO_MQTT_LOGIN_LENGTH         32    // max length of sensOCampus MQTT login
#define SENSO_MQTT_PASSWD_LENGTH        64    // max length of sensOCampus MQTT password

// UnitID & subID
#define INVALID_I2CADDR                 (uint8_t)(-1)     // used to send back an invalid i2c addr as subID

// defaults values for testing purposes
#define DEFL_MQTT_SERVER                "neocampus.univ-tlse3.fr"
#ifdef DISABLE_SSL
  #define DEFL_MQTT_STD_PORT            1883            // MQTT
  #define DEFL_MQTT_ABROAD_PORT         10883           // MQTT for abroad access
#else
  #define DEFL_MQTT_STD_PORT            8883            // MQTTs
  #define DEFL_MQTT_ABROAD_PORT         10888           // MQTTs for abroad access
#endif /* DISABLE_SSL */
#define DEFL_MQTT_LOGIN                 "test"          // for SANDBOX mode
#define DEFL_MQTT_PASSWD                "test"          // for SANDBOX mode
#define DEFL_MQTT_TOPIC                 "TestTopic"     // for SANDBOX mode


/*
 * PubSub MQTT client (re)definitions
 */
#if defined(MQTT_KEEPALIVE)
  #warning "MQTT_KEEPALIVE overriden by compilation directive !"
#else
  #define MQTT_KEEPALIVE          60
#endif /* MQTT_KEEPALIVE */
#if defined(MQTT_SOCKET_TIMEOUT)
  #warning "MQTT_SOCKET_TIMEOUT overriden by compilation directive !"
#else
  #define MQTT_SOCKET_TIMEOUT     60
#endif /* MQTT_SOCKET_TIMEOUT */
#if defined(MQTT_MAX_PACKET_SIZE)
  #warning "MQTT_MAX_PACKET_SIZE overriden by compilation directive !"
#else
  #define MQTT_MAX_PACKET_SIZE    256
#endif /* MQTT_MAX_PACKET_SIZE */



/*
 * WiFi (and loosely related to) definitions
 */
// type for mac address in raw format
typedef uint8_t mac_addr_t[6];

/*
typedef struct { 
    uint8_t bytes[6]; 
} mac_addr_t;
*/

// type for mac address in string format
typedef char mac_str_t[18];

/* WiFi AP mode credentials:
 *   Password required to get connected to the WiFi settings mode.
 */
#if !defined(WIFI_AP_CREDENTIALS) && !defined(DISABLE_AP_PASSWD)
#define WIFI_AP_CREDENTIALS     "neOSensor"
#endif /* WIFI_AP_CREDENTIALS */


/*
 * Modules related definitions
 */
/* a module's config file will be something like
 * "/module_luminosity.json"
 */
#define MCFG_FILE_PREFIX        "/module_"
#define MCFG_FILE_SUFFIX        ".json"


#endif /* _NEOCAMPUS_H */

