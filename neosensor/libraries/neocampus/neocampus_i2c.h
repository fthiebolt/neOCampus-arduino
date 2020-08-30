/*
 * neOCampus operation
 * 
 * I2C functions
 * 
 * Thiebolt F. June 17
 * 
 */


#ifndef _NEOCAMPUS_I2C_H_
#define _NEOCAMPUS_I2C_H_

/*
 * Includes
 */
#include <Arduino.h>



/*
 * Definitions
 */

// i2c bus related definition
//#define DEBUG_I2C           // low-level I2C debug messages (verbose option!!)
#define I2C_ADDR_START  0x01
#define I2C_ADDR_STOP   0x7F



/*
 * Common functions
 */
// I2C bus scanner
uint8_t i2c_scan(uint8_t start);

// I2C synchronous functions
int i2c_quick_write( uint8_t adr );

uint8_t read8(uint8_t adr, uint8_t reg);
void write8(uint8_t adr, uint8_t val);
void write8(uint8_t adr, uint8_t reg, uint8_t val);

uint16_t read16(uint8_t adr, uint8_t reg);              // big endian (default)
void write16(uint8_t adr, uint8_t reg, uint16_t val);   // big endian (default)

uint16_t read16le(uint8_t adr, uint8_t reg);            // little-endian
void write16le(uint8_t adr, uint8_t reg, uint16_t val); // little-endian

uint8_t readList( uint8_t adr, uint8_t reg, uint8_t *tab, uint8_t tabsize, uint8_t pauseMs=0 );
uint8_t writeList( uint8_t adr, uint8_t reg, uint8_t *tab, uint8_t tabsize );

// I2C synchronous LL functions
uint8_t readList_ll( uint8_t adr, uint8_t *tab, uint8_t tabsize, uint8_t pauseMs=0 );

#endif /* _NEOCAMPUS_I2C_H */


