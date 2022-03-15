/**************************************************************************/
/*! 
    @file     tsl2561.c
    @author   K. Townsend (microBuilder.eu / adafruit.com)

    @section LICENSE

    Software License Agreement (BSD License)

    Copyright (c) 2010, microBuilder SARL, Adafruit Industries
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
    1. Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
    3. Neither the name of the copyright holders nor the
    names of its contributors may be used to endorse or promote products
    derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

    @history
    2017  - mostly rewritten for neOCampus

*/
/**************************************************************************/


#include <Arduino.h>

#include "neocampus.h"
#include "neocampus_debug.h"
#include "neocampus_i2c.h"

#include "TSL2561.h"


/*
 * Definitions
 */
#define TSL2561_DELAY_INTTIME_13MS    (15)
#define TSL2561_DELAY_INTTIME_101MS   (120)
#define TSL2561_DELAY_INTTIME_402MS   (450)



/**************************************************************************/
/*! 
    @brief  Declare list of possible I2C addrs
*/
/**************************************************************************/
const uint8_t TSL2561::i2c_addrs[3] = { TSL2561_ADDR_LOW, TSL2561_ADDR_FLOAT, TSL2561_ADDR_HIGH };

/* declare kind of units */
const char *TSL2561::units = "lux";


/**************************************************************************/
/*! 
    @brief  Test if device at 'adr' is really a MCP9808
*/
/**************************************************************************/
boolean TSL2561::is_device( uint8_t a ) {
  
  // First step, parse all addresses
  boolean found = false;
  for( uint8_t i=0; i < sizeof(TSL2561::i2c_addrs); i++ ) {
    if( TSL2561::i2c_addrs[i] == a) {
      found = true;
      break;
    }
  }
  if( found == false ) return false;

  // check device identity
  if( !_check_identity(a) ) return false;

  // ... okay, we're probably a MCP9808 ;)
  return true;
}



/*
 * Constructor
 */
TSL2561::TSL2561( void ) : generic_driver() {
  _i2caddr = INVALID_I2CADDR;
  _initialized = false;
  _integration = TSL2561_DEFL_INTEGR_TIME;
  _gain = TSL2561_DEFL_GAIN;
  _auto_gain = true;    // auto gain activated
}



boolean TSL2561::begin( uint8_t addr=INVALID_I2CADDR ) {
  // get i2caddr
  if( (addr < (uint8_t)(I2C_ADDR_START)) or (addr > (uint8_t)(I2C_ADDR_STOP)) ) return false;
  _i2caddr = addr;

  // Wire.begin(); // No, Wire.begin already done earlier!
  
  // Note: by default, the device is in power down mode on bootup
  powerON();

  
  // check that power is ON
  uint8_t _res;
  _res = read8(_i2caddr,  TSL2561_COMMAND_BIT | TSL2561_CLEAR_BIT | TSL2561_REGISTER_CONTROL);
  if( (_res & TSL2561_CONTROL_POWERON) != (uint8_t)TSL2561_CONTROL_POWERON ) return false;

  
  // check device identity
  if( !_check_identity(_i2caddr) ) return false;

  // Set default integration time and gain
  setTiming(_integration);
  setGain(_gain);

  // chip is now initialized
  _initialized = true;
  
  // Note: by default, the device is in power down mode on bootup
  powerOFF();

  return true;
}

void TSL2561::powerON(void)
{
  _powerON( _i2caddr );
}

void TSL2561::powerOFF(void)
{
  _powerOFF( _i2caddr );
}


/* Auto Gain to enable automated full range of sensor :) 
 * - true --> activation
 * - false --> disable auto gain
 */
void TSL2561::enableAutoRange(bool enable) {
  _auto_gain = enable ? true : false;;
}


void TSL2561::setGain(tsl2561Gain_t gain) {
  if (!_initialized) begin();

  // start device
  powerON();
  
  _gain = gain;
  write8(_i2caddr, TSL2561_COMMAND_BIT | TSL2561_CLEAR_BIT | TSL2561_REGISTER_TIMING, _integration | _gain);  

  // stop device to save energy
  powerOFF();
}

void TSL2561::setTiming(tsl2561IntegrationTime_t integration) {
  if (!_initialized) begin();

  // start device
  powerON();
  
  _integration = integration;
  write8(_i2caddr, TSL2561_COMMAND_BIT | TSL2561_CLEAR_BIT | TSL2561_REGISTER_TIMING, _integration | _gain);

  // stop device to save energy
  powerOFF();
}


/* ------------------------------------------------------------------------------
 * Private methods 
 */

/*
 * static PowerON/OFF (for static acquire)
 */
void TSL2561::_powerON( uint8_t a ) {
  // Enable the device by setting the control bit to 0x03
  write8( a, TSL2561_COMMAND_BIT | TSL2561_CLEAR_BIT | TSL2561_REGISTER_CONTROL, TSL2561_CONTROL_POWERON);
}

void TSL2561::_powerOFF( uint8_t a ) {
  // Disable the device by setting the control bit to 0x03
  write8( a, TSL2561_COMMAND_BIT | TSL2561_CLEAR_BIT | TSL2561_REGISTER_CONTROL, TSL2561_CONTROL_POWEROFF);
}

/*
 * Check that device identity is what we expect!
 */
bool TSL2561::_check_identity( uint8_t a ) {

  // stop'n start device
  _powerOFF(a); delay(10);
  _powerON(a); delay(10);

  // check Register ID
  Wire.beginTransmission(a);
  Wire.write((uint8_t)(TSL2561_COMMAND_BIT | TSL2561_CLEAR_BIT | TSL2561_REGISTER_CONTROL));
  Wire.beginTransmission(a);
  Wire.requestFrom(a, (uint8_t)1);

TO BE CONTINUED

  else yield();

  uint8_t i=0;
  while( Wire.available() and (i < tabsize) ) {
    tab[i++] = Wire.read();
  }
  Wire.endTransmission();

TO BE CONTINUED

  uint8_t _res = read8(a,TSL2561_REGISTER_ID);
  if( (_res&0xF0)!=REGISTER_ID_TSL2561 and (_res&0xF0)!=0x00  ) return false;

  return true;
}



/* ----------------------------------------------------------
 * Low-level channel 0 and channel 1 sensor acquisition
 *   Channel0 --> Visible + IR
 *   Channel1 --> IR only
 * - pointer to uint16_t that will hold ch0 and ch1 values
 * - return None
 */
void TSL2561::_getData (uint16_t *ch0, uint16_t *ch1) {

  /* Enable the device by setting the control bit to 0x03 */
  powerON();

  /* Wait x ms for ADC to complete */
  switch( _integration )
  {
    case TSL2561_INTEGRATIONTIME_13MS:
      delay(TSL2561_DELAY_INTTIME_13MS);  // KTOWN: Was 14ms
      break;
    case TSL2561_INTEGRATIONTIME_101MS:
      delay(TSL2561_DELAY_INTTIME_101MS); // KTOWN: Was 102ms
      break;
    default:
      delay(TSL2561_DELAY_INTTIME_402MS); // KTOWN: Was 403ms
      break;
  }

  /* Reads a two byte value from channel 0 (visible + infrared)
   *  little-endian! */
  *ch0 = read16le(_i2caddr, TSL2561_COMMAND_BIT | TSL2561_WORD_BIT | TSL2561_REGISTER_CHAN0_LOW); // read16 will both read CHAN0_LOW and CHAN0_HI
  //log_debug(F("\n\t[TSL2561] ch0 = ")); log_debug((int)(*ch0),DEC);

  /* Reads a two byte value from channel 1 (infrared)
   *  little-endian! */
  *ch1 = read16le(_i2caddr, TSL2561_COMMAND_BIT | TSL2561_WORD_BIT | TSL2561_REGISTER_CHAN1_LOW); // read16 will both read CHAN1_LOW and CHAN1_HI
  //log_debug(F("\n\t[TSL2561] ch1 = "); log_debug((int)(*ch1), DEC);

  /* Turn the device off to save power */
  powerOFF();
}



/*
 * Function that retrieves both broadband and IR channels of sensor
 * If 'auto Gain' is enabled, it may iterate with the various possible
 * gains.
 * - pointers to uint16_t that will hold ch0 and ch1 values
 * - return None
 */
void TSL2561::getLuminosity( uint16_t *broadband, uint16_t *ir ) {
  if( !_initialized ) begin();

  // acquire data
  _getData( broadband, ir );

  // auto gain disabled ?
  if( _auto_gain==false ) return;

/*
  // Do we need to change the gain ?
  // ... auto gain computation
  if( ( ((*broadband & 0xF000) == 0) and ((*ir & 0xF000) == 0) ) and (_gain==TSL2561_GAIN_1X) ) {

    // set gain to 16 and get values
    log_debug(F("\n[TSL2561][auto] set gain to 16X ..."));
    setGain(TSL2561_GAIN_16X);

    // ... and acquire data anew ...
    _getData( broadband, ir );
    return;
  }

  if( ( ((*broadband & 0xF000) == (uint16_t)0xF000) and ((*ir & 0xF000) == (uint16_t)0xF000) ) and (_gain==TSL2561_GAIN_16X) ) {

    // set gain to 1 and get values
    log_debug(F("\n[TSL2561][auto] set back gain to 1X ..."));
    setGain(TSL2561_GAIN_1X);

    // ... and acquire data anew ...
    _getData( broadband, ir );
    return;
  }
*/


  // Read data until we find a valid range
  bool valid = false;
  bool _agcCheck = false;
  do
  {
    uint16_t _b, _ir;
    uint16_t _hi, _lo;
    tsl2561IntegrationTime_t _it = _integration;

    // Get the hi/low threshold for the current integration time
    switch(_it)
    {
      case TSL2561_INTEGRATIONTIME_13MS:
        _hi = TSL2561_AGC_THI_13MS;
        _lo = TSL2561_AGC_TLO_13MS;
        break;
      case TSL2561_INTEGRATIONTIME_101MS:
        _hi = TSL2561_AGC_THI_101MS;
        _lo = TSL2561_AGC_TLO_101MS;
        break;
      default:
        _hi = TSL2561_AGC_THI_402MS;
        _lo = TSL2561_AGC_TLO_402MS;
    }

    _getData(&_b, &_ir);

    // Run an auto-gain check if we haven't already done so ...
    if (!_agcCheck)
    {
      if ((_b < _lo) && (_gain == TSL2561_GAIN_1X))
      {
        // Increase the gain and try again
        log_debug(F("\n[TSL2561][auto] set gain to 16X ..."));
        setGain(TSL2561_GAIN_16X);
        // Drop the previous conversion results
        _getData(&_b, &_ir);
        // Set a flag to indicate we've adjusted the gain
        _agcCheck = true;
      }
      else if ((_b > _hi) && (_gain == TSL2561_GAIN_16X))
      {
        // Drop gain to 1x and try again
        log_debug(F("\n[TSL2561][auto] set back gain to 1X ..."));
        setGain(TSL2561_GAIN_1X);
        // Drop the previous conversion results
        _getData(&_b, &_ir);
        // Set a flag to indicate we've adjusted the gain
        _agcCheck = true;
      }
      else
      {
        // Nothing to look at here, keep moving ....
        // Reading is either valid, or we're already at the chips limits
        *broadband = _b;
        *ir = _ir;
        valid = true;
      }
    }
    else
    {
      // If we've already adjusted the gain once, just return the new results.
      // This avoids endless loops where a value is at one extreme pre-gain,
      // and the the other extreme post-gain
      *broadband = _b;
      *ir = _ir;
      valid = true;
    }
  } while( !valid );

  return;
}



/*
 * Compute LUX value according to ch0 and ch1 sensors values
 * - ch0 --> channel 0 sensor (broadband)
 * - ch1 --> channel 1 sensor (IR)
 * - return computed luminosity (lux)
 */
uint32_t TSL2561::calculateLux(uint16_t ch0, uint16_t ch1)
{
  unsigned long chScale;
  unsigned long channel1;
  unsigned long channel0;

  switch( _integration )
  {
    case TSL2561_INTEGRATIONTIME_13MS:
      chScale = TSL2561_LUX_CHSCALE_TINT0;
      break;
    case TSL2561_INTEGRATIONTIME_101MS:
      chScale = TSL2561_LUX_CHSCALE_TINT1;
      break;
    default: // No scaling ... integration time = 402ms
      chScale = (1 << TSL2561_LUX_CHSCALE);
  }

  // Scaling only if gain is 1X
  if( _gain==TSL2561_GAIN_1X ) chScale = chScale << 4;

  // scale the channel values
  channel0 = (ch0 * chScale) >> TSL2561_LUX_CHSCALE;
  channel1 = (ch1 * chScale) >> TSL2561_LUX_CHSCALE;

  // find the ratio of the channel values (Channel1/Channel0)
  unsigned long ratio1 = 0;
  if (channel0 != 0) ratio1 = (channel1 << (TSL2561_LUX_RATIOSCALE+1)) / channel0;

  // round the ratio value
  unsigned long ratio = (ratio1 + 1) >> 1;

  unsigned int b, m;

#ifdef TSL2561_PACKAGE_CS
  if ((ratio >= 0) && (ratio <= TSL2561_LUX_K1C))
    {b=TSL2561_LUX_B1C; m=TSL2561_LUX_M1C;}
  else if (ratio <= TSL2561_LUX_K2C)
    {b=TSL2561_LUX_B2C; m=TSL2561_LUX_M2C;}
  else if (ratio <= TSL2561_LUX_K3C)
    {b=TSL2561_LUX_B3C; m=TSL2561_LUX_M3C;}
  else if (ratio <= TSL2561_LUX_K4C)
    {b=TSL2561_LUX_B4C; m=TSL2561_LUX_M4C;}
  else if (ratio <= TSL2561_LUX_K5C)
    {b=TSL2561_LUX_B5C; m=TSL2561_LUX_M5C;}
  else if (ratio <= TSL2561_LUX_K6C)
    {b=TSL2561_LUX_B6C; m=TSL2561_LUX_M6C;}
  else if (ratio <= TSL2561_LUX_K7C)
    {b=TSL2561_LUX_B7C; m=TSL2561_LUX_M7C;}
  else if (ratio > TSL2561_LUX_K8C)
    {b=TSL2561_LUX_B8C; m=TSL2561_LUX_M8C;}
#else
  if ((ratio >= 0) && (ratio <= TSL2561_LUX_K1T))
    {b=TSL2561_LUX_B1T; m=TSL2561_LUX_M1T;}
  else if (ratio <= TSL2561_LUX_K2T)
    {b=TSL2561_LUX_B2T; m=TSL2561_LUX_M2T;}
  else if (ratio <= TSL2561_LUX_K3T)
    {b=TSL2561_LUX_B3T; m=TSL2561_LUX_M3T;}
  else if (ratio <= TSL2561_LUX_K4T)
    {b=TSL2561_LUX_B4T; m=TSL2561_LUX_M4T;}
  else if (ratio <= TSL2561_LUX_K5T)
    {b=TSL2561_LUX_B5T; m=TSL2561_LUX_M5T;}
  else if (ratio <= TSL2561_LUX_K6T)
    {b=TSL2561_LUX_B6T; m=TSL2561_LUX_M6T;}
  else if (ratio <= TSL2561_LUX_K7T)
    {b=TSL2561_LUX_B7T; m=TSL2561_LUX_M7T;}
  else if (ratio > TSL2561_LUX_K8T)
    {b=TSL2561_LUX_B8T; m=TSL2561_LUX_M8T;}
#endif

  unsigned long temp;
  temp = ((channel0 * b) - (channel1 * m));

  // do not allow negative lux value
  if (temp < 0) temp = 0;

  // round lsb (2^(LUX_SCALE-1))
  temp += (1 << (TSL2561_LUX_LUXSCALE-1));

  // strip off fractional portion
  uint32_t lux = temp >> TSL2561_LUX_LUXSCALE;

  // Signal I2C had no errors
  return lux;
}



/*
 * Simple wrapper to retrieve lux value from sensor :)
 * - return Lux value :)
 */
boolean TSL2561::acquire( float *pval ) {

  uint16_t ch0, ch1;

  // get channels luminosity
  getLuminosity( &ch0, & ch1 );

  // ... and return computed lux value :)
  *pval = (float)calculateLux( ch0, ch1 );
  return true;
}

