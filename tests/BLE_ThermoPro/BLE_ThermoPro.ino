/*******************************/
/*********** INCLUDES***********/
/*******************************/
#include <M5StickCPlus.h>
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

/*******************************/
/********* DEFINITIONS *********/
/*******************************/

#define SCAN_TIME 5                   // scan period in second
bool BLEScanActivated = false;
#define ENDIAN_CHANGE_U16(x) ((((x)&0xFF00) >> 8) + (((x)&0xFF) << 8))
/*****************************/
/********* FONCTIONS *********/
/*****************************/

// ***** set up Serial Port *****
void setupSerial() {
#ifdef SERIAL_BAUDRATE
  delay(3000);                    // time for USB serial link to come up anew
  Serial.begin(SERIAL_BAUDRATE);  // Start serial for output
  Serial.setDebugOutput(true);
#endif
}

class Advertised : public BLEAdvertisedDeviceCallbacks {
  public:
  void onResult(BLEAdvertisedDevice device) {
    printBLE(device);
    return;
    }
  
  private:
    // ***** print BLE device info *****
    void printBLE(BLEAdvertisedDevice device) {
      if(device.getName() =="TP358 (5551)"){
        Serial.printf("Device:%s \n",device.toString().c_str() );
        Serial.printf("Température:%f°C , Humidité:%d \n",getTemperature(device), getHumidity(device));
      }
    }
      
    // ***** getTemperature *****
    float getTemperature(BLEAdvertisedDevice device) {
      
      uint8_t temp_lb = (uint8_t) device.getManufacturerData().c_str()[1]; // temperature lower byte 
      uint8_t temp_hb = (uint8_t)(device.getManufacturerData().c_str()[2]); // temperature higher byte 
      int16_t temperature = (int16_t)((uint16_t)(temp_hb)<<8) + (uint16_t)temp_lb;
      return ((float)(temperature)/10);
      
      return ((float)(temperature));
    }

        // ***** getHumidity *****
    uint8_t getHumidity(BLEAdvertisedDevice device) {
      const uint8_t *pHum = (const uint8_t *)&device.getManufacturerData().c_str()[3];
      return *pHum;
    }
   };
    
void setup() {
  //init serial port
  setupSerial();
  M5.begin();
  //init BLE
  BLEDevice::init("");
  Serial.printf("Starting \n");

}

// ***** start BLE scan  *****
void startScanBLE() {
  BLEScan* pBLEScan = BLEDevice::getScan(); // start scanning
  pBLEScan->setAdvertisedDeviceCallbacks(new Advertised(), true);
  pBLEScan->setActiveScan(true);
  pBLEScan->start(SCAN_TIME, scanCompleteCB);
  BLEScanActivated = true;
}

// ***** scan Complete Call Back  *****
static void scanCompleteCB(BLEScanResults scanResults) { // Callback invoked when scanning has completed
  BLEScanActivated = false;
}

// ***** manage BLE scan  *****
void manageBLEscan() {
  if (!BLEScanActivated)
  startScanBLE(); // start scanning
}



void loop() {

  manageBLEscan();
  delay(1000);
}
