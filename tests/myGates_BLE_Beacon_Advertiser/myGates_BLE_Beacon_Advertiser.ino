#include <M5StickCPlus.h>
#include "BLEDevice.h"
#include "BLEUtils.h"
#include "BLEBeacon.h"

#define ADVERTISMENT_INTERVAL     500  //(ms) advertise every 0.5 second
#define ADVERTISMENT_DURATION     100  //(ms)
#define BEACON_UUID               "DEADDEAD-F88F-0042-F88F-010203040506" //same UUID for all vehicles
#define UT3_AUTONOMOUS_VEHICLE    0x0042
#define EASYMILE_EZ10             0x0000
#define FORCE_GATE_OPEN           0b0100000000000000
#define CLEAR_GATE_CALIBRATION    0b1000000000000000
#define TX_POWER                  ESP_PWR_LVL_N14



#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

BLEAdvertising *pAdvertising; // BLE Advertisement type

bool send_force_gate_frame = false;
uint16_t MAJOR;
uint16_t MINOR;

void setBeacon() {
  //configure ibeacon data
  MAJOR = UT3_AUTONOMOUS_VEHICLE;
  MINOR = EASYMILE_EZ10;
  if(send_force_gate_frame){ // if button A was pressed, send FORCE_GATE_OPEN trame
    MINOR = (MINOR | FORCE_GATE_OPEN);
    send_force_gate_frame = false;
    Serial.println("forcing gate to open");
  }

  
  BLEBeacon oBeacon = BLEBeacon();
  oBeacon.setManufacturerId(0x4C00); // fake Apple 0x004C LSB (ENDIAN_CHANGE_U16!)
  oBeacon.setProximityUUID(BLEUUID(BEACON_UUID));
  oBeacon.setMajor(MAJOR);
  oBeacon.setMinor(MINOR);
  oBeacon.setSignalPower(TX_POWER);
  BLEAdvertisementData oAdvertisementData = BLEAdvertisementData();
  BLEAdvertisementData oScanResponseData = BLEAdvertisementData();

  oAdvertisementData.setFlags(0x04);

  std::string strServiceData = "";

  strServiceData += (char)26;     // Len
  strServiceData += (char)0xFF;   // Type
  strServiceData += oBeacon.getData(); 
  oAdvertisementData.addData(strServiceData);

  pAdvertising->setAdvertisementData(oAdvertisementData);
  pAdvertising->setScanResponseData(oScanResponseData);

}
#define LED 10

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(M5_BUTTON_HOME, INPUT_PULLUP);
  digitalWrite(LED, HIGH);
  M5.begin();
  M5.Lcd.setRotation(1);
  M5.Lcd.fillScreen(BLACK);

  M5.Lcd.setCursor(2, 0, 2);
  M5.Lcd.printf("UUID: %s",BEACON_UUID);
 /* M5.Lcd.setCursor(2, 40, 2);
  M5.Lcd.printf("Major %d Minor %d",MAJOR ,MINOR);  */

  Serial.begin(115200);

  // Create the BLE Device
  BLEDevice::init("navette neOCampus");
  pAdvertising = BLEDevice::getAdvertising();

  // Increase TX POWER
  //BLEDevice::setPower(TX_POWER);
  esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_DEFAULT, TX_POWER);
  esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_ADV, TX_POWER);

    // Create the BLE Server
  BLEServer *pServer = BLEDevice::createServer();
  pAdvertising = BLEDevice::getAdvertising();
  BLEDevice::startAdvertising();
  
  setBeacon();
}

void loop() {
  M5.update();  // Read the press state of the key
  if (M5.BtnA.wasReleased()) {  // If the button A is pressed
    send_force_gate_frame = true;
    Serial.println("button A was pressed");
  }
  setBeacon();
  pAdvertising->start();
  digitalWrite(LED, LOW);  
  Serial.printf("Advertizing started...\n");
  delay(ADVERTISMENT_DURATION);
  Serial.println("Advertizing stop...");
  pAdvertising->stop();
  digitalWrite(LED, HIGH);  
  delay(ADVERTISMENT_INTERVAL - ADVERTISMENT_DURATION);
}
