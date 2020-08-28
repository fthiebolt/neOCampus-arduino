/*
 * neOCampus library
 * 
 * - I2C synchronous functions read/write 8 and 16bits
 */


/*
 * Includes
 */
#include <Wire.h>

// neOCampus specific includes
#include "neocampus.h"
#include "neocampus_debug.h"

#include "neocampus_i2c.h"



/* I2C scanner: it starts to scan I2C bus according to specified start parameter
 *  Scan stops whenever a device respond and we send back it address. You can continue scanning 
 *  giving previous answered addr+1
 */
uint8_t i2c_scan(uint8_t start) {

  // check that i2c addr is within range ...
  if( (start < I2C_ADDR_START) || (start > I2C_ADDR_STOP) ) {
    log_debug("\n###ERROR I2C scan start addr is wrong!"); log_flush();
    return -1;
  }
  
  // ... then let's start to scan i2c bus :)
  int res;
  for( uint8_t _addr=start; _addr <= I2C_ADDR_STOP; _addr++ ) {

    // i2c quick_write :)
    res = i2c_quick_write( _addr );
    delay(20);  // usefull ?  
 
    if( res==0 ) return _addr;
    else if( res==4 ) {
      log_debug("\n###ERROR I2C scan err="); log_debug(DEC,res);log_flush();
      return -1;
    }
  }

  // nothing detected
  return -1;
}

/**************************************************************************/
/*! 
    @brief  Low level I2C read and write functions!
*/
/**************************************************************************/
int i2c_quick_write( uint8_t adr ) {
  int res;
  Wire.beginTransmission( adr );
  res = Wire.endTransmission();

#ifdef DEBUG_I2C
  if( res==0 ) {
    char _msg[255];
    snprintf(_msg, sizeof(_msg), "\n[I2C-Qwrite] adr=0x%02x device detected ...", adr);
    log_debug(_msg);log_flush();
  }
#endif
  return res;
}

void write8(uint8_t adr, uint8_t value) {
#ifdef DEBUG_I2C
  char _msg[255];
  snprintf(_msg, sizeof(_msg), "\n[I2C-write8] adr=0x%02x (no reg) val=0x%02x", adr, value);
  log_debug(_msg);log_flush();
#endif
  Wire.beginTransmission(adr);
  Wire.write(value);
  Wire.endTransmission();
}

void write8(uint8_t adr, uint8_t reg, uint8_t value) {
#ifdef DEBUG_I2C
  char _msg[255];
  snprintf(_msg, sizeof(_msg), "\n[I2C-write8] adr=0x%02x reg=%d val=0x%02x", adr, reg, value);
  log_debug(_msg);log_flush();
#endif
  Wire.beginTransmission(adr);
  Wire.write((uint8_t)reg);
  Wire.write(value);
  Wire.endTransmission();
}

uint8_t read8(uint8_t adr, uint8_t reg) {
  uint8_t val;
  Wire.beginTransmission(adr);
  Wire.write((uint8_t)reg);
  Wire.endTransmission();

  Wire.requestFrom(adr, (uint8_t)1);
  val= Wire.read();

#ifdef DEBUG_I2C
  char _msg[255];
  snprintf(_msg, sizeof(_msg), "\n[I2C-read8] adr=0x%02x reg=%d val=0x%02x", adr, reg, val);
  log_debug(_msg);log_flush();
#endif

  return val;
}

void write16(uint8_t adr, uint8_t reg, uint16_t value) {
#ifdef DEBUG_I2C
  char _msg[255];
  snprintf(_msg, sizeof(_msg), "\n[I2C-write16] adr=0x%02x reg=%d val=0x%04x", adr, reg, value);
  log_debug(_msg);log_flush();
#endif
  Wire.beginTransmission(adr);
  Wire.write((uint8_t)reg);
  Wire.write(value >> 8);
  Wire.write(value & 0xFF);
  Wire.endTransmission();
}

uint16_t read16(uint8_t adr, uint8_t reg) {
  uint16_t val;

  Wire.beginTransmission(adr);
  Wire.write((uint8_t)reg);
  Wire.endTransmission();
  
  Wire.requestFrom(adr, (uint8_t)2);
  val = Wire.read();
  val <<= 8;
  val |= Wire.read();  

#ifdef DEBUG_I2C
  char _msg[255];
  snprintf(_msg, sizeof(_msg), "\n[I2C-read16] adr=0x%02x reg=%d val=0x%04x", adr, reg, val);
  log_debug(_msg);log_flush();
#endif

  return val;  
}



/*
 * I2C write16 little-endian
 */
void write16le(uint8_t adr, uint8_t reg, uint16_t value) {
#ifdef DEBUG_I2C
  char _msg[255];
  snprintf(_msg, sizeof(_msg), "\n[I2C-write16le] adr=0x%02x reg=%d val=0x%04x", adr, reg, value);
  log_debug(_msg);log_flush();
#endif
  Wire.beginTransmission(adr);
  Wire.write((uint8_t)reg);
  Wire.write(value & 0xFF);
  Wire.write(value >> 8);
  Wire.endTransmission();
}

/*
 * I2C read16 little-endian
 */
uint16_t read16le(uint8_t adr, uint8_t reg) {
  uint16_t val;

  Wire.beginTransmission(adr);
  Wire.write((uint8_t)reg);
  Wire.endTransmission();
  
  Wire.requestFrom(adr, (uint8_t)2);
  uint8_t lsb = Wire.read();
  uint8_t msb = Wire.read();
  val = msb << 8;
  val |= lsb;  

#ifdef DEBUG_I2C
  char _msg[255];
  snprintf(_msg, sizeof(_msg), "\n[I2C-read16le] adr=0x%02x reg=%d val=0x%04x", adr, reg, val);
  log_debug(_msg);log_flush();
#endif

  return val;  
}



/*
 * I2C bulk transfers
 * involving the 'i2c repeated start bit'
 * [option] delay between writing command and reading result
 * return number of bytes read
 */
uint8_t readList( uint8_t adr, uint8_t reg, uint8_t tab[], uint8_t tabsize, uint8_t pauseMs ) {
  
  Wire.beginTransmission(adr);
  Wire.write((uint8_t)reg);
  Wire.endTransmission( false );  // enables the 'repeated start bit'

  if( pauseMs ) delay( pauseMs );

  Wire.requestFrom(adr, tabsize);
  uint8_t i=0;
  while( Wire.available() and ( i < tabsize) ) {
    tab[i++] = Wire.read();
  }
  Wire.endTransmission();

#ifdef DEBUG_I2C
  char _msg[255];
  snprintf(_msg, sizeof(_msg), "\n[I2C-readList] adr=0x%02x reg=%d val=[0x%02x 0x%02x 0x%02x 0x%02x ... ]", adr, reg, tab[0], tab[1], tab[2], tab[3]);
  log_debug(_msg);log_flush();
#endif
  
  return i;
}

uint8_t writeList( uint8_t adr, uint8_t reg, uint8_t *tab, uint8_t tabsize ){
  log_debug("\nNOT YET AVAILABLE :|");
  return 0;
}


// --- low level i2c functions (i.e specific use)

/*
 * I2C low-level bulk transfers
 * Note: we're just expecting to read 'n' bytes of data from an i2c device.
 * return number of bytes read
 */
uint8_t readList_ll( uint8_t adr, uint8_t tab[], uint8_t tabsize, uint8_t pauseMs ) {
  
  /* NOTE: we don't write anything, just start with read */
  Wire.requestFrom(adr, tabsize);

  if( pauseMs ) delay( pauseMs );
  else yield();

  uint8_t i=0;
  while( Wire.available() and (i < tabsize) ) {
    tab[i++] = Wire.read();
  }
  Wire.endTransmission();

#ifdef DEBUG_I2C
  char _msg[255];
  snprintf(_msg, sizeof(_msg), "\n[I2C-readList-ll] adr=0x%02x val=[0x%02x 0x%02x 0x%02x 0x%02x ... ]", adr, tab[0], tab[1], tab[2], tab[3]);
  log_debug(_msg);log_flush();
#endif
  
  return i;
}




