/*******************************/
/*********** INCLUDES***********/
/*******************************/

#include <Arduino.h>
#include <ArduinoOTA.h>
#include <WiFi.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <BLEBeacon.h>
#include "sdkconfig.h"
#include "esp_task_wdt.h"

#include "gateParameters.h"
#include "credentials.h"

/*******************************/
/********* DEFINITIONS *********/
/*******************************/

#define ENDIAN_CHANGE_U16(x) ((((x)&0xFF00) >> 8) + (((x)&0xFF) << 8))
#define HIGH4BITS_U16(x) (((x)&0b1111000000000000) >> 12)
#define RELAY1 32

// State enumeration
typedef enum {
  STATE_SCAN = 0, /*STATE_SCAN : scanning iBeacon frame with UUID = NAVETTE_UUID. Opening door when the RSSI is high enough. Go to STATE_OPEN_GATE after opening the door.*/
  STATE_OPEN_GATE, /*TATE_OPEN_GATE : door is open. Keep the door open while receiving frame. Go to STATE_SCAN when not receiving frame after GO_TO_SCAN_STATE_DELAY*/
  STATE_OTA /*STATE_OTA : disable BLE and start uploading software with Wifi*/
} MACHINE_STATE;

MACHINE_STATE STATE;     /*state of the system : can be either STATE_SCAN or STATE_OPEN_GATE or STATE_OTA*/

// Time related definition
time_t t;           /*time is seconds*/
time_t timerOTA;    /*time OTA software update started*/
time_t tPulseGate = 0;  /*time last Pulse to open gate*/

struct InfoBeacon {  /*structure contening received beacon RSSI and time*/
  int rssi = -100;
  time_t time = 0;
} tabRecBeacon[3]; /*info of the last 3 received frames. Used to compute the average RSSI.*/

bool BLEScanActivated = false;

/*****************************/
/********* FONCTIONS *********/
/*****************************/

// set up Serial Port
void setupSerial() {
#ifdef SERIAL_BAUDRATE
  delay(3000);                    /*time for USB serial link to come up anew*/
  Serial.begin(SERIAL_BAUDRATE);  /*start serial for output*/
  Serial.setDebugOutput(true);
#endif
}

// compute WiFi AP name
const char *getAPname( void ) {
  uint8_t macAddr[6];
  static char _apName[16] = "";
  if( !strlen(_apName) ) {
    WiFi.macAddress(macAddr); /*get MAC address*/
    snprintf(_apName,sizeof(_apName),"GATE_%02X%02X",macAddr[4],macAddr[5]);
  }
  return (const char*)_apName;
}

// set up WiFi AP
void setUpWifiAP() {
  WiFi.mode(WIFI_AP);
  WiFi.softAP(getAPname(), password);
}

// set up Wdtifor BLE scan_evt timeout error
void setupWdtBLE(){
  esp_task_wdt_init(10, true); /*enable panic so ESP32 restarts, interrupt when task executed for more than 10 seconds*/
  esp_task_wdt_add(NULL); /*add current thread to WDT watch*/
}

// set up On The Air software upload
void setupOTA() {
  // Port defaults to 3232
  ArduinoOTA.setPort(3232);
  // Hostname
  ArduinoOTA.setHostname(ssid);
  // Authentication
  ArduinoOTA.setPassword(password);

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else  /*U_SPIFFS*/
      type = "filesystem";

    /*NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()*/
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
    STATE = STATE_SCAN;
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
}

// open gate
void openGate() {
  Serial.printf("OPENING GATE\n"); /*DEBUG*/
  digitalWrite(RELAY1, HIGH); /*activate relay1 for 0.5 sec*/
  delay(PULSE_DURATION);
  digitalWrite(RELAY1, LOW);
  time(&tPulseGate); /*save time of the last pulse*/
}

// update Beacon RSSI table
void updateBeaconTab(InfoBeacon newBeacon) { /*save RSSI and time in the table tabRecBeacon*/
  tabRecBeacon[2] = tabRecBeacon[1];
  tabRecBeacon[1] = tabRecBeacon[0];
  tabRecBeacon[0] = newBeacon;
}

// compute average RSSI
int averageRSSI() {
  time(&t); // get time
  if (difftime(t,tabRecBeacon[2].time) > DELAY_REJECT_FRAME) {  /*if last frames were received more than x sec ago, return an average RSSI of -100*/
    return -100;
  } else {
    int avRSSI = (tabRecBeacon[2].rssi + tabRecBeacon[1].rssi + tabRecBeacon[0].rssi) / 3;  /*compute the average RSSI using the last 3 RSSI values*/
    return avRSSI;
  }
}

class IBeaconAdvertised : public BLEAdvertisedDeviceCallbacks {
  public:

    // BLE on result
    void onResult(BLEAdvertisedDevice device) {
      /*check if iBeacon frame*/
      if (!isIBeacon(device)) {
        return;
      }

      /*check for NAVETTE_UUID*/
      if (!isNavetteUUID(device)) {
        return;
      }

      /*received a frame FORCE GATE OPEN*/
      if (HIGH4BITS_U16(getMinor(device)) == FORCEGATEOPEN) {
        Serial.printf("FORCE OPEN FRAME "); /*DEBUG*/
        openGate();
        return;
      }

      /*received a FRAME OTA Software Update*/
      if (HIGH4BITS_U16(getMinor(device)) == OTASWUPDATE) {
        time(&timerOTA); /*save OTA start time */
        Serial.printf("OTA SOFTWARE UPDATING MODE \n"); /*DEBUG*/
        STATE = STATE_OTA;
        return;
      }

      InfoBeacon newBeacon = saveBeaconInfo(device); /*get received Beacon info*/
      updateBeaconTab(newBeacon); /*save received frame RSSI in the table*/
      printIBeacon(device); /*DEBUG*/
    }

  private:

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

    // is UUID_NAVETTE ?
    bool isNavetteUUID(BLEAdvertisedDevice device) { /*check if iBeacon frame UUID = NAVETTE_UUID*/
      if (getProxyUuid(device).equals(BLEUUID(NAVETTE_UUID))) {
        return true;
      } else {
        return false;
      }
    }

    // save RSSI info
    InfoBeacon saveBeaconInfo(BLEAdvertisedDevice device) { /*get signal RSSI and reception time*/
      InfoBeacon newBeacon;
      time(&t);
      newBeacon.rssi = device.getRSSI();
      newBeacon.time = t;
      return newBeacon;
    }

    // get companyId
    unsigned short getCompanyId(BLEAdvertisedDevice device) {
      const unsigned short *pCompanyId = (const unsigned short *)&device
                                         .getManufacturerData()
                                         .c_str()[0];
      return *pCompanyId;
    }

    // get iBeacon Header
    unsigned short getIBeaconHeader(BLEAdvertisedDevice device) {
      const unsigned short *pHeader = (const unsigned short *)&device
                                      .getManufacturerData()
                                      .c_str()[2];
      return *pHeader;
    }

    // get iBEACON UUID
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

    // BEACON Major 
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
      time(&t);
      Serial.printf("name:%s uuid:%s major:%d minor:%d rssi:%d, avRSSI:%d \r\n",
                    device.getName().c_str(),
                    getProxyUuid(device).toString().c_str(),
                    getMajor(device),
                    getMinor(device),
                    device.getRSSI(),
                    averageRSSI());
    }
};

// manage gate opening 
void manageGateOpening() {
  switch (STATE) { /*check gate state*/

    case STATE_OPEN_GATE:
      if (difftime(t,tPulseGate) > DELAY_BETWEEN_PULSE) { /*end a pulse to keep the gate open every X=DELAY_BETWEEN_PULSE sec*/
        openGate();
      }
      break;

    case STATE_OTA :
    case STATE_SCAN:
    default:
      /*do not open gate*/
      break; /*end case STATE_SCAN*/
  }
}

// manage gate state
void manageState()
{
  time(&t); /*get time*/

  switch (STATE) { /*check gate state*/

    case STATE_OTA :
      if (difftime(t,timerOTA) > OTA_EXIT_AFTER) {
        STATE = STATE_SCAN; /*go to STATE_SCAN*/
        setupWdtBLE(); /*setup Wdt for BLE*/
        Serial.printf("GO TO STATE_SCAN\n"); /*DEBUG*/
      }
      break;/*end case STATE_OTA*/

    case STATE_OPEN_GATE:
      if (difftime(t,tabRecBeacon[0].time) > GO_TO_SCAN_STATE_DELAY) {  /*f last frame was received more than x seconds ago (GO_TO_SCAN_STATE_DELAY)*/
        STATE = STATE_SCAN; /*go to STATE_SCAN*/
        Serial.printf("GO TO STATE_SCAN\n"); /*DEBUG*/
      }
      break; /*end case STATE_OPEN_GATE*/

    case STATE_SCAN:
      int avRSSI = averageRSSI(); /*compute average RSSI (with 3 last values)*/
      if (avRSSI > RSSI_THRESHOLD_OPEN_GATE) {  /*if the received signal power is above the threshold RSSI_THRESHOLD_OPEN_GATE*/
        STATE = STATE_OPEN_GATE;  /*change state and go to state STATE_OPEN_GATE*/
        Serial.printf("GO TO STATE_OPEN_GATE\n"); /*DEBUG*/
      } else {
        /*RSSI is too low*/
      }
      break; /*end case STATE_SCAN*/
  }
}

// start BLE scan
void startScanBLE() {
  BLEScan* pBLEScan = BLEDevice::getScan(); // start scanning
  pBLEScan->setAdvertisedDeviceCallbacks(new IBeaconAdvertised(), true);
  pBLEScan->setActiveScan(true);
  pBLEScan->start(SCAN_TIME, scanCompleteCB);
  BLEScanActivated = true;
}

// scan complete Call Back 
/* Callback invoked when scanning has completed */
static void scanCompleteCB(BLEScanResults scanResults) { 
  BLEScanActivated = false;
}

// manage BLE scan
void manageBLEscan() {
  switch (STATE) { /*check gate state*/

    case STATE_OTA :
      /*do not restart BLE scan*/
      esp_task_wdt_delete(NULL); /*disable watchdog timer for BLE*/
      break;

    case STATE_OPEN_GATE:
    case STATE_SCAN:
    default:
      if (!BLEScanActivated)
        startScanBLE(); /*start scanning*/
        esp_task_wdt_reset(); /*reset the watchdog timer*/
      break;
  }
}

// setup
void setup() {
  /*init serial port*/
  setupSerial();

  /*init relay module*/
  pinMode(RELAY1, OUTPUT);
  
  /*setup WiFi as Access Point*/
  setUpWifiAP();

  /*setup OTA*/
  setupOTA();
  ArduinoOTA.begin();
  
  /*init BLE*/
  BLEDevice::init("");

  /*init state*/
  STATE = STATE_SCAN;

  /*setup Wdt for BLE*/
  setupWdtBLE(); 
  
  /*print ready*/
  Serial.println();
  Serial.println("Ready");
  Serial.println();
}

// loop
void loop() {
  ArduinoOTA.handle();
  manageState();
  manageGateOpening();
  manageBLEscan();
  delay(100);
}
