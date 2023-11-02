/*******************************/
/*********** INCLUDES***********/
/*******************************/
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
      uint8_t *pTemp = (uint8_t *)&device.getManufacturerData().c_str()[1];
      return ((float)*pTemp) /10;
    }

        // ***** getHumidity *****
    uint8_t getHumidity(BLEAdvertisedDevice device) {
      const uint8_t *pHum = (const uint8_t *)&device.getManufacturerData()
                                      .c_str()[3];
      return *pHum;
    }
   };
    
void setup() {
  //init serial port
  setupSerial();

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
