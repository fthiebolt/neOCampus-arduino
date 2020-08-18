/**************************************************************************/
/*! 
    @file     MCP47FEB.h
    @author   F. Thiebolt
	  @license
	
    This is part of a the neOCampus drivers library.
    MCP47FEB01 is a 8bits single DAC with I2C interface
    MCP47FEB02 is a 8bits dual DAC with I2C interface,
    MCP47FEB11 is a 10bits single DAC with I2C interface
    MCP47FEB12 is a 10bits dual DAC with I2C interface,
    MCP47FEB21 is a 12bits single DAC with I2C interface
    MCP47FEB22 is a 12bits dual DAC with I2C interface.
    
    Remember: all transfers are 16bits wide
    
    (c) Copyright 2017 Thiebolt F. <thiebolt@irit.fr>

	@section  HISTORY

    2017-Oct    - First release, 8bits single DAC support F. Thiebolt
    
*/
/**************************************************************************/

#ifndef _MCP47FEB_H_
#define _MCP47FEB_H_


#include <Arduino.h>

// generic DAC driver
#include "driver_dac.h"



/*
 * Definitions
 */



// DAC volatile address space
#define MCP47FEB_REG_DAC0         0x00    // DAC0 volatile register
#define MCP47FEB_REG_DAC1         0x01    // DAC0 volatile register

// DAC NON volatile address space
#define MCP47FEB_NVREG_DAC0       0x10    // DAC0 non-volatile register
#define MCP47FEB_NVREG_DAC1       0x11    // DAC0 non-volatile register

// NV I2C address
#define MCP47FEB_NVREG_I2C        0x1A    // GAIN and I2C base address non-volatile register
#define MCP47FEB_I2C_MASK         0x7F

// (NON) Volatile Programmable Gain
#define MCP47FEB_NVREG_GAIN       0x1A    // GAIN and I2C base address non-volatile register
#define MCP47FEB_NVG0_MASK        0x0200
#define MCP47FEB_NVG1_MASK        0x0400
// Programmable Gain (volatile)
#define MCP47FEB_REG_GAIN         0x0A    // GAIN and Status register
#define MCP47FEB_GAIN_MASK        0x0300  // both DAC0 and DAC1 ;)
// Gain modes for both volatile and non volatile
#define MCP47FEB_GAIN_1X          0x00
#define MCP47FEB_GAIN_2X          0x01

// (NON) Volatile Power Mode definitions
#define MCP47FEB_REG_POWER        0x09    // POWER MODE register
#define MCP47FEB_NVREG_POWER      0x19    // POWER MODE non-volatile register
#define MCP47FEB_POWER_MASK       0xFFF0  // both DAC0 and DAC1 ;)
#define MCP47FEB_NORMAL_OPS       0x00    // both DAC0 and DAC1 ;)
#define MCP47FEB_PWRDN_1k         0x05    // both DAC0 and DAC1 ;)
#define MCP47FEB_PWRDN_100k       0x0A    // both DAC0 and DAC1 ;)
#define MCP47FEB_PWRDN_OPEN       0x0F    // both DAC0 and DAC1 ;)

// (NON) Volatile Reference Voltage definitions
#define MCP47FEB_REG_VREF         0x08    // VREF selection register
#define MCP47FEB_NVREG_VREF       0x18    // VREF non-volatile selection register
#define MCP47FEB_VREF_MASK        0xFFF0  // both DAC0 and DAC1 ;)
#define MCP47FEB_VREF_VDD         0x00    // both DAC0 and DAC1 ;)
#define MCP47FEB_VREF_VREFPIN     0x0A    // both DAC0 and DAC1 ;)
#define MCP47FEB_VREF_VREFPIN_BUF 0x0F    // both DAC0 and DAC1 ;)

// WiperLock definitions (preliminary)
#define MCP47FEB_REG_WLOCK        0x0B    // WiperLock register
#define MCP47FEB_WLOCK_MASK       0xFFF0  // both DAC0 and DAC1 ;)
#define MCP47FEB_WLOCK_UNLOCK     0x00    // both DAC0 and DAC1 volatile and non volatile ;)



/*
 * Class
 */
class MCP47FEB : public driver_dac {
  public:
    MCP47FEB( void );
  
    bool begin( uint8_t );        // start with an i2c address
  
    // Set the configuration bits for the DAC
    void setGain( uint8_t );
    void setVRef( uint8_t );
    void setPowerMode( uint8_t );
    
    /*
    * Set the DAC output level
    */
    uint16_t setOutputLevel( uint16_t );    // set output to DAC0
    uint8_t setPercentOutput( uint8_t );    // parent class mandatory
    
    // for MQTT
    String subID( void ) { return String(_i2caddr); };


    // --- static methods / constants -----------------------
    
    // list of possibles I2C addrs
    static const uint8_t i2c_addrs[4];

    // device detection
    static boolean is_device( uint8_t );
    
    // i2c specific read / write
    static uint16_t _read16( uint8_t, uint8_t );
    static void _write16( uint8_t, uint8_t, uint16_t );
    
  // --- private methods / attributes ---------------------
  private:
    // attributes
    uint8_t _i2caddr;
    bool _initialized;
    uint16_t _output;     // DAC current output level
    uint8_t _resolution;  // DAC output resolution
};

#endif /* _MCP47FEB_H_ */
