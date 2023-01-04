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
#include <time.h>
#include "sdkconfig.h"

/*******************************/
/********* DEFINITIONS *********/
/*******************************/

#define ENDIAN_CHANGE_U16(x) ((((x)&0xFF00) >> 8) + (((x)&0xFF) << 8))
#define HIGH4BITS_U16(x) (((x)&0b1111000000000000) >> 12)

#define RELAY1 32

#define NAVETTE_UUID "DEADDEAD-F88F-0042-F88F-010203040506"   // same UUID for all vehicles
#define FORCEGATEOPEN 0b0100                                  // minor high bits = 0b01 => force gate to open
#define CLEARGATECALIBRATION 0b1000                           // minor high bits = 0b10 => disable BLE scan and upload software
#define OTASWUPDATE 0b1100                                    // minor high bits = 0b11 => clear gate calibration

#define OTA_EXIT_AFTER 60*5             // after X sec if the software is not updating, getting out of the mode STATE_OTA
#define SCAN_TIME 1                   // scan period in second
#define GO_TO_SCAN_STATE_DELAY 4      // if no frame was received during X sec, go to STATE_SCAN
#define DELAY_REJECT_FRAME 3           // if the last frame was received more than X seconds ago, the average RSSI is not computed and returns an average RSSI of -100
#define PULSE_DURATION 500            // pulse to open gate. duration in ms
#define DELAY_BETWEEN_PULSE 5         // to keep the gate open emit a pulse every X seconds
#define RSSI_THRESHOLD_OPEN_GATE -95  // if the average RSSI is above this threshold the gate can be open
#define SERIAL_BAUDRATE 115200

// Wifi credentials
const char *ssid = "Gate";
const char *password = "neOCampus";

//State enumeration
typedef enum {
  STATE_SCAN = 0, // STATE_SCAN : scanning iBeacon frame with UUID = NAVETTE_UUID. Opening door when the RSSI is high enough. Go to STATE_OPEN_GATE after opening the door.
  STATE_OPEN_GATE, // STATE_OPEN_GATE : door is open. Keep the door open while receiving frame. Go to STATE_SCAN when not receiving frame after GO_TO_SCAN_STATE_DELAY
  STATE_OTA // STATE_OTA : disable BLE and start uploading software with Wifi
} MACHINE_STATE;

MACHINE_STATE STATE;     // state of the system : can be either STATE_SCAN or STATE_OPEN_GATE or STATE_OTA

//Time related definition
time_t t;           // time is seconds
time_t timerOTA;    // time OTA software update started
time_t tPulseGate = 0;  // time last Pulse to open gate

struct InfoBeacon {  // structure contening received beacon RSSI and time
  int rssi = -100;
  time_t time = 0;
} tabRecBeacon[3]; // info of the last 3 received frames. Used to compute the average RSSI.

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

// ***** set up WiFi AP*****
void setUpWifiAP() {
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
}

// ***** set up On The Air software upload *****
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
    else  // U_SPIFFS
      type = "filesystem";

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
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

// ***** open gate *****
void openGate() {
  Serial.printf("OPENING GATE\n"); //DEBUG
  digitalWrite(RELAY1, HIGH);       //activate relay1 for 0.5 sec
  delay(PULSE_DURATION);
  digitalWrite(RELAY1, LOW);
  time(&tPulseGate);  // save time of the last pulse
}

// ***** update Beacon RSSI table *****
void updateBeaconTab(InfoBeacon newBeacon) {  // save RSSI and time in the table tabRecBeacon
  tabRecBeacon[2] = tabRecBeacon[1];
  tabRecBeacon[1] = tabRecBeacon[0];
  tabRecBeacon[0] = newBeacon;
}

// ***** compute average RSSI *****
int averageRSSI() {
  time(&t); // get time
  if (difftime(t,tabRecBeacon[2].time) > DELAY_REJECT_FRAME) {  // if last frames were received more than x sec ago, return an average RSSI of -100
    return -100;
  } else {
    int avRSSI = (tabRecBeacon[2].rssi + tabRecBeacon[1].rssi + tabRecBeacon[0].rssi) / 3;  // compute the average RSSI using the last 3 RSSI values
    return avRSSI;
  }
}

class IBeaconAdvertised : public BLEAdvertisedDeviceCallbacks {
  public:

    // BLE
    void onResult(BLEAdvertisedDevice device) {
      // check if iBeacon frame
      if (!isIBeacon(device)) {
        return;
      }

      // check for NAVETTE_UUID
      if (!isNavetteUUID(device)) {
        return;
      }

      // received a frame FORCE GATE OPEN
      if (HIGH4BITS_U16(getMinor(device)) == FORCEGATEOPEN) {
        Serial.printf("FORCE OPEN FRAME ");   // DEBUG
        openGate();
        return;
      }

      // received a FRAME OTA Software Update
      if (HIGH4BITS_U16(getMinor(device)) == OTASWUPDATE) {
        time(&timerOTA); // save OTA start time 
        Serial.printf("OTA SOFTWARE UPDATING MODE \n");   // DEBUG
        STATE = STATE_OTA;
        return;
      }

      InfoBeacon newBeacon = saveBeaconInfo(device);   // get received Beacon info
      updateBeaconTab(newBeacon);   // save received frame RSSI in the table
      printIBeacon(device);    // DEBUG
    }

  private:

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
    bool isNavetteUUID(BLEAdvertisedDevice device) {  // check if iBeacon frame UUID = NAVETTE_UUID
      if (getProxyUuid(device).equals(BLEUUID(NAVETTE_UUID))) {
        return true;
      } else {
        return false;
      }
    }

    // ***** save RSSI info *****
    InfoBeacon saveBeaconInfo(BLEAdvertisedDevice device) {  // get signal RSSI and reception time
      InfoBeacon newBeacon;
      time(&t);
      newBeacon.rssi = device.getRSSI();
      newBeacon.time = t;
      return newBeacon;
    }

    // ***** get companyId *****
    unsigned short getCompanyId(BLEAdvertisedDevice device) {
      const unsigned short *pCompanyId = (const unsigned short *)&device
                                         .getManufacturerData()
                                         .c_str()[0];
      return *pCompanyId;
    }

    // ***** get iBeacon Header *****
    unsigned short getIBeaconHeader(BLEAdvertisedDevice device) {
      const unsigned short *pHeader = (const unsigned short *)&device
                                      .getManufacturerData()
                                      .c_str()[2];
      return *pHeader;
    }

    // ***** get iBEACON UUID *****
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
      // char buf[512];
      // memset(buf,0, sizeof (buf));
      // strncpy(buf, msg, (sizeof msg)-1);
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

// ***** manage gate Opening  *****
void manageGateOpening() {
  switch (STATE) { //check gate state

    case STATE_OPEN_GATE:
      if (difftime(t,tPulseGate) > DELAY_BETWEEN_PULSE) {  // send a pulse to keep the gate open every X=DELAY_BETWEEN_PULSE sec
        openGate();
      }
      break;

    case STATE_OTA :
    case STATE_SCAN:
    default:
      // do not open gate
      break; //end case STATE_SCAN
  }
}

// ***** manage gate state  *****
void manageState()
{
  time(&t); // get time

  switch (STATE) { //check gate state

    case STATE_OTA :
      if (difftime(t,timerOTA) > OTA_EXIT_AFTER) {
        STATE = STATE_SCAN; // go to STATE_SCAN
        Serial.printf("GO TO STATE_SCAN\n"); // DEBUG
      }
      break;//end case STATE_OTA

    case STATE_OPEN_GATE:
      if (difftime(t,tabRecBeacon[0].time) > GO_TO_SCAN_STATE_DELAY) {  // if last frame was received more than x seconds ago (GO_TO_SCAN_STATE_DELAY)
        STATE = STATE_SCAN; // go to STATE_SCAN
        Serial.printf("GO TO STATE_SCAN\n"); // DEBUG
      }
      break; //end case STATE_OPEN_GATE

    case STATE_SCAN:
      int avRSSI = averageRSSI(); // compute average RSSI (with 3 last values)
      if (avRSSI > RSSI_THRESHOLD_OPEN_GATE) {  // if the received signal power is above the threshold RSSI_THRESHOLD_OPEN_GATE
        STATE = STATE_OPEN_GATE;  // change state and go to state STATE_OPEN_GATE
        Serial.printf("GO TO STATE_OPEN_GATE\n"); // DEBUG
      } else {
        // RSSI is too low
      }
      break; //end case STATE_SCAN
  }
}

// ***** start BLE scan  *****
void startScanBLE() {
  BLEScan* pBLEScan = BLEDevice::getScan(); // start scanning
  pBLEScan->setAdvertisedDeviceCallbacks(new IBeaconAdvertised(), true);
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
  switch (STATE) { //check gate state

    case STATE_OTA :
      // do not restart BLE scan
      break;//end case STATE_OTA

    case STATE_OPEN_GATE:
    case STATE_SCAN:
    default:
      if (!BLEScanActivated)
        startScanBLE(); // start scanning
      break; //end case STATE_SCAN
  }
}

void setup() {
  //init serial port
  setupSerial();

  //init relay module
  pinMode(RELAY1, OUTPUT);

  //setup WiFi as Access Point
  setUpWifiAP();

  //setup OTA
  setupOTA();
  ArduinoOTA.begin();

  //init BLE
  BLEDevice::init("");

  //init state
  STATE = STATE_SCAN;

  Serial.println();
  Serial.println("Ready");
  Serial.println();
}

void loop() {
  ArduinoOTA.handle();
  manageState();
  manageGateOpening();
  manageBLEscan();
  delay(100);
}
