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


/* [static] declare kind of units */
const char *SCD4x::units_co2  = "ppm";
const char *SCD4x::units_temp = "celsius";
const char *SCD4x::units_rh   = "%r.H.";

/* [static] initialize static vars
 * REMEMBER: only possible because there's ONLY ONE SCD4x per neOSensor
 */
unsigned long SCD4x::_lastCheck   = 0;
unsigned long SCD4x::_lastData    = 0;
boolean SCD4x::_periodic_measure  = false;
uint16_t SCD4x::_co2_sensor       = (uint16_t)(-1);
boolean SCD4x::_co2_sensor_valid  = false;
uint16_t SCD4x::_t_sensor         = (uint16_t)(-1);
boolean SCD4x::_t_sensor_valid    = false;
uint16_t SCD4x::_rh_sensor        = (uint16_t)(-1);
boolean SCD4x::_rh_sensor_valid   = false;


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
   * - software reset
   * - calibration ?
   * - start periodic measurement ?
   */

  // soft reset
  // REMEMBER: we're shared across multiple modules !!
  sw_reset( _i2caddr );

  return true;
}


/*
 * Power modes: ON & OFF
 */
void SCD4x::powerON( void )
{
}

void SCD4x::powerOFF( void )
{
  // stop periodic measure
  if( _periodic_measure ) {
    uint16_t _cmd = static_cast<uint16_t>(scd4xCmd_t::stop_periodic_measurement);
    _writeCmd( _i2caddr, _cmd );
    delay(500);

    _periodic_measure = false;
  }
}


/**************************************************************************/
/*! 
    @brief  Reads the 16-bit [co2|temp|hygro] register and returns as float.

*/
/**************************************************************************/
boolean SCD4x::acquire( float *pval )
{
  // periodic measurement ?
  if( !_periodic_measure ) {
    // send command to acquire new sensors values
    uint16_t _cmd = static_cast<uint16_t>(scd4xCmd_t::start_periodic_measurement);
    _writeCmd( _i2caddr, _cmd );

    _periodic_measure = true;
    _lastCheck = millis();
    _lastData = _lastCheck;
    return false; // data won't get ready before min. 5000ms
  }

  // is measurement over ?
  if( _isDataReady() ) {
    /* extract new data along with CRC check
     * and set proper flag in received data
     */
    _readSensor();
  }

  if( pval==nullptr ) return false;

  // CO2
  if( _measureType == scd4xMeasureType_t::co2 ) {
    return getCO2( pval );
  }

  // TEMPERATURE
  if( _measureType == scd4xMeasureType_t::temperature ) {
    return getTemp( pval );
  }

  // HUMIDITY
  if( _measureType == scd4xMeasureType_t::humidity ) {
    return getRH( pval );
  }

  log_warning(F("\n[SCD4x] unknown MeasureType 0x"));log_warning(static_cast<uint8_t>(_measureType),HEX);log_flush();
  return false;
}


/*
 * READ sensor's CO2
 * Note: only sends back new values
 */
boolean SCD4x::getCO2( float *pval )
{
  if( pval==nullptr ) return false;
  if( !_co2_sensor_valid ) return false;

  *pval = (float)_co2_sensor;

  _co2_sensor_valid = false;

  return true;
}


/*
 * READ sensor's TEMPERATURE
 */
boolean SCD4x::getTemp( float *pval )
{
  if( pval==nullptr ) return false;
  if( !_t_sensor_valid ) return false;

  int32_t _tmp = (int32_t)_t_sensor;
  // from Adafruit SHT31
  // temp = (stemp * 175.0f) / 65536.0f - 45.0f;
  // 100.0 x _tmp = (17500 x _tmp) / (16384 x 4) - 4500
  _tmp = ((4375 * _tmp) >> 14) - 4500;
  *pval = (float)_tmp / 100.0f;

  _t_sensor_valid = false;

  return true;
}


/*
 * READ sensor's HUMIDITY
 */
boolean SCD4x::getRH( float *pval ) {

  if( pval==nullptr ) return false;
  if( !_rh_sensor_valid ) return false;

  uint32_t _tmp = (uint32_t)_rh_sensor;
  // from Adafruit SHT31
  // humidity = (shum * 100.0f) / 65536.0f;
  // 100.0 x _rh = val x 100 x 100 / ( 4096 x 16 )
  _tmp = (625 * _tmp) >> 12;
  *pval = (float)_tmp / 100.0f;

  _rh_sensor = false;

  return true;
}


/* ------------------------------------------------------------------------------
 * Private methods and attributes
 */

/*
 * Software reset through I2C command
 */
void SCD4x::sw_reset( uint8_t adr ) {
  log_debug(F("\n[SCD4x] SOFT RESET action started ..."));log_flush();
  _writeCmd( adr, static_cast<uint16_t>(scd4xCmd_t::reinit) );
  delay( 20 );

  _periodic_measure = false;
}


/*
 * Read 16bits sensors values and check CRCs
 * Note: SCD4x sensors read CO2, T and RH, all feature CRC
 * Once a new data is available, we store it within the proper
 * variable and we set its companion flag accordingly
 */
bool SCD4x::_readSensor( void ) {

  uint8_t _retry = 3;
  bool status = false;

  while( not status and _retry-- ) {
    // cmd for read out serial number
    _writeCmd( _i2caddr, static_cast<uint16_t>(scd4xCmd_t::read_measurement) );

    // read answer 3 * (MSB + LSB + CRC)
    uint8_t buf[9];
    if( readList_ll( _i2caddr, buf, sizeof(buf)) < sizeof(buf) ) {
      log_error(F("\n[SCD4x] not enough bytes read !"));log_flush();
      delay(10); // 1ms min. delay between commands
      continue; // ... and let's retry ...
    }

    // we don't know about bytes validity but we've been able to read
    // measurement from sensor :)
    _lastData = millis();
    status = true;

    // DEBUG
    hex_dump( (char*)buf, sizeof(buf) );
    break;

    // CO2 + CRC
    if( crc_check(buf, 2, buf[2]) ) {
      // CO2 retrieval
      _co2_sensor = makeWord(buf[0], buf[1]);
      _co2_sensor_valid = true;
    }
    else {
      log_error(F("\n[SCD4x] CO2 CRC check failed !"));log_flush();
      status = false;
    }

    // T + CRC
    if( crc_check(buf+3, 2, buf[5]) ) {
      // T retrieval
      _t_sensor = makeWord(buf[3], buf[4]);
      _t_sensor_valid = true;
    }
    else {
      log_error(F("\n[SCD4x] Temp CRC check failed !"));log_flush();
      status = false;
    }

    // RH + CRC
    if( crc_check(buf+6, 2, buf[8]) ) {
      // RH retrieval
      _rh_sensor = makeWord(buf[6], buf[7]);
      _rh_sensor_valid = true;
    }
    else {
      log_error(F("\n[SCD4x] R.H CRC check failed !"));log_flush();
      status = false;
    }

    // this is the end ... The Doors
    break;
  }

  return status;
}


/*
 * CRC related attributes
 */
const uint8_t SCD4x::_crc8_polynom = 0x31;

#ifdef SCD4X_CRC_LOOKUP_TABLE
// CRC8 lookup table
const uint8_t SCD4x::_crc8_table[256] PROGMEM = {
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
bool SCD4x::crc_check( uint8_t data[], uint8_t nb_bytes, uint8_t checksum ) {
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
bool SCD4x::crc_check( uint8_t data[], uint8_t nb_bytes, uint8_t checksum ) {
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
#endif /* SCD4X_CRC_LOOKUP_TABLE */


/*
 * Write i2c command
 */
void SCD4x::_writeCmd( uint8_t a, uint16_t cmd ) {
  write8( a, (uint8_t)(cmd>>8) , (uint8_t)(cmd&0xFF) );
  delay(1); // 1ms min between i2c transactions
}


/*
 * Check if read_measurement is over or not
 * Note: support for only one check per second
 */
bool SCD4x::_isDataReady( void ) {

  unsigned long _curTime = millis();
  if( (_curTime - _lastCheck) < (unsigned long)(SCD4X_DATA_CHECK_MS) ) return false;
  _lastCheck = _curTime;

  if( (_curTime -_lastData) >= (unsigned long)(SCD4X_DATA_TIMEOUT)*1000UL ) {
    // TIMEOUT in data arrival detected !!
    log_warning(F("\n[SCD4x] data TIMEOUT ?!?! ... restart the chip:s"));log_flush();
    sw_reset( _i2caddr );
    return false;
  }

  uint8_t _retry = 3;
  bool status = false;

  while( not status and _retry-- ) {
    // cmd for getting measurement status
    _writeCmd( _i2caddr, static_cast<uint16_t>(scd4xCmd_t::get_data_ready_status) );

    // read answer (MSB + LSB + CRC)
    uint8_t buf[3];
    if( readList_ll(_i2caddr, buf, sizeof(buf)) < sizeof(buf) ) {
      log_error(F("\n[SCD4x] not enough bytes read !"));log_flush();
      delay(10); // 1ms is min. delay between commands
      continue;
    }

    // check answer's CRC
    if( not crc_check(buf, 2, buf[2]) ) {
      log_error(F("\n[SCD4x] CRC check failed !"));log_flush();
      delay(10); // 1ms is min. delay between commands
      continue;
    }

    // then check status
    uint16_t value = makeWord(buf[0], buf[1]);
    if( value & (uint16_t)0x07FF ) status = true;
    break;
  }

  return status;
}


/*
 * Check that device identity is what we expect!
 */
bool SCD4x::_check_identity( uint8_t a ) {

  uint8_t _retry = 3;
  bool status = false;

  while( not status and _retry-- ) {
    // cmd for read out serial number
    _writeCmd( a, static_cast<uint16_t>(scd4xCmd_t::get_serial_number) );

    // read answer 3 * (MSB + LSB + CRC)
    uint8_t buf[9];
    if( readList_ll(a, buf, sizeof(buf)) < sizeof(buf) ) {
      log_error(F("\n[SCD4x] not enough bytes read !"));log_flush();
      delay(10); // 1ms is min. delay between commands
      sw_reset( a ); delay(10);
      continue;
    }

    // DEBUG
    hex_dump( (char*)buf, sizeof(buf) );
    break;

    // check answer's CRC
    if( not crc_check(buf, 2, buf[2]) ) {
      log_error(F("\n[SCD4x] CRC check failed !"));log_flush();
      delay(10); // 1ms is min. delay between commands
      continue;
    }
    if( not crc_check(buf+3, 2, buf[5]) ) {
      log_error(F("\n[SCD4x] CRC check failed !"));log_flush();
      delay(10); // 1ms is min. delay between commands
      continue;
    }
    if( not crc_check(buf+6, 2, buf[8]) ) {
      log_error(F("\n[SCD4x] CRC check failed !"));log_flush();
      delay(10); // 1ms is min. delay between commands
      continue;
    }
  
    // DISPLAY retrieved serial number :)
    {
      char tmp[48];
      snprintf(tmp, sizeof(tmp),"\n[SCD4x] serial number = 0x%02X%02X %02X%02X %02X%02X",
                buf[7],buf[6],buf[4],buf[3],buf[1],buf[0]);
      log_debug(tmp); log_flush();
    }

    status = true;
  }
  // check result
  if( not status ) return false;

  // additional tests here

  return true;
}
