/*
 * neOCampus operation
 * 
 * EEPROM operations to store / retrieve keys
 * 
 * THiebolt F. May.18   replace min() with _min() due to esp8266 arduino release >= 2.4.0
 * Thiebolt F. July 17  initial release
 * 
 */



/*
 * Includes
 */
#include <EEPROM.h>

// neOCampus specific includes
#include "neocampus_eeprom.h"




/*
 * Functions
 */
// HEX DUMP of EEPROM content
void hexdumpEEPROM( void ) {
  log_debug(F("\n### EEPROM HEX DUMP (only first 20bytes) ###")); log_flush();
  for(uint8_t i=0; i < 20; i++ ){
    char _line[30];
    snprintf(_line,sizeof(_line),"\nEEPROM[%u] = 0x%02x '%c'", i, EEPROM.read(i), EEPROM.read(i));
    log_debug(_line); delay(20); log_flush();
  }
}


// EEPROM setup for sensOCampus purpose
bool setupEEPROM( void ) {
  log_info(F("\n[EEPROM] starting setup with size = ")); log_info(NEOCAMPUS_EEPROM_SIZE,DEC); log_flush();

  // declare EEPROM size
  EEPROM.begin( NEOCAMPUS_EEPROM_SIZE );

  // check if already initialized ?
  if( checkEEPROMvalidity() ) {
    log_debug(F("\n[EEPROM] already initialized :) "));
    return true;
  }

  // format EEPROM
  formatEEPROM();

  // set validity flag
  setEEPROMvalidity();

  return true;
}

// check EEPROM is valid
inline bool checkEEPROMvalidity( void ) {
  if( EEPROM.read(EEPROM_ADDR_CHECK) != (uint8_t)EEPROM_VALIDITY_CHECK ) return false;
  if( EEPROM.read(EEPROM_ADDR_SIZE) != (uint8_t)EEPROM_ADDR_LAST ) return false;
  return true;
}

// invalidate EEPROM content
void clearEEPROMvalidity( void ) {
  EEPROM.write(EEPROM_ADDR_CHECK, (uint8_t)0x00);
  delay(20);
  EEPROM.commit();
}

// format EEPROM for neOCampus use
bool formatEEPROM( void ) {
  log_debug(F("\n[EEPROM] nb bytes cleared is "));log_debug(EEPROM_ADDR_LAST,DEC); log_flush();
  for( uint16_t i=0; i<EEPROM_ADDR_LAST; i++ ) {
    EEPROM.write(i, 0);
  }  
  delay(20);
  return EEPROM.commit();
}

// set EEPROM validity flag
inline void setEEPROMvalidity( void ) {
  EEPROM.write(EEPROM_ADDR_CHECK, (uint8_t)EEPROM_VALIDITY_CHECK);
  EEPROM.write(EEPROM_ADDR_SIZE, (uint8_t)EEPROM_ADDR_LAST);
  delay(20);
  EEPROM.commit();
}


/* 
 * low-level write EEPROM function
 */
void _writeEEPROM( uint16_t start, uint16_t maxsize, const char *buf ) {
  //log_debug(F("\n[EEPROM] from addr : "));log_debug(start,DEC); log_flush();
  //log_debug(F("\n[EEPROM] writing string : ")); log_debug(buf); log_flush();
  for( uint16_t i=start; i<(uint16_t)(start+maxsize); i++ ) {
    EEPROM.write(i, buf[i-start]);
    if( buf[i-start] == '\0' ) break;
  }
  delay(20);
  EEPROM.commit();
}

// store 'login' within EEPROM
void setEEPROMlogin( const char *buf ) {
  // call to low-level write EEPROM function
  _writeEEPROM( (uint16_t)EEPROM_ADDR_MQTT_LOGIN, (uint16_t)SENSO_MQTT_LOGIN_LENGTH, buf );
}

// store 'passwd' within EEPROM
void setEEPROMpasswd( const char *buf ) {
  // call to low-level write EEPROM function
  _writeEEPROM( (uint16_t)EEPROM_ADDR_MQTT_PASSWD, (uint16_t)SENSO_MQTT_PASSWD_LENGTH, buf );
}


/* 
 * low-level read EEPROM function
 */
bool _readEEPROM( uint16_t start, uint16_t maxsize, char *buf ) {
  uint16_t i;
  for( i=(uint16_t)start; i<(uint16_t)(start+maxsize); i++ ) {
    buf[i-start] = EEPROM.read(i);
    if( buf[i-start] == '\0' ) break;
  }
  // check for zero sized strings
  if( i == start ) return false;
  return true;
}

// retrieve 'login' from EEPROM
bool getEEPROMlogin( char *buf, size_t bufsize ) {
  if( !buf or bufsize==0 ) return false;
  // call to low-level read EEPROM function
  return _readEEPROM( (uint16_t)EEPROM_ADDR_MQTT_LOGIN, (uint16_t)(_min(SENSO_MQTT_LOGIN_LENGTH,bufsize)), buf );
}

// retrieve 'passwd' from EEPROM
bool getEEPROMpasswd( char *buf, size_t bufsize ) {
  if( !buf or bufsize==0 ) return false;
  // call to low-level read EEPROM function
  return _readEEPROM( (uint16_t)EEPROM_ADDR_MQTT_PASSWD, (uint16_t)(_min(SENSO_MQTT_PASSWD_LENGTH,bufsize)), buf );  
}


