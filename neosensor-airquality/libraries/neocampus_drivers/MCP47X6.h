/**************************************************************************/
/*! 
    @file     MCP47x6.h
    @author   F. Thiebolt
	  @license
	
    This is part of a the neOCampus drivers library.
    MCP47x6 DAC 8/10/12bits with an I2C interface
        
    (c) Copyright 2017 Thiebolt.F <thiebolt@irit.fr>

	@section  HISTORY

    2017-July    - F.Thiebolt Initial release
    
*/
/**************************************************************************/

#ifndef _MCP47X6_H_
#define _MCP47X6_H_


#include <Arduino.h>

// generic DAC driver
#include "driver_dac.h"



/*
 * Definitions
 */


/** Config Data Defn
 * Config = 0bCCCVVPPG
 */

// MCP4706, 4716 and 4726 resolution
#define MCP4706_RESOLUTION        8       // 8 bits DAC
#define MCP4716_RESOLUTION        10      // 10 bits DAC
#define MCP4726_RESOLUTION        12      // 10 bits DAC

// Programmable Gain definitions
#define MCP47X6_GAIN_MASK	0xFE
#define MCP47X6_GAIN_1X	0x00
#define MCP47X6_GAIN_2X	0x01

// Power Down Mode definitions
#define MCP47X6_PWRDN_MASK     0xF9
#define MCP47X6_NORMAL_OPS     0x00
#define MCP47X6_PWRDN_1K       0x02
#define MCP47X6_PWRDN_100K     0x04
#define MCP47X6_PWRDN_500K     0x06

// Reference Voltage definitions
#define MCP47X6_VREF_MASK         0xE7
#define MCP47X6_VREF_VDD          0x00
#define MCP47X6_VREF_VREFPIN	    0x10
#define MCP47X6_VREF_VREFPIN_BUF  0x18

// Command definitioins
#define MCP47X6_CMD_MASK       0x1F
#define MCP47X6_CMD_VOLDAC     0x00   // update voltaile DAC
#define MCP47X6_CMD_VOLALL     0x40   // update volatile DAC & volatile config bits
#define MCP47X6_CMD_ALL        0x60   // update volatile+EEPROM DAC & volatile+EEPROM config bits
#define MCP47X6_CMD_VOLCONFIG  0x80   // update volatile config bits

// Default values for MCP47X6 chips
#define MCP47X6_DEFL_CONFIG       ( MCP47X6_VREF_VDD | MCP47X6_NORMAL_OPS | MCP47X6_GAIN_1X )



/*
 * Class
 */
class MCP47X6 : public driver_dac {
  public:
    MCP47X6( void );
  
    bool begin( uint8_t );        // start with an i2c address
  
    // Set the configuration bits for the DAC
    void setGain( uint8_t );
    void setVRef( uint8_t );
    
    /*
    * Set the DAC output level
    */
    uint16_t setOutputLevel( uint16_t level );
    uint8_t setPercentOutput( uint8_t );         // parent class mandatory
    
    // MQTT need to retrieve i2c addr (i.e subID)
    uint8_t subID( void ) { return _i2caddr; }; // parent class mandatory
    
    // --- static methods / constants -----------------------
    
    // list of possibles I2C addrs
    static const uint8_t i2c_addrs[8];

    // device detection
    static boolean is_device( uint8_t );

  // --- private methods / attributes ---------------------
  private:
    // attributes
    uint8_t   _i2caddr;
    bool      _initialized;
    uint8_t   _config;      // config bits [Vref1, Vref0, PD1, PD0, G]
    uint16_t  _output;      // DAC current output level
    uint8_t   _resolution;  // DAC resolution

    // methods ...
    void inline setConfig( void );
    void inline setConfig( uint8_t config );
};

#endif /* _MCP47X6_H_ */

