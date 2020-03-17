/*
 * neOCampus operation
 * 
 * Utilities functions like
 * - WiFi setup
 * - OTA updates
 * - SPIFFS related
 * - JSON encoding / decoding functions
 * - ...
 * 
 * Thiebolt F. July 17
 * 
 */



#ifndef _NEOCAMPUS_UTILS_H_
#define _NEOCAMPUS_UTILS_H_



/* ----------------------------------------------------------------------------
 * --- Includes ---------------------------------------------------------------
 * ----------------------------------------------------------------------------
 */
#include <Arduino.h>

#include "wifiParametersMgt.h"



/*
 * Global shared variables/functions
 */
// reboot flag
extern bool _need2reboot;



/* ----------------------------------------------------------------------------
 * --- Definitions ------------------------------------------------------------
 * ----------------------------------------------------------------------------
 */
// --- WiFi definitions -------------------------------------------------------
#define WM_CONFIG_PORTAL_TIMEOUT        300   // seconds config portail will stay active
#define WM_CONNECTION_ATTEMPT_TIMEOUT   90    // will wait up to xxs for connecting to the specified SSID



/* ----------------------------------------------------------------------------
 * --- Functions prototypes ---------------------------------------------------
 * ----------------------------------------------------------------------------
 */
// check clear switch for CLEAR procedure
bool checkCLEARswitch( uint8_t );

// whole neocampus WiFi connexion setup
bool setupWiFi( wifiParametersMgt *wp=nullptr );

// compute WiFi Access Point Name
const char *getAPname( void );

// obtain MAC address (e.g "18:fc:b3:de:c6:02")
const char *getMacAddress( void );

// retrieve current domain name
const char *getDomainName( void );

// retrieve current time
const char *getCurTime( const char *fmt=nullptr );

// HTTP get & get with credentials
bool http_get( const char *url, char *buf, size_t bufsize );
bool http_get( const char *url, char *buf, size_t bufsize, const char *login, const char *passwd );

// setup SPIFFS
bool setupSPIFFS( void );

// Format SPI FileSystem
bool formatSPIFFS( void );

// Reset configuration and restart (delete module's config files and reboot)
bool neOSensor_reset( void );

// proper restart
void neOSensor_reboot( void );

// --- utilities functions ----------------------------------------------------

// hex dump of a buffer
void hex_dump( const char *, size_t );

#endif /* _NEOCAMPUS_UTILS_H */
