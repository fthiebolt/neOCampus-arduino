/**************************************************************************/
/*! 
    @file     SCD4x.h
    @author   F. Thiebolt
	  @license
	
    This is part of a the neOCampus drivers library.
    SCD4x is an ultrasonic CO2 sensor that features both temperature and hygro
    on an I2C interface
    
    Remember: all transfers are 16bits wide
    
    (c) Copyright 2022 Thiebolt.F <thiebolt@irit.fr>

	@section  HISTORY

    2022-March    - F.Thiebolt Initial release
    
*/
/**************************************************************************/

#include <Arduino.h>
#include <limits.h>

#include "neocampus.h"
#include "neocampus_debug.h"
#include "neocampus_i2c.h"

// [may.20] debug
#include "neocampus_utils.h"


#include "SCD4x.h"



/**************************************************************************/
/*! 
    @brief  Declare list of possible I2C addrs
*/
/**************************************************************************/
const uint8_t SCD4x::i2c_addrs[] = { 0x69 };



/**************************************************************************/
/*! 
    @brief  Test if device at 'adr' is really what we think of
*/
/**************************************************************************/
boolean SCD4x::is_device( uint8_t a ) {
  
  // First step, parse all addresses
  boolean found = false;
  for( uint8_t i=0; i < sizeof(SCD4x::i2c_addrs); i++ ) {
    if( SCD4x::i2c_addrs[i] == a) {
      found = true;
      break;
    }
  }
  if( found == false ) return false;
  
  // check device identity
  if( !_check_identity(a) ) return false;

  // ... okay, we're probably what we expect ;)
  return true;
}


/**************************************************************************/
/*! 
    @brief  Instantiates a new class
*/
/**************************************************************************/
SCD4x::SCD4x( scd4xMeasureType_t kindness  ) : generic_driver() {
  _i2caddr = -1;
  _measureType = kindness;
}


/* declare kind of units */
const char *SCD4x::units_co2  = "ppm";
const char *SCD4x::units_temp = "celsius";
const char *SCD4x::units_rh   = "%r.H.";

/* declare others static vars */
unsigned long SCD4x::_lastMsRead  = 0;
uint16_t SCD4x::_co2_sensor = (uint16_t)(-1);
uint16_t SCD4x::_t_sensor   = (uint16_t)(-1);
uint16_t SCD4x::_rh_sensor  = (uint16_t)(-1);


/**************************************************************************/
/*! 
    @brief  send back units
*/
/**************************************************************************/
const char * SCD4x::sensorUnits( uint8_t idx ) {
  
  switch( _measureType ) {
    case scd4xMeasureType_t::co2:
      return units_co2;
      break;
    case scd4xMeasureType_t::temperature:
      return units_temp;
      break;
    case scd4xMeasureType_t::humidity:
      return units_rh;
      break;
    default:
      log_error(F("\n[SCD4x] unknown kind of measureType ?!?")); log_flush();
      return nullptr;
  }
}


/**************************************************************************/
/*! 
    @brief  Setups the HW
*/
/**************************************************************************/
boolean SCD4x::begin( uint8_t addr=-1) {
  // get i2caddr
  if( (addr < (uint8_t)(I2C_ADDR_START)) or (addr > (uint8_t)(I2C_ADDR_STOP)) ) return false;
  _i2caddr = addr;

  // check device identity
  if( !_check_identity(_i2caddr) ) return false;

  /* set config:
   * - nothing to configure
   * - reset ?
   */

  // define defaults parameters

  /* start lastmsg time measurement.
   * This way, we get sure to have at least a first acquisition! */
  _lastMsRead = ULONG_MAX/2;

  return true;
}


/*
 * Power modes: ON or powerOFF
 */
void SCD4x::powerOFF( void )
{
  // device does not feature continuous integration so nothing to start or stop
}

void SCD4x::powerON( void )
{
  // device does not feature continuous integration so nothing to start or stop
}

TO BE CONTINUED

/**************************************************************************/
/*! 
    @brief  Setups the HW: set resolution of sensor
    @note   remember that resolution is virtual, we just set integration
            timings, proper cmds for datat acquisition ought to get sent.
*/
/**************************************************************************/
bool SHT3x::setResolution( sht3xResolution_t res ) {

  // set integration timeout
  if( res == sht3xResolution_t::high_res )
    _integrationTime = static_cast<uint8_t>(sht3xIntegration_t::ms_integrate_high);
  else if( res == sht3xResolution_t::medium_res )
    _integrationTime = static_cast<uint8_t>(sht3xIntegration_t::ms_integrate_medium);
  else if( res == sht3xResolution_t::low_res )
    _integrationTime = static_cast<uint8_t>(sht3xIntegration_t::ms_integrate_low);
  else return false;

  // add some constant to integration time ...
  _integrationTime+=(uint8_t)SHT3X_INTEGRATION_TIME_CTE;

  // finish :)
  return true;
}


/**************************************************************************/
/*! 
    @brief  Reads the 16-bit temperature register and returns the Centigrade
            temperature as a float.

*/
/**************************************************************************/
boolean SHT3x::acquire( float *pval )
{
  if( pval==nullptr ) return false;

  // HUMIDITY
  if( _measureType == sht3xMeasureType_t::humidity ) {
    return getRH( pval );
  }

  // TEMPERATURE
  if( !getTemp(pval) ) {
    // error reading value ... too bad
    return false;
  }

  // [Mar.18] temperature correction for last i2c sensor ... the one
  // supposed to get tied to the main board.
#ifdef TEMPERATURE_CORRECTION_LASTI2C
  static uint8_t _last_i2c = (uint8_t)-1;
  // determine last i2c addr
  if( _last_i2c == (uint8_t)-1 ) {
    _last_i2c=i2c_addrs[(sizeof(i2c_addrs)/sizeof(i2c_addrs[0]))-1];
  }
  // is last sensor ?
  if ( _i2caddr == _last_i2c ) {
    *pval += TEMPERATURE_CORRECTION_LASTI2C;
    log_debug(F("\n[SHT3x] corrected temperature for i2c=0x"));log_debug(_i2caddr,HEX);log_flush();
  }
#endif

  return true;
}


/*
 * READ sensor's HUMIDITY
 */
boolean SHT3x::getRH( float *pval ) {

  if( pval==nullptr ) return false;

  bool res = false;
  uint8_t retry = 3;
  uint16_t val;
  
  while( res==false and retry-- ) {
    res = _readSensor( &val );
    if( res == true ) break;
    delay(1);  // 1ms demay between commands
  }

  if( res==false ) {
    // error ...
    log_error(F("\n[SHT3x] unable to read value ?!?!"));log_flush();
    return false;
  }

  uint32_t _tmp = (uint32_t)val;
  // from Adafruit SHT31
  // simplified (65536 instead of 65535) integer version of:
  // humidity = (shum * 100.0f) / 65535.0f;
  // 100.0 x _rh = val x 100 x 100 / ( 4096 x 16 )
  _tmp = (625 * _tmp) >> 12;
  *pval = (float)_tmp / 100.0f;
  return true;
}


/*
 * READ sensor's TEMPERATURE
 */
boolean SHT3x::getTemp( float *pval )
{
  if( pval==nullptr ) return false;

  bool res = false;
  uint8_t retry = 3;
  uint16_t val;
  
  while( res==false and retry-- ) {
    res = _readSensor( &val );
    if( res == true ) break;
    delay(1);  // 1ms demay between commands
  }

  if( res==false ) {
    // error ...
    log_error(F("\n[SHT3x] unable to read value ?!?!"));log_flush();
    return false;
  }

  int32_t _tmp = (int32_t)val;
  // from Adafruit SHT31
  // simplified (65536 instead of 65535) integer version of:
  // temp = (stemp * 175.0f) / 65535.0f - 45.0f;
  // 100.0 x _tmp = (17500 x _tmp) / (16384 x 4) - 4500
  _tmp = ((4375 * _tmp) >> 14) - 4500;
  *pval = (float)_tmp / 100.0f;
  return true;
}


/* ------------------------------------------------------------------------------
 * Private methods and attributes
 */

/*
 * Software reset through I2C command
 */
void SHT3x::sw_reset( uint8_t adr ) {
  log_debug(F("\n[SHT3x] SOFT RESET action started ..."));log_flush();
  _writeCmd( adr, static_cast<uint16_t>(sht3xCmd_t::soft_reset) );
  delay( 50 );
}


/*
 * Read 16bits sensor values registers and check CRCs
 * Note: SHT3x sensors read both T and RH with both CRCs
 * hence we store static values with a timestamp to avoid
 * multiple (useless) access for same things.
 */
bool SHT3x::_readSensor( uint16_t *pval ) {

  // do we need to acquire fresh sensors values ?
  if( (millis() - _lastMsRead ) >= (unsigned long)(SHT3X_SENSOR_CACHE_MS) ) {

    // select proper command
    uint16_t _cmd;
    if( _resolution == sht3xResolution_t::high_res )
      _cmd = static_cast<uint16_t>(sht3xCmd_t::meas_highRes);
    else if( _resolution == sht3xResolution_t::medium_res )
      _cmd = static_cast<uint16_t>(sht3xCmd_t::meas_medRes);
    else
      _cmd = static_cast<uint16_t>(sht3xCmd_t::meas_lowRes);
    
    uint8_t _retry = 3;
    bool status = false;
    while( status == false and _retry-- ) {
      // start acquisition command
      _writeCmd( _i2caddr, _cmd );

      // wait for integration
      delay( _integrationTime );

      // retrieve data:
      //  MSB[T] + LSB[T] + CRC[T] + MSB[RH] + LSB[RH] + CRC[RH]
      uint8_t buf[6];
      if( readList_ll(_i2caddr, buf, sizeof(buf)) < sizeof(buf) ) {
        log_error(F("\n[SHT2x] insufficient bytes answered"));log_flush();
        yield();
        sw_reset( _i2caddr );
        continue;
      }

      // check first CRC ( TEMP )
      if( not crc_check(buf,2,buf[2]) ) {
        log_error(F("\n[SHT3x] invalid CRC for TEMP ...")); log_flush();
        yield();
        continue;
      }

      // check second CRC ( RH )
      if( not crc_check(&buf[3],2,buf[5]) ) {
        log_error(F("\n[SHT3x] invalid CRC for RH ...")); log_flush();
        yield();
        continue;
      }

      // both CRC are valid, let's grab the data
      _t_sensor = buf[0] << 8;
      _t_sensor |= buf[1];
      _rh_sensor = buf[3] << 8;
      _rh_sensor |= buf[4];

      status = true;  // success
    }
    // check acquisition is ok
    if( not status ) {
      log_error(F("\n[SHT3x] unable to read sensor ...give up :("));log_flush();
      return false;
    }

    // success ==> update last read
    _lastMsRead = millis();
  }
  else {
    log_debug(F("\n[SHT3x] using cached value for "));
    if( _measureType == sht3xMeasureType_t::temperature ) {
      log_debug(F("TEMP sensor!"));
    }
    else {
      log_debug(F("RH sensor!"));
    }
    log_flush();
  }

  // send back pointer to proper value
  if( _measureType == sht3xMeasureType_t::temperature ) *pval = _t_sensor;
  else *pval = _rh_sensor;

  return true;
}


/*
 * CRC related attributes
 */
const uint8_t SHT3x::_crc8_polynom = 0x31;

#ifdef SHT3X_CRC_LOOKUP_TABLE
// CRC8 lookup table
const uint8_t SHT3x::_crc8_table[256] PROGMEM = {
0x00,0x31,0x62,0x53,0xC4,0xF5,0xA6,0x97,0xB9,0x88,0xDB,0xEA,0x7D,0x4C,0x1F,0x2E,
0x43,0x72,0x21,0x10,0x87,0xB6,0xE5,0xD4,0xFA,0xCB,0x98,0xA9,0x3E,0x0F,0x5C,0x6D,
0x86,0xB7,0xE4,0xD5,0x42,0x73,0x20,0x11,0x3F,0x0E,0x5D,0x6C,0xFB,0xCA,0x99,0xA8,
0xC5,0xF4,0xA7,0x96,0x01,0x30,0x63,0x52,0x7C,0x4D,0x1E,0x2F,0xB8,0x89,0xDA,0xEB,
0x3D,0x0C,0x5F,0x6E,0xF9,0xC8,0x9B,0xAA,0x84,0xB5,0xE6,0xD7,0x40,0x71,0x22,0x13,
0x7E,0x4F,0x1C,0x2D,0xBA,0x8B,0xD8,0xE9,0xC7,0xF6,0xA5,0x94,0x03,0x32,0x61,0x50,
0xBB,0x8A,0xD9,0xE8,0x7F,0x4E,0x1D,0x2C,0x02,0x33,0x60,0x51,0xC6,0xF7,0xA4,0x95,
0xF8,0xC9,0x9A,0xAB,0x3C,0x0D,0x5E,0x6F,0x41,0x70,0x23,0x12,0x85,0xB4,0xE7,0xD6,
0x7A,0x4B,0x18,0x29,0xBE,0x8F,0xDC,0xED,0xC3,0xF2,0xA1,0x90,0x07,0x36,0x65,0x54,
0x39,0x08,0x5B,0x6A,0xFD,0xCC,0x9F,0xAE,0x80,0xB1,0xE2,0xD3,0x44,0x75,0x26,0x17,
0xFC,0xCD,0x9E,0xAF,0x38,0x09,0x5A,0x6B,0x45,0x74,0x27,0x16,0x81,0xB0,0xE3,0xD2,
0xBF,0x8E,0xDD,0xEC,0x7B,0x4A,0x19,0x28,0x06,0x37,0x64,0x55,0xC2,0xF3,0xA0,0x91,
0x47,0x76,0x25,0x14,0x83,0xB2,0xE1,0xD0,0xFE,0xCF,0x9C,0xAD,0x3A,0x0B,0x58,0x69,
0x04,0x35,0x66,0x57,0xC0,0xF1,0xA2,0x93,0xBD,0x8C,0xDF,0xEE,0x79,0x48,0x1B,0x2A,
0xC1,0xF0,0xA3,0x92,0x05,0x34,0x67,0x56,0x78,0x49,0x1A,0x2B,0xBC,0x8D,0xDE,0xEF,
0x82,0xB3,0xE0,0xD1,0x46,0x77,0x24,0x15,0x3B,0x0A,0x59,0x68,0xFF,0xCE,0x9D,0xAC
};

// crc checksum verification
bool SHT3x::crc_check( uint8_t data[], uint8_t nb_bytes, uint8_t checksum ) {
	uint8_t crc = 0xff;
  while( nb_bytes ) {
    crc = pgm_read_byte_near(_crc8_table + (*data ^ crc));
    data++;
    nb_bytes--;
  }

  if( (crc & 0xFF) != checksum ) return false;
 	else return true;
}
#else
// crc checksum verification
bool SHT3x::crc_check( uint8_t data[], uint8_t nb_bytes, uint8_t checksum ) {
	uint8_t crc = 0xff;
  uint8_t byteCtr;

 	//calculates 8-Bit checksum with given polynomial
  for( byteCtr = nb_bytes; byteCtr; --byteCtr) {
 	  crc ^= *data++;
 	  for (uint8_t bit = 8; bit; --bit) {
      crc = (crc & 0x80) ? (crc << 1) ^ _crc8_polynom : (crc << 1);
 	  }
 	}

 	if (crc != checksum) return false;
 	else return true;
}
#endif /* SHT3X_CRC_LOOKUP_TABLE */


/*
 * Check that device identity is what we expect!
 */
void SHT3x::_writeCmd( uint8_t a, uint16_t cmd ) {
  write8( a, (uint8_t)(cmd>>8) , (uint8_t)(cmd&0xFF) );
  delay(1); // 1ms min between i2c transactions
}


/*
 * Check that device identity is what we expect!
 */
bool SHT3x::_check_identity( uint8_t a ) {

  uint8_t _retry = 3;
  bool status = false;

  while( not status and _retry-- ) {
    // cmd for read out status register
    _writeCmd( a, static_cast<uint16_t>(sht3xCmd_t::get_status) );

    // read answer (MSB + LSB + CRC)
    uint8_t buf[3];
    if( readList_ll(a, buf, sizeof(buf)) < sizeof(buf) ) {
      log_error(F("\n[SHT3x] not enough bytes read !"));log_flush();
      delay(1); // min time between commands
      sw_reset( a );
      continue;
    }

    // check answer's CRC
    if( not crc_check(buf, 2, buf[2]) ) {
      log_error(F("\n[SHT3x] CRC check failed !"));log_flush();
      delay(1); // min time between commands
      continue;
    }
  
    // finally does status match our expectations
    uint16_t status_reg = buf[0] << 8;
    status_reg |= buf[1];
    if( (status_reg & SHT3X_STATUS_REG_MASK) != SHT3X_STATUS_REG_DEFL ) return false;

    status = true;
  }
  // check result
  if( not status ) return false;

  // additional tests here

  return true;
}
