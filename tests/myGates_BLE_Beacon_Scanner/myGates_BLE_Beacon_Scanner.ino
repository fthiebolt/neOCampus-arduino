#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <BLEBeacon.h>
#include "sys/time.h"
#include <time.h>

#define ENDIAN_CHANGE_U16(x) ((((x)&0xFF00) >> 8) + (((x)&0xFF) << 8))
#define HIGH2BITS_U16(x) (((x)&0b1100000000000000) >> 14)

#define NAVETTE_UUID          "DEADDEAD-F88F-0042-F88F-010203040506" // same UUID for all vehicles
#define FORCEGATEOPEN         0b01 // minor high bits = 0b01 => force gate to open
#define CLEARGATECALIBRATION  0b11 // minor high bits = 0b11 => clear gate calibration
#define SCAN_TIME             20 // scan period in second



class IBeaconAdvertised: public BLEAdvertisedDeviceCallbacks {
  public:
    // BLE
    void onResult(BLEAdvertisedDevice device) {
      // check if iBeacon Tram
      if (!isIBeacon(device)) {
        return;
      }
      // check for NAVETTE_UUID
      if (getProxyUuid(device).equals(BLEUUID(NAVETTE_UUID))){
        printIBeacon(device);

        //received a trame FORCE GATE OPEN
        if (HIGH2BITS_U16(getMinor(device))==FORCEGATEOPEN){
          openGate();
        }
      }
    }

  private:
  
    struct timeval tv;
    time_t t;
    struct tm *info;
    char buffer[64];
  
    // open Gate
    void openGate() {
      Serial.println("OPENING GATE");
    }
  
    // is frame iBeacon ?
    bool isIBeacon(BLEAdvertisedDevice device) {
      if (device.getManufacturerData().length() < 25) {
        return false;
      }
      if (getCompanyId(device) != 0x004C) {
        return false;
      }
      if (getIBeaconHeader(device) != 0x1502) {
        return false;
      }
      return true;
    }

    // CompanyId
    unsigned short getCompanyId(BLEAdvertisedDevice device) {
      const unsigned short* pCompanyId = (const unsigned short*)&device
                                         .getManufacturerData()
                                         .c_str()[0];
      return *pCompanyId;
    }

    // iBeacon Header
    unsigned short getIBeaconHeader(BLEAdvertisedDevice device) {
      const unsigned short* pHeader = (const unsigned short*)&device
                                      .getManufacturerData()
                                      .c_str()[2];
      return *pHeader;
    }

    // iBEACON UUID
    BLEUUID getProxyUuid(BLEAdvertisedDevice device) {
      BLEUUID uuid;
      std::string strManufacturerData = device.getManufacturerData();
      uint8_t cManufacturerData[100];
      strManufacturerData.copy((char *)cManufacturerData, strManufacturerData.length(), 0);
      BLEBeacon oBeacon = BLEBeacon();
      oBeacon.setData(strManufacturerData);
      uuid = oBeacon.getProximityUUID();
      return uuid;
     }

    // iBEACON Major
    uint16_t getMajor(BLEAdvertisedDevice device) {
      std::string strManufacturerData = device.getManufacturerData();
      uint8_t cManufacturerData[100];
      strManufacturerData.copy((char *)cManufacturerData, strManufacturerData.length(), 0);
      BLEBeacon oBeacon = BLEBeacon();
      oBeacon.setData(strManufacturerData);
      return ENDIAN_CHANGE_U16(oBeacon.getMajor());
    }

    // iBEACON Minor
    uint16_t getMinor(BLEAdvertisedDevice device) {
      std::string strManufacturerData = device.getManufacturerData();
      uint8_t cManufacturerData[100];
      strManufacturerData.copy((char *)cManufacturerData, strManufacturerData.length(), 0);
      BLEBeacon oBeacon = BLEBeacon();
      oBeacon.setData(strManufacturerData);
      return ENDIAN_CHANGE_U16(oBeacon.getMinor());
    }

    // iBEACON TxPower
    int8_t getTxPower(BLEAdvertisedDevice device) {
      std::string strManufacturerData = device.getManufacturerData();
      uint8_t cManufacturerData[100];
      strManufacturerData.copy((char *)cManufacturerData, strManufacturerData.length(), 0);
      BLEBeacon oBeacon = BLEBeacon();
      oBeacon.setData(strManufacturerData);
      return oBeacon.getSignalPower();
    }

    // print iBeacon device info
    void printIBeacon(BLEAdvertisedDevice device) {
      gettimeofday(&tv, NULL);
      Serial.printf("time:%d name:%s uuid:%s major:%d minor:%d rssi:%d\r\n",
                    (tv.tv_sec),
                    device.getName().c_str(),
                    getProxyUuid(device).toString().c_str(),
                    getMajor(device),
                    getMinor(device),
                    device.getRSSI()
                    );
    }
};

void setup() { 
  Serial.begin(115200);
  BLEDevice::init("");
}

void loop() {
  BLEScan* scan = BLEDevice::getScan();
  scan->setAdvertisedDeviceCallbacks(new IBeaconAdvertised(), true);
  scan->setActiveScan(true);
  scan->start(SCAN_TIME);
}
