/* Simple SHT21 test
   https://github.com/e-radionicacom/SHT21-Arduino-Library
 */

#include "Arduino.h"
#include <Wire.h>


//---------- Defines -----------------------------------------------------------
#define I2C_ADD 0x40  // I2C device address

const uint16_t POLYNOMIAL = 0x131;  // P(x)=x^8+x^5+x^4+1 = 100110001

//==============================================================================
#define TRIGGER_T_MEASUREMENT_HM 0XE3   // command trig. temp meas. hold master
#define TRIGGER_RH_MEASUREMENT_HM 0XE5  // command trig. hum. meas. hold master
#define TRIGGER_T_MEASUREMENT_NHM 0XF3  // command trig. temp meas. no hold master
#define TRIGGER_RH_MEASUREMENT_NHM 0XF5 // command trig. hum. meas. no hold master
#define USER_REGISTER_W 0XE6        // command writing user register
#define USER_REGISTER_R 0XE7            // command reading user register
#define SOFT_RESET 0XFE                 // command soft reset
//==============================================================================
// HOLD MASTER - SCL line is blocked (controlled by sensor) during measurement
// NO HOLD MASTER - allows other I2C communication tasks while sensor performing
// measurements.


class SHT21
{
  private:
    //==============================================================================
    uint16_t readSensor_hm(uint8_t command);
    //==============================================================================
    // reads SHT21 with hold master operation mode
    // input: temp/hum command
    // return:  temp/hum raw data (16bit scaled)
    

    //==============================================================================
    float CalcRH(uint16_t rh);
    //==============================================================================
    // calculates the relative humidity
    // input:  rh:   humidity raw value (16bit scaled)
    // return:     relative humidity [%RH] (float)

    //==============================================================================
    float CalcT(uint16_t t);
    //==============================================================================
    // calculates the temperature
    // input:  t:   temperature raw value (16bit scaled)
    // return:    relative temperature [°C] (float)

    //==============================================================================
    uint8_t CRC_Checksum(uint8_t data[], uint8_t no_of_bytes, uint8_t checksum);
    //==============================================================================
    // CRC-8 checksum for error detection
    // input:  data[]       checksum is built based on this data
    //         no_of_bytes  checksum is built for n bytes of data
    //         checksum     expected checksum
    // return:              1          = checksum does not match
    //                      0              = checksum matches

    
  public:
    //==============================================================================
    float getHumidity(void);
    //==============================================================================
    // calls humidity measurement with hold master mode

    //==============================================================================
    float getTemperature(void);
    //==============================================================================
    // calls temperature measurement with hold master mode

    //==============================================================================
    void reset();
    //==============================================================================
    // performs a soft reset, delays 15ms

    //==============================================================================
    uint8_t getSerialNumber(uint8_t return_sn);
    //==============================================================================
    // returns electronical identification code depending of selected memory
    // location

};

//==============================================================================
// PUBLIC
//==============================================================================
float SHT21::getHumidity(void) {
  uint16_t result;  // return variable
  
  //result = readSensor_hm(TRIGGER_RH_MEASUREMENT_HM);
  result = readSensor_hm(TRIGGER_RH_MEASUREMENT_NHM);

  return CalcRH(result);
}

float SHT21::getTemperature(void) {
  uint16_t result;  // return variable
  
  //result = readSensor_hm(TRIGGER_T_MEASUREMENT_HM);
  result = readSensor_hm(TRIGGER_T_MEASUREMENT_NHM);

  return CalcT(result);
}

void SHT21::reset() {
  Wire.beginTransmission(I2C_ADD);
  Wire.write(SOFT_RESET);
  Wire.endTransmission();

  delay(15);  // wait for SHT to reset
}

uint8_t SHT21::getSerialNumber(uint8_t return_sn) {

  uint8_t serialNumber[8];

  // read memory location 1
  Wire.beginTransmission(I2C_ADD);
  Wire.write(0xFA);
  Wire.write(0x0F);
  Wire.endTransmission();

  Wire.requestFrom(I2C_ADD,8);
  while(Wire.available() < 8) {}

  serialNumber[5] = Wire.read();  // read SNB_3
  Wire.read();          // CRC SNB_3 not used
  serialNumber[4] = Wire.read();  // read SNB_2
  Wire.read();          // CRC SNB_2 not used
  serialNumber[3] = Wire.read();  // read SNB_1
  Wire.read();          // CRC SNB_1 not used
  serialNumber[2] = Wire.read();  // read SNB_0
  Wire.read();          // CRC SNB_0 not used

  // read memory location 2
  Wire.beginTransmission(I2C_ADD);
  Wire.write(0xFC);
  Wire.write(0xC9);
  Wire.endTransmission();

  Wire.requestFrom(I2C_ADD,6);
  while(Wire.available() < 6) {}

  serialNumber[1] = Wire.read();  // read SNC_1
  serialNumber[0] = Wire.read();  // read SNC_0
  Wire.read();          // CRC SNC_1/SNC_0 not used
  serialNumber[7] = Wire.read();  // read SNA_1     
  serialNumber[6] = Wire.read();  // read SNA_0
  Wire.read();          // CRC SNA_1/SNA_0 not used

  return serialNumber[return_sn];
}

//==============================================================================
// PRIVATE
//==============================================================================

uint16_t SHT21::readSensor_hm(uint8_t command) {
  uint8_t checksum;
  uint8_t data[2];
  uint16_t result;
  uint8_t n = 0;
  uint8_t d;
  
  if(command == TRIGGER_RH_MEASUREMENT_HM || command == TRIGGER_RH_MEASUREMENT_NHM) d = 30;
  if(command == TRIGGER_T_MEASUREMENT_HM || command == TRIGGER_T_MEASUREMENT_NHM) d = 85;
  
  Wire.beginTransmission(I2C_ADD);
  Wire.write(command);
  Wire.endTransmission( );
  delay(d);
  Wire.requestFrom(I2C_ADD,3);
  
  while(Wire.available() < 3) {
    delay(10);
    n++;
    if(n>10) return 0;
  }

  data[0] = Wire.read();  // read data (MSB)
  data[1] = Wire.read();  // read data (LSB)
  checksum = Wire.read(); // read checksum

  result = (data[0] << 8);
  result += data[1];

  if(CRC_Checksum (data,2,checksum)) {
    reset();
    return 1;
  }

  else return result;
}

float SHT21::CalcRH(uint16_t rh) {

  rh &= ~0x0003;  // clean last two bits

  return (-6.0 + 125.0/65536 * (float)rh); // return relative humidity
}

float SHT21::CalcT(uint16_t t) {

  t &= ~0x0003; // clean last two bits
  return (-46.85 + 175.72/65536 * (float)t);
}

uint8_t SHT21::CRC_Checksum(uint8_t data[], uint8_t no_of_bytes, uint8_t checksum) {
  uint8_t crc = 0;  
  uint8_t byteCtr;

  //calculates 8-Bit checksum with given polynomial
  for (byteCtr = 0; byteCtr < no_of_bytes; ++byteCtr){
    crc ^= (data[byteCtr]);
    for (uint8_t bit = 8; bit > 0; --bit) {
      if (crc & 0x80) crc = (crc << 1) ^ POLYNOMIAL;
      else crc = (crc << 1);
    }
  }
  if (crc != checksum) return 1;
  else return 0;
}


/* Declarations */

/* Global variables */
SHT21 sht; 
float temp;   // variable to store temperature
float humidity; // variable to store hemidity



/*
 * SETUP
 */
void setup() {
  delay(5000);
  Serial.begin(115200);

  Wire.begin();    // begin Wire
  
}

/*
 * LOOP
 */
void loop() {
  temp = sht.getTemperature();  // get temp from SHT 
  humidity = sht.getHumidity(); // get temp from SHT

  Serial.print("Temp: ");      // print readings
  Serial.print(temp);
  Serial.print("\t Humidity: ");
  Serial.println(humidity);
  Serial.flush();

  delay(1000);  // min delay for 14bit temp reading is 85ms
}
