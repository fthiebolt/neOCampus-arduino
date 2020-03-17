/*
 * neOCampus operation
 * 
 * EEPROM operations to store / retrieve keys
 * 
 * Thiebolt F. July 17
 * 
 */


#ifndef _NEOCAMPUS_EEPROM_H_
#define _NEOCAMPUS_EEPROM_H_

/*
 * Includes
 */

#include <Arduino.h>


#include "neocampus.h"
#include "neocampus_debug.h"



/*
 * Definitions
 */
#define NEOCAMPUS_EEPROM_SIZE           256

#define EEPROM_ADDR_CHECK               0                         // ought to contain value EEPROM_CHECK_VAL
#define EEPROM_ADDR_SIZE                (EEPROM_ADDR_CHECK + 1)   // ought to get equal to EEPROM_ADDR_LAST (in case EEPROM structure change, we reset everything)
#define EEPROM_ADDR_MQTT_LOGIN          (EEPROM_ADDR_SIZE + 1)
#define EEPROM_ADDR_MQTT_PASSWD         (EEPROM_ADDR_MQTT_LOGIN + SENSO_MQTT_LOGIN_LENGTH)
#define EEPROM_ADDR_LAST                (EEPROM_ADDR_MQTT_PASSWD + SENSO_MQTT_PASSWD_LENGTH)

#define EEPROM_VALIDITY_CHECK           42

/*
 * Functions
 */
// initialize EEPROM
bool setupEEPROM( void );

// verify EEPROM content validity
inline bool checkEEPROMvalidity( void );

// clear EEPROM validity (invalidation)
void clearEEPROMvalidity( void );

// format EEPROM for neOCampus use
bool formatEEPROM( void );

// set EEPROM validity flag
inline void setEEPROMvalidity( void );

// store 'login' within EEPROM
void setEEPROMlogin( const char * );

// store 'passwd' within EEPROM
void setEEPROMpasswd( const char * );

// retrieve 'login' from EEPROM
bool getEEPROMlogin( char *, size_t );

// retrieve 'passwd' from EEPROM
bool getEEPROMpasswd( char *, size_t );

// HEX DUMP of EEPROM content
void hexdumpEEPROM( void );

#endif /* _NEOCAMPUS_EEPROM_H_ */

