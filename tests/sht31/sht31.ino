/* Simple SHT31 test
  Rob Tillaart https://github.com/RobTillaart/SHT31
 */

#include "Arduino.h"
#include <Wire.h>


//---------- Defines -----------------------------------------------------------
#define SHT31_ADDRESS   0x44    // could be either 0x44 0x45


//
//    FILE: SHT31.h
//  AUTHOR: Rob Tillaart
// VERSION: 0.3.4
//    DATE: 2019-02-08
// PURPOSE: Arduino library for the SHT31 temperature and humidity sensor
//          https://www.adafruit.com/product/2857
//     URL: https://github.com/RobTillaart/SHT31
//
#define SHT31_LIB_VERSION             (F("0.3.4"))

// fields readStatus
#define SHT31_STATUS_ALERT_PENDING    (1 << 15)
#define SHT31_STATUS_HEATER_ON        (1 << 13)
#define SHT31_STATUS_HUM_TRACK_ALERT  (1 << 11)
#define SHT31_STATUS_TEMP_TRACK_ALERT (1 << 10)
#define SHT31_STATUS_SYSTEM_RESET     (1 << 4)
#define SHT31_STATUS_COMMAND_STATUS   (1 << 1)
#define SHT31_STATUS_WRITE_CRC_STATUS (1 << 0)

// error codes
#define SHT31_OK                      0x00
#define SHT31_ERR_WRITECMD            0x81
#define SHT31_ERR_READBYTES           0x82
#define SHT31_ERR_HEATER_OFF          0x83
#define SHT31_ERR_NOT_CONNECT         0x84
#define SHT31_ERR_CRC_TEMP            0x85
#define SHT31_ERR_CRC_HUM             0x86
#define SHT31_ERR_CRC_STATUS          0x87
#define SHT31_ERR_HEATER_COOLDOWN     0x88
#define SHT31_ERR_HEATER_ON           0x89

// SUPPORTED COMMANDS - single shot mode only
#define SHT31_READ_STATUS       0xF32D
#define SHT31_CLEAR_STATUS      0x3041

#define SHT31_SOFT_RESET        0x30A2
#define SHT31_HARD_RESET        0x0006

#define SHT31_MEASUREMENT_FAST  0x2416    // page 10 datasheet
#define SHT31_MEASUREMENT_SLOW  0x2400    // no clock stretching

#define SHT31_HEAT_ON           0x306D
#define SHT31_HEAT_OFF          0x3066
#define SHT31_HEATER_TIMEOUT    180000UL  // milliseconds

class SHT31
{
public:
  SHT31();

#if defined(ESP8266) || defined(ESP32)
  bool begin(const uint8_t address, uint8_t dataPin, uint8_t clockPin);
#endif
  bool begin(const uint8_t address);
  bool begin(const uint8_t address,  TwoWire *wire);

  // blocks 15 milliseconds + actual read + math
  bool read(bool fast = true);

  // check sensor is reachable over I2C
  bool isConnected();

  // details see datasheet; summary in SHT31.cpp file
  uint16_t readStatus();

  // lastRead is in milliSeconds since start
  uint32_t lastRead() { return _lastRead; };

  bool reset(bool hard = false);

  // do not use heater for long periods,
  // use it for max 3 minutes to heat up
  // and let it cool down at least 3 minutes.
  void setHeatTimeout(uint8_t seconds);
  uint8_t getHeatTimeout() { return _heatTimeout; };
;
  bool heatOn();
  bool heatOff();
  bool isHeaterOn();  // is the sensor still heating up?
  bool heatUp() { return isHeaterOn(); };   // will be obsolete in future

  float getHumidity() { return rawHumidity * (100.0 / 65535); };
  float getTemperature() { return rawTemperature * (175.0 / 65535) - 45; };
  uint16_t getRawHumidity() {return rawHumidity ; };
  uint16_t getRawTemperature() {return rawTemperature; };

  // ASYNC INTERFACE
  bool requestData();
  bool dataReady();
  bool readData(bool fast = true);

  int getError(); // clears error flag

private:
  uint8_t crc8(const uint8_t *data, uint8_t len);
  bool writeCmd(uint16_t cmd);
  bool readBytes(uint8_t n, uint8_t *val);
  TwoWire* _wire;

  uint8_t   _address;
  uint8_t   _heatTimeout;   // seconds
  uint32_t  _lastRead;
  uint32_t  _lastRequest;   // for async interface
  uint32_t  _heaterStart;
  uint32_t  _heaterStop;
  bool      _heaterOn;

  uint16_t rawHumidity;
  uint16_t rawTemperature;

  uint8_t _error;
};


//==============================================================================
// PUBLIC
//==============================================================================
SHT31::SHT31()
{
  _address        = 0;
  _lastRead       = 0;
  rawTemperature  = 0;
  rawHumidity     = 0;
  _heatTimeout    = 0;
  _heaterStart    = 0;
  _heaterStop     = 0;
  _heaterOn       = false;
  _error          = SHT31_OK;
}


#if defined(ESP8266) || defined(ESP32)
bool SHT31::begin(const uint8_t address, const uint8_t dataPin, const uint8_t clockPin)
{
  if ((address != 0x44) && (address != 0x45))
  {
    return false;
  }
  _address = address;

  _wire = &Wire;
  if ((dataPin < 255) && (clockPin < 255))
  {
    _wire->begin(dataPin, clockPin);
  } else {
    _wire->begin();
  }
  return reset();
}
#endif


bool SHT31::begin(const uint8_t address)
{
  return begin(address, &Wire);
}


bool SHT31::begin(const uint8_t address,  TwoWire *wire)
{
  if ((address != 0x44) && (address != 0x45))
  {
    return false;
  }
  _address = address;
  _wire    = wire;
  _wire->begin();
  return reset();
}


bool SHT31::read(bool fast)
{
  if (writeCmd(fast ? SHT31_MEASUREMENT_FAST : SHT31_MEASUREMENT_SLOW) == false)
  {
    return false;
  }
  delay(fast ? 4 : 15); // table 4 datasheet
  return readData(fast);
}


bool SHT31::isConnected()
{
  _wire->beginTransmission(_address);
  int rv = _wire->endTransmission();
  if (rv != 0) _error = SHT31_ERR_NOT_CONNECT;
  return (rv == 0);
}

#ifdef doc
// bit - description
// ==================
// 15 Alert pending status
//    '0': no pending alerts
//    '1': at least one pending alert - default
// 14 Reserved ‘0’
// 13 Heater status
//    '0’ : Heater OFF - default
//    '1’ : Heater ON
// 12 Reserved '0’
// 11 Humidity tracking alert
//    '0’ : no alert - default
//    '1’ : alert
// 10 Temp tracking alert
//    '0’ : no alert - default
//    '1’ : alert
// 9:5 Reserved '00000’
// 4 System reset detected
//    '0': no reset since last ‘clear status register’ command
//    '1': reset detected (hard or soft reset command or supply fail) - default
// 3:2 Reserved ‘00’
// 1 Command status
//    '0': last cmd executed successfully
//    '1': last cmd not processed. Invalid or failed checksum
// 0 Write data checksum status
//    '0': checksum of last write correct
//    '1': checksum of last write transfer failed
#endif


uint16_t SHT31::readStatus()
{
  uint8_t status[3] = { 0, 0, 0 };
  // page 13 datasheet
  if (writeCmd(SHT31_READ_STATUS) == false)
  {
    return 0xFFFF;
  }
  // 16 bit status + CRC
  if (readBytes(3, (uint8_t*) &status[0]) == false)
  {
    return 0xFFFF;
  }

  if (status[2] != crc8(status, 2)) 
  {
    _error = SHT31_ERR_CRC_STATUS;
    return 0xFFFF;
  }

  return (uint16_t) (status[0] << 8) + status[1];
}


bool SHT31::reset(bool hard)
{
  bool b = writeCmd(hard ? SHT31_HARD_RESET : SHT31_SOFT_RESET);
  if (b == false)
  {
    return false;
  }
  delay(1);  // table 4 datasheet
  return true;
}


void SHT31::setHeatTimeout(uint8_t seconds)
{
  _heatTimeout = seconds;
  if (_heatTimeout > 180) _heatTimeout = 180;
}


bool SHT31::heatOn()
{
  if (isHeaterOn()) return true;
  if ((_heaterStop > 0) && (millis() - _heaterStop < SHT31_HEATER_TIMEOUT))
  {
    _error = SHT31_ERR_HEATER_COOLDOWN;
    return false;
  }
  if (writeCmd(SHT31_HEAT_ON) == false)
  {
    _error = SHT31_ERR_HEATER_ON;
    return false;
  }
  _heaterStart = millis();
  _heaterOn    = true;
  return true;
}


bool SHT31::heatOff()
{
  // always switch off the heater - ignore _heaterOn flag.
  if (writeCmd(SHT31_HEAT_OFF) == false)
  {
    _error = SHT31_ERR_HEATER_OFF;  // can be serious!
    return false;
  }
  _heaterStop = millis();
  _heaterOn   = false;
  return true;
}


bool SHT31::isHeaterOn()
{
  if (_heaterOn == false)
  {
    return false;
  }
  // did not exceed time out
  if (millis() - _heaterStart < (_heatTimeout * 1000UL))
  {
    return true;
  }
  heatOff();
  return false;
}


bool SHT31::requestData()
{
  if (writeCmd(SHT31_MEASUREMENT_SLOW) == false)
  {
    return false;
  }
  _lastRequest = millis();
  return true;
}


bool SHT31::dataReady()
{
  return ((millis() - _lastRequest) > 15);  // TODO MAGIC NR
}


bool SHT31::readData(bool fast)
{
  uint8_t buffer[6];
  if (readBytes(6, (uint8_t*) &buffer[0]) == false)
  {
    return false;
  }

  if (!fast)
  {
    if (buffer[2] != crc8(buffer, 2)) 
    {
      _error = SHT31_ERR_CRC_TEMP;
      return false;
    }
    if (buffer[5] != crc8(buffer + 3, 2)) 
    {
      _error = SHT31_ERR_CRC_HUM;
      return false;
    }
  }

  rawTemperature = (buffer[0] << 8) + buffer[1];
  rawHumidity    = (buffer[3] << 8) + buffer[4];

  _lastRead = millis();

  return true;
}


int SHT31::getError()
{
  int rv = _error;
  _error = SHT31_OK;
  return rv;
}


//////////////////////////////////////////////////////////

uint8_t SHT31::crc8(const uint8_t *data, uint8_t len) 
{
  // CRC-8 formula from page 14 of SHT spec pdf
  const uint8_t POLY(0x31);
  uint8_t crc(0xFF);

  for (uint8_t j = len; j; --j) 
  {
    crc ^= *data++;

    for (uint8_t i = 8; i; --i) 
    {
      crc = (crc & 0x80) ? (crc << 1) ^ POLY : (crc << 1);
    }
  }
  return crc;
}


bool SHT31::writeCmd(uint16_t cmd)
{
  _wire->beginTransmission(_address);
  _wire->write(cmd >> 8 );
  _wire->write(cmd & 0xFF);
  if (_wire->endTransmission() != 0)
  {
    _error = SHT31_ERR_WRITECMD;
    return false;
  }
  return true;
}


bool SHT31::readBytes(uint8_t n, uint8_t *val)
{
  int rv = _wire->requestFrom(_address, (uint8_t) n);
  if (rv == n)
  {
    for (uint8_t i = 0; i < n; i++)
    {
      val[i] = _wire->read();
    }
    return true;
  }
  _error = SHT31_ERR_READBYTES;
  return false;
}



/* Declarations */

/* Global variables */
SHT31 sht;
float temp;     // variable to store temperature
float humidity; // variable to store hemidity



/*
 * SETUP
 */
void setup() {
  delay(5000);
  Serial.begin(115200);
  Serial.println(F("\n\n\n[SHT31] demo ..."));Serial.flush();
  delay(1000);

  Wire.begin();    // begin Wire
  sht.begin(SHT31_ADDRESS);
  delay(10);

  sht.heatOff();

  uint16_t stat = sht.readStatus();
  Serial.print(stat, HEX);
  Serial.println();
}

/*
 * LOOP
 */
void loop() {

  sht.read(false);
  temp = sht.getTemperature();  // get temp from SHT 
  humidity = sht.getHumidity(); // get temp from SHT

  Serial.print("Temp: ");      // print readings
  Serial.print(temp);
  Serial.print("\t Humidity: ");
  Serial.println(humidity);
  Serial.flush();

  delay(3000);  // min delay for 14bit temp reading is 85ms
}
