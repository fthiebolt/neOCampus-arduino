/**************************************************************************/
/*! 
    @file     SHT2x.h
    @author   F. Thiebolt
	  @license
	
    This is part of a the neOCampus drivers library.
    SHT2x is humidity + temperature sensor with an I2C interface
    
    Remember: all transfers are 16bits wide
    
    (c) Copyright 2020 Thiebolt.F <thiebolt@irit.fr>
  @note
    Warning, some (fakes?) sensors does not properly set CRC when reading
      sensor's UUID --> removing CRC check for UUID
    It seems too that temperature sensor part does not need correction when
      being tied to the neOSensor board.
	@section  HISTORY

    2020-May    - F.Thiebolt Initial release (UUID's CRC check diasbled)
    
*/
/**************************************************************************/

#include <Arduino.h>

#include "neocampus.h"
#include "neocampus_debug.h"
#include "neocampus_i2c.h"

// [may.20] debug
//#include "neocampus_utils.h"


#include "SHT2x.h"



/**************************************************************************/
/*! 
    @brief  Declare list of possible I2C addrs
    Note: only one i2c addr !
*/
/**************************************************************************/
const uint8_t SHT2x::i2c_addrs[1] = { 0x40 };


/**************************************************************************/
/*! 
    @brief  Test if device at 'adr' is really a MCP9808
*/
/**************************************************************************/
boolean SHT2x::is_device( uint8_t a ) {
  
  // First step, parse all addresses
  boolean found = false;
  for( uint8_t i=0; i < sizeof(SHT2x::i2c_addrs); i++ ) {
    if( SHT2x::i2c_addrs[i] == a) {
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
SHT2x::SHT2x( sht2xMeasureType_t kindness ) : generic_driver() {
  _i2caddr = -1;
  _measureType = kindness;
  _resolution = SHT2X_DEFL_RESOLUTION;
}


/* declare kind of units */
const char *SHT2x::_t_units = "celsius";
const char *SHT2x::_rh_units = "%r.H.";


/**************************************************************************/
/*! 
    @brief  send back units
*/
/**************************************************************************/
const char * SHT2x::sensorUnits( void ) {
  switch( _measureType ) {
    case sht2xMeasureType_t::temperature:
      return _t_units;
      break;
    case sht2xMeasureType_t::humidity:
      return _rh_units;
      break;
    default:
      log_error(F("\n[SHT2x] unknown kind of measureType ?!?")); log_flush();
      return nullptr;
  }
}



/**************************************************************************/
/*! 
    @brief  Setups the HW
*/
/**************************************************************************/
boolean SHT2x::begin( uint8_t addr=INVALID_I2CADDR ) {
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
  setResolution( _resolution );

  return true;
}


/*
 * Power modes: ON or powerOFF
 */
void SHT2x::powerOFF( void )
{
  // device does not feature continuous integration so nothing to start or stop
}

void SHT2x::powerON( void )
{
  // device does not feature continuous integration so nothing to start or stop
}


/**************************************************************************/
/*! 
    @brief  Setups the HW
*/
/**************************************************************************/
bool SHT2x::setResolution( sht2xResolution_t res ) {

  uint8_t userRegVal;

  /* set resolution and timing */
  // RH:12 bits, T:14 bits
  if( res == sht2xResolution_t::res_12_14bits ) {
    userRegVal = static_cast<uint8_t>(sht2xResolution_t::res_12_14bits);
    if( _measureType == sht2xMeasureType_t::humidity )
      _integrationTime = static_cast<uint8_t>(sht2xIntegrationRH_t::ms_integrate_12bits);
    else
      _integrationTime = static_cast<uint8_t>(sht2xIntegrationT_t::ms_integrate_14bits);
  }
  // RH:8 bits, T:12 bits
  else if( res == sht2xResolution_t::res_8_12bits ) {
    userRegVal = static_cast<uint8_t>(sht2xResolution_t::res_8_12bits);
    if( _measureType == sht2xMeasureType_t::humidity )
      _integrationTime = static_cast<uint8_t>(sht2xIntegrationRH_t::ms_integrate_8bits);
    else
      _integrationTime = static_cast<uint8_t>(sht2xIntegrationT_t::ms_integrate_12bits);
  }
  // RH:10 bits, T:13 bits
  else if( res == sht2xResolution_t::res_10_13bits ) {
    userRegVal = static_cast<uint8_t>(sht2xResolution_t::res_10_13bits);
    if( _measureType == sht2xMeasureType_t::humidity )
      _integrationTime = static_cast<uint8_t>(sht2xIntegrationRH_t::ms_integrate_10bits);
    else
      _integrationTime = static_cast<uint8_t>(sht2xIntegrationT_t::ms_integrate_13bits);
  }
  // RH:11 bits, T:11 bits
  else if( res == sht2xResolution_t::res_11_11bits ) {
    userRegVal = static_cast<uint8_t>(sht2xResolution_t::res_11_11bits);
    if( _measureType == sht2xMeasureType_t::humidity )
      _integrationTime = static_cast<uint8_t>(sht2xIntegrationRH_t::ms_integrate_11bits);
    else
      _integrationTime = static_cast<uint8_t>(sht2xIntegrationT_t::ms_integrate_11bits);
  }
  else {
    return false;
  }

  // user register ought to get read first, modified with mask, then written
  uint8_t _curUserReg = read8( _i2caddr, static_cast<uint8_t>(sht2xCmd_t::set_user_reg) );
  _curUserReg &= ~static_cast<uint8_t>(sht2xResolution_t::res_mask);
  userRegVal &= static_cast<uint8_t>(sht2xResolution_t::res_mask);
  userRegVal |= _curUserReg;

  write8( _i2caddr, static_cast<uint8_t>(sht2xCmd_t::set_user_reg), (uint8_t)userRegVal);

  // add some constant to integration time ...
  _integrationTime+=(uint8_t)SHT2X_INTEGRATION_TIME_CTE;

  // finish :)
  return true;
}


/**************************************************************************/
/*! 
    @brief  Reads the 16-bit temperature register and returns the Centigrade
            temperature as a float.

*/
/**************************************************************************/
boolean SHT2x::acquire( float *pval )
{
  if( pval==nullptr ) return false;

  // HUMIDITY
  if( _measureType == sht2xMeasureType_t::humidity ) {
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
  // SHT2x only feature ONE i2c address ... so the last one as default
  *pval += TEMPERATURE_CORRECTION_LASTI2C;
  log_debug(F("\n[SHT2x] corrected temperature for i2c=0x"));log_debug(_i2caddr,HEX);log_flush();
#endif

  return true;
}


/*
 * READ sensor's HUMIDITY
 */
boolean SHT2x::getRH( float *pval ) {

  if( pval==nullptr ) return false;

  uint16_t val;
  bool res = false;
  uint8_t retry = 3;
  
  while( res==false and retry-- ) {
    res = _readSensor_hm( sht2xCmd_t::get_rh_hm, &val );
    if( res == true ) break;
    yield();  // some intrinsic delay before restarting
  }

  if( res==false ) {
    // error ...
    log_error(F("\n[SHT2x] unable to read value ?!?!"));log_flush();
    return false;
  }


  // check that we read proper sensor
  // status bit(1) = 1 for RH, 0 for T
  if( not ((val >> 1) & 0x01) ) {
    log_error(F("\n[SHT2x] wrong sensor read (ought to be RH) ?!?!")); log_flush();
    return false;
  }

  val &= ~0x0003;	// clear status bits
  *pval = (-6.0 + 125.0/65536 * (float)val);
  return true;
}


/*
 * READ sensor's TEMPERATURE
 */
boolean SHT2x::getTemp( float *pval )
{
  if( pval==nullptr ) return false;

  uint16_t val;
  bool res = false;
  uint8_t retry = 3;
  
  while( res==false and retry-- ) {
    res = _readSensor_hm( sht2xCmd_t::get_temp_hm, &val );
    if( res == true ) break;
    yield();  // some intrinsic delay before restarting
  }

  if( res==false ) {
    // error ...
    log_error(F("\n[SHT2x] unable to read value ?!?!"));log_flush();
    return false;
  }

  // check that we read proper sensor
  // status bit(1) = 1 for RH, 0 for T
  if( ((val >> 1) & 0x01) ) {
    log_error(F("\n[SHT2x] wrong sensor read (ought to be T) ?!?!")); log_flush();
    return false;
  }

  val &= ~0x0003;	// clear status bits
  *pval = (-46.85 + 175.72/65536 * (float)val);
  return true;
}


/* ------------------------------------------------------------------------------
 * Private methods and attributes
 */

/*
 * Software reset through I2C command
 */
void SHT2x::sw_reset( uint8_t adr ) {
  log_debug(F("\n[SHT2x] SOFT RESET action started ..."));log_flush();
  write8( adr, static_cast<uint8_t>(sht2xCmd_t::soft_reset) );
  delay( 50 );
}


/*
 * Read 16bits sensor values registers and check CRC
 * Note: Hold Mode means that we won't give back i2c access
 *  prior to fully completed access
 */
bool SHT2x::_readSensor_hm( sht2xCmd_t cmd, uint16_t *pval ) {

  // check command
  if( cmd != sht2xCmd_t::get_temp_hm and cmd != sht2xCmd_t::get_rh_hm ) {
    log_error(F("\n[SHT2x] unsupported command: ")); log_error(static_cast<uint8_t>(cmd),HEX); log_flush();
    return false;
  }

  uint8_t buf[3]; // 16bits data + 8bits CRC

  // read sensor's values register
  if( readList( _i2caddr, static_cast<uint8_t>(cmd), buf, sizeof(buf), _integrationTime ) != sizeof(buf) ) {
    log_error(F("\n[SHT2x] insufficient bytes answered"));log_flush();
    return false;
  }

  // check for CRC
  if( not crc_check( buf, sizeof(buf)-1, buf[sizeof(buf)-1] ) ) {
    log_error(F("\n[SHT2x] invalid CRC received ...")); log_flush();
    sw_reset( _i2caddr );
    return false;
  }

  // arrange val
  *pval = (buf[0] << 8);
  *pval |= buf[1];

  return true;
}


/*
 * CRC related attributes
 */
const uint8_t SHT2x::_crc8_polynom = 0x31;

#ifdef SHT2X_CRC_LOOKUP_TABLE
// CRC16 lookup table
const uint8_t SHT2x::_crc8_table[256] PROGMEM = {
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
bool SHT2x::crc_check( uint8_t data[], uint8_t nb_bytes, uint8_t checksum ) {
	uint8_t crc = 0;	
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
bool SHT2x::crc_check( uint8_t data[], uint8_t nb_bytes, uint8_t checksum ) {
	uint8_t crc = 0;	
  uint8_t byteCtr;

 	//calculates 8-Bit checksum with given polynomial
  for( byteCtr = 0; byteCtr < nb_bytes; ++byteCtr) {
 	  crc ^= (data[byteCtr]);
 	  for (uint8_t bit = 8; bit > 0; --bit) {
 	    if (crc & 0x80) crc = (crc << 1) ^ _crc8_polynom;
 	    else crc = (crc << 1);
 	  }
 	}
  
 	if (crc != checksum) return false;
 	else return true;
}
#endif /* SHT2X_CRC_LOOKUP_TABLE */


/*
 * Serial Number: retrieve sensor's specific serial number (64bits)
 * Note: serial number will get read once
 */
uint64_t SHT2x::getSerialNumber( uint8_t adr ) {

  static uint64_t sn = (uint64_t)(-1);
  
  if( sn == (uint64_t)(-1) ) {

    bool status = false;
    uint8_t retry = 3;
    uint8_t serialNumber[8];

    while( status == false and retry-- ) {
      uint8_t _tab[8];

      // read memory location 1
      write8( adr, 0xFA, 0x0F );

      // now read 8 bytes
      if( readList_ll( adr, _tab, sizeof(_tab) ) < sizeof(_tab) ) {
        log_error(F("\n[SHT2x] unable to read bytes for serial number ... retry"));
        yield();
        sw_reset( adr );
        continue;
      }
      // [may.20] debug
      // hex_dump( (char*)_tab, sizeof(_tab) );

      #ifndef SHT2X_CRC_UUID_DISABLED
      // _tab[1] = CRC SNB_3
      if( not crc_check(&_tab[0], 1, _tab[1]) ) {
        log_error(F("\n[SHT2x][UUID] SNB_3 invalid CRC for 0x"));log_error(_tab[0],HEX);log_flush();
        yield();
        continue;
      }
      #endif /* SHT2X_CRC_UUID_DISABLED */
      serialNumber[5] = _tab[0];  // read SNB_3
      
      #ifndef SHT2X_CRC_UUID_DISABLED
      // _tab[3] = CRC SNB_2
      if( not crc_check(&_tab[2], 1, _tab[3]) ) {
        log_error(F("\n[SHT2x][UUID] SNB_2 invalid CRC for 0x"));log_error(_tab[2],HEX);log_flush();
        yield();
        continue;
      }
      #endif /* SHT2X_CRC_UUID_DISABLED */
      serialNumber[4] = _tab[2];  // read SNB_2

      #ifndef SHT2X_CRC_UUID_DISABLED
      // _tab[5] = CRC SNB_1
      if( not crc_check(&_tab[4], 1, _tab[5]) ) {
        log_error(F("\n[SHT2x][UUID] SNB_1 invalid CRC for 0x"));log_error(_tab[4],HEX);log_flush();
        yield();
        continue;
      }
      #endif /* SHT2X_CRC_UUID_DISABLED */
      serialNumber[3] = _tab[4];  // read SNB_1
      
      #ifndef SHT2X_CRC_UUID_DISABLED
      // _tab[7] = CRC SNB_0
      if( not crc_check(&_tab[6], 1, _tab[7]) ) {
        log_error(F("\n[SHT2x][UUID] SNB_0 invalid CRC for 0x"));log_error(_tab[6],HEX);log_flush();
        yield();
        continue;
      }
      #endif /* SHT2X_CRC_UUID_DISABLED */
      serialNumber[2] = _tab[6];  // read SNB_0

      status = true;  // success
    }
    if( not status ) {
      log_error(F("\n[SHT2x] multiple CRC errors ... give up :("));log_flush();
      return sn;
    }

    // 2nd Part of UUID
    status = false;
    retry = 3;
    while( status == false and retry-- ) {
      uint8_t _tab[6];

      // read memory location 2
      write8( adr, 0xFC, 0xC9 );

      // now read 6 bytes
      if( readList_ll( adr, _tab, sizeof(_tab) ) < sizeof(_tab) ) {
        log_error(F("\n[SHT2x] unable to read bytes for serial number ... retry"));
        yield();
        sw_reset( adr );
        continue;
      }

      #ifndef SHT2X_CRC_UUID_DISABLED
      // _tab[2] = CRC for SNC_0,SNC_1
      if( not crc_check(&_tab[0], 2, _tab[2]) ) {
        log_error(F("\n[SHT2x][UUID] SNC_0,SNC_1 invalid CRC for 0x"));log_error(_tab[0],HEX);log_error(F(" 0x"));log_error(_tab[1],HEX);log_flush();
        yield();
        continue;
      }
      #endif /* SHT2X_CRC_UUID_DISABLED */
      serialNumber[1] = _tab[0];  // read SNC_1
      serialNumber[0] = _tab[1];  // read SNC_0

      #ifndef SHT2X_CRC_UUID_DISABLED
      // _tab[5] = CRC for SNA_0,SNA_1
      if( not crc_check(&_tab[3], 2, _tab[5]) ) {
        log_error(F("\n[SHT2x][UUID] SNA_0,SNA_1 invalid CRC for 0x"));log_error(_tab[3],HEX);log_error(F(" 0x"));log_error(_tab[4],HEX);log_flush();
        yield();
        continue;
      }
      #endif /* SHT2X_CRC_UUID_DISABLED */
      serialNumber[7] = _tab[3];  // read SNA_1
      serialNumber[6] = _tab[4];  // read SNA_0
      status = true;  // success
    }
    if( not status ) {
      log_error(F("\n[SHT2x] multiple CRC errors ... give up :("));log_flush();
      return sn;
    }

    sn = (uint64_t)0;
    for( uint8_t i=0; i < sizeof(serialNumber); i++ ) {
      sn |= (serialNumber[i] << i*8);
    }

    {
      char _msg[255];
      snprintf(_msg, sizeof(_msg), "\n[SHT2x] adr=0x%02x SerialNumber=0x%02x%02x %02x%02x %02x%02x %02x%02x",
                adr, serialNumber[7], serialNumber[6], serialNumber[5], serialNumber[4],
                serialNumber[3], serialNumber[2], serialNumber[1], serialNumber[0]);
      log_debug(_msg);log_flush();
    }
  }

  return sn;
}


/*
 * Check that device identity is what we expect!
 */
bool SHT2x::_check_identity( uint8_t a ) {

  // retrieve serial number
  if( getSerialNumber( a ) == (uint64_t)(-1) ) {
    // we've not been able to read the serial number ... maybe not the device we're expecting
    return false;
  }

  return true;
}
