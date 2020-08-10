/*
 * neOCampus operation
 * 
 * Over The Air (OTA) firmware upgrade utilities
 *
 * ---
 * F.Thiebolt   aug.20  switched to HTTP with ssl support :)
 * Thiebolt F.  aug.17  initial release
 * 
 */



#ifndef _NEOCAMPUS_OTA_H_
#define _NEOCAMPUS_OTA_H_



/* ----------------------------------------------------------------------------
 * --- Includes ---------------------------------------------------------------
 * ----------------------------------------------------------------------------
 */
#include <Arduino.h>



/* ----------------------------------------------------------------------------
 * --- Definitions ------------------------------------------------------------
 * ----------------------------------------------------------------------------
 */
/*
 * OTA URL is something like
 * http://neocampus.univ-tlse3.fr/images/${hardware}/${board_name}/default.json
 */
#define OTA_BASE_URL          "https://neocampus.univ-tlse3.fr/images"
#define OTA_JSON_FILE         "default.json"    // name of file to look after @ neocampus server
#define OTA_URL_LENGTH        128               // maximum length of OTA's url
#define OTA_JSON_LENGTH       512               // maximum size of json file retrieved from server


/* ----------------------------------------------------------------------------
 * --- Functions prototypes ---------------------------------------------------
 * ----------------------------------------------------------------------------
 */
/*
 * Main firmware update function
 */
bool neOCampusOTA( void );

// upgrade from image's url
bool neOCampusOTA_url( const char * );

// get current revision firmware
firmwareRev_t getFirmwareRev();

// get hardware (e.g ESP8266, ESP32, unknown ...)
const char *getHardware( void );

// get board name (e.g neOSensor)
const char *getBoardName( void );

// get board revision
uint8_t getBoarRev( void );

#endif /* _NEOCAMPUS_OTA_H */

