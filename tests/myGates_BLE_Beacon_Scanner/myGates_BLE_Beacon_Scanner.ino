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

#define STATE_SCAN            1
#define STATE_OPEN_GATE       2

#define RSSI_THRESHOLD_OPEN_GATE  -95

// ***** definitions *****
int STATE;

struct RSSI {   
  int val;          
  int time;
  };
  RSSI tabRSSI[2];

struct timeval tv;
time_t t;

class IBeaconAdvertised: public BLEAdvertisedDeviceCallbacks {
  public:
  
    // BLE
    void onResult(BLEAdvertisedDevice device) {
      // check if iBeacon tram
      if (!isIBeacon(device)) {
        return;
      }
      
      // check for NAVETTE_UUID
      if (!isNavetteUUID(device)){
        return;
      }
      
      // received a trame FORCE GATE OPEN
      if (HIGH2BITS_U16(getMinor(device))==FORCEGATEOPEN){
        printIBeacon(device); // DEBUG
        Serial.printf("  - force open tram\n"); // DEBUG
        openGate();
        updateRSSItab(getRSSI(device));
        return;
      }
        
        //check gate state (STATE_SCAN or STATE_OPEN_GATE)
        switch(STATE) {
          
          case STATE_OPEN_GATE :
            // nothing to do
            printIBeacon(device); // DEBUG
            Serial.printf("  - gate is open\n"); // DEBUG
            updateRSSItab(getRSSI(device)); // TODO : to delete to reopen gate after 4 sec ?
            STATE = STATE_OPEN_GATE;
          break;
          
          case STATE_SCAN :
          default:
          RSSI newRSSI = getRSSI(device);
          int avRSSI = averageRSSI(newRSSI);
            if(avRSSI > RSSI_THRESHOLD_OPEN_GATE) {
              printIBeacon(device); // DEBUG
              Serial.printf("  - RSSI average OK:%d\n",avRSSI); // DEBUG
              openGate();
              updateRSSItab(newRSSI);
              STATE = STATE_OPEN_GATE;
            }
            else {
              printIBeacon(device); // DEBUG
              Serial.printf("  - RSSI average too low:%d\n",avRSSI); // DEBUG
              updateRSSItab(newRSSI);
            }
          break;
        }
    }

  private:
    
    // ***** compute average RSSI *****
    int averageRSSI(RSSI newRSSI){
      
      if ((newRSSI.time - tabRSSI[0].time) >3){ // if last trame was detected more than 3 sec ago, return an average RSSI of -100 // TODO compare epochtime insted
        updateRSSItab(newRSSI);
        return -100;
      }
      else{
        int averageRSSI = (newRSSI.val + tabRSSI[0].val + tabRSSI[1].val) /3;
        updateRSSItab(newRSSI);
        return averageRSSI;
      }
    }

    // ***** update RSSI table *****
    void updateRSSItab (RSSI newRSSI){
        tabRSSI[1]=tabRSSI[0];
        tabRSSI[0]= newRSSI;
    }
    
    // ***** open gate *****
    void openGate() {
      Serial.println(" OPENING GATE");
    }
  
    // ***** is frame iBeacon ? *****
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

    // ***** is UUID_NAVETTE ? *****
    bool isNavetteUUID(BLEAdvertisedDevice device) {
     if( getProxyUuid(device).equals(BLEUUID(NAVETTE_UUID))){
        return true;
      }
      else {
        return false;
     }
    }
    
    // ***** RSSI *****
    RSSI getRSSI(BLEAdvertisedDevice device){
      RSSI newRSSI;
      gettimeofday(&tv, NULL); // TODO : save epoch time insted 
      newRSSI.val = device.getRSSI();
      newRSSI.time = tv.tv_sec;
      return newRSSI;
      }
      
    // ***** companyId *****
    unsigned short getCompanyId(BLEAdvertisedDevice device) {
      const unsigned short* pCompanyId = (const unsigned short*)&device
                                         .getManufacturerData()
                                         .c_str()[0];
      return *pCompanyId;
    }

    // ***** iBeacon Header *****
    unsigned short getIBeaconHeader(BLEAdvertisedDevice device) {
      const unsigned short* pHeader = (const unsigned short*)&device
                                      .getManufacturerData()
                                      .c_str()[2];
      return *pHeader;
    }

    // ***** iBEACON UUID *****
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

    // ***** iBEACON Major *****
    uint16_t getMajor(BLEAdvertisedDevice device) {
      std::string strManufacturerData = device.getManufacturerData();
      uint8_t cManufacturerData[100];
      strManufacturerData.copy((char *)cManufacturerData, strManufacturerData.length(), 0);
      BLEBeacon oBeacon = BLEBeacon();
      oBeacon.setData(strManufacturerData);
      return ENDIAN_CHANGE_U16(oBeacon.getMajor());
    }

    // ***** iBEACON Minor *****
    uint16_t getMinor(BLEAdvertisedDevice device) {
      std::string strManufacturerData = device.getManufacturerData();
      uint8_t cManufacturerData[100];
      strManufacturerData.copy((char *)cManufacturerData, strManufacturerData.length(), 0);
      BLEBeacon oBeacon = BLEBeacon();
      oBeacon.setData(strManufacturerData);
      return ENDIAN_CHANGE_U16(oBeacon.getMinor());
    }

    // ***** iBEACON TxPower *****
    int8_t getTxPower(BLEAdvertisedDevice device) {
      std::string strManufacturerData = device.getManufacturerData();
      uint8_t cManufacturerData[100];
      strManufacturerData.copy((char *)cManufacturerData, strManufacturerData.length(), 0);
      BLEBeacon oBeacon = BLEBeacon();
      oBeacon.setData(strManufacturerData);
      return oBeacon.getSignalPower();
    }

    // ***** print iBeacon device info *****
    void printIBeacon(BLEAdvertisedDevice device) {
      gettimeofday(&tv, NULL);
      Serial.printf("time:%d name:%s uuid:%s major:%d minor:%d rssi:%d\r",
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

  //init tabRSSI
  tabRSSI[0].val = -100;
  tabRSSI[1].val = -100;
  tabRSSI[0].time = 0;
  tabRSSI[1].time = 0;

  //init state
  STATE = STATE_SCAN;
}

void loop() {
  if(STATE == STATE_OPEN_GATE){
      gettimeofday(&tv, NULL); // TODO : save epoch time insted 
      if((tv.tv_sec - tabRSSI[0].time)>4){ //TODO define time variable
        STATE = STATE_SCAN;
        Serial.printf("time:%d START SCANNING\n", tv.tv_sec);
      }
  }
      
  BLEScan* scan = BLEDevice::getScan();
  scan->setAdvertisedDeviceCallbacks(new IBeaconAdvertised(), true);
  scan->setActiveScan(true);
  scan->start(SCAN_TIME);
  }
