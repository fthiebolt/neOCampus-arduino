#include <Arduino.h>
#include <ArduinoOTA.h>
#include <WiFi.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <BLEBeacon.h>
#include <time.h>

#define ENDIAN_CHANGE_U16(x) ((((x)&0xFF00) >> 8) + (((x)&0xFF) << 8))
#define HIGH2BITS_U16(x) (((x)&0b1100000000000000) >> 14)

#define RELAY1 32

#define NAVETTE_UUID "DEADDEAD-F88F-0042-F88F-010203040506"  // same UUID for all vehicles
#define FORCEGATEOPEN 0b01                                   // minor high bits = 0b01 => force gate to open
#define CLEARGATECALIBRATION 0b11                            // minor high bits = 0b11 => clear gate calibration

#define STATE_SCAN 1       // STATE_SCAN : scanning iBeacon tram with UUID = NAVETTE_UUID. Opening door when the RSSI is high enough. Go to STATE_OPEN_GATE after opening the door.
#define STATE_OPEN_GATE 2  // STATE_OPEN_GATE : door is open. Keep the door open while receiving tram. Go to STATE_SCAN when not receiving tram after GO_TO_SCAN_STATE_DELAY

#define SCAN_TIME 2                   // scan period in second
#define GO_TO_SCAN_STATE_DELAY 3      // if no trame was received during X sec, go to STATE_SCAN
#define DELAY_REJECT_TRAM 3           // if the last tram was received more than X seconds ago, the average RSSI is not computed and returns an average RSSI of -100
#define PULSE_DURATION 500            // pulse to open gate. duration in ms
#define DELAY_BETWEEN_PULSE 5         // to keep the gate open emit a pulse every X seconds
#define RSSI_THRESHOLD_OPEN_GATE -95  // if the average RSSI is above this threshold the gate can be open
#define SERIAL_BAUDRATE 115200
// ***** definitions *****
int STATE;     // state of the system : can be either STATE_SCAN or STATE_OPEN_GATE
time_t t;      // time is seconds
struct RSSI {  // table contening the RSSI and time of the last 2 trams. Used to compute the average RSSI.
  int val = -100;
  time_t time = 0;
} tabRSSI[2];
time_t tPulseGate = 0;


// ***** set up WiFi *****
void setUpWifi() {
  WiFi.mode(WIFI_AP);
  WiFi.softAP("gate01");
}

// ***** set up On The Air software upload *****
void setupOTA() {
  // Port defaults to 3232
  ArduinoOTA.setPort(3232);
  // Hostname
  ArduinoOTA.setHostname("gate01");
  // Authentication
  ArduinoOTA.setPassword("neOCampus");

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
  Serial.println(" OPENING GATE");  //DEBUG
  digitalWrite(RELAY1, HIGH);       //activate relay1 for 0.5 sec
  delay(PULSE_DURATION);
  digitalWrite(RELAY1, LOW);
  tPulseGate = t;  // save time of the last pulse
}

class IBeaconAdvertised : public BLEAdvertisedDeviceCallbacks {
public:

  // BLE
  void onResult(BLEAdvertisedDevice device) {
    // check if iBeacon tram
    if (!isIBeacon(device)) {
      return;
    }

    // check for NAVETTE_UUID
    if (!isNavetteUUID(device)) {
      return;
    }

    // received a trame FORCE GATE OPEN
    if (HIGH2BITS_U16(getMinor(device)) == FORCEGATEOPEN) {
      //printIBeacon(device);                    // DEBUG
      //Serial.printf("  - force open tram\n");  // DEBUG
      openGate();
      updateRSSItab(getRSSI(device));
      return;
    }

    //check gate state (STATE_SCAN or STATE_OPEN_GATE)
    switch (STATE) {

      case STATE_OPEN_GATE:
        // gate is open, keep the door open by sending a pulse every x sec
        //printIBeacon(device);                 // DEBUG
        //Serial.printf("  - gate is open\n");  // DEBUG
        updateRSSItab(getRSSI(device));  // save RSSI in the table

        break;

      case STATE_SCAN:
      default:
        //check if gate needs to be open
        RSSI newRSSI = getRSSI(device);           // get RSSI
        int avRSSI = averageRSSI(newRSSI);        // compute average RSSI (with 3 last values)
        if (avRSSI > RSSI_THRESHOLD_OPEN_GATE) {  // if the received signal power is above the threshold RSSI_THRESHOLD_OPEN_GATE
          //printIBeacon(device);                               // DEBUG
          //Serial.printf("  - RSSI average OK:%d\n", avRSSI);  // DEBUG
          openGate();               // open the gate
          updateRSSItab(newRSSI);   // save received tram RSSI in the table
          STATE = STATE_OPEN_GATE;  // change state and go to state STATE_OPEN_GATE
        } else {
          //printIBeacon(device);                                    // DEBUG
          //Serial.printf("  - RSSI average too low:%d\n", avRSSI);  // DEBUG
          updateRSSItab(newRSSI);
          STATE = STATE_SCAN;
        }
        break;
    }
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
  bool isNavetteUUID(BLEAdvertisedDevice device) {  // check if iBeacon tram UUID = NAVETTE_UUID
    if (getProxyUuid(device).equals(BLEUUID(NAVETTE_UUID))) {
      return true;
    } else {
      return false;
    }
  }

  // ***** RSSI *****
  RSSI getRSSI(BLEAdvertisedDevice device) {  // get signal RSSI and reception time
    RSSI newRSSI;
    time(&t);
    newRSSI.val = device.getRSSI();
    newRSSI.time = t;
    return newRSSI;
  }

  // ***** update RSSI table *****
  void updateRSSItab(RSSI newRSSI) {  // save RSSI and time in the table tabRSSI
    tabRSSI[1] = tabRSSI[0];
    tabRSSI[0] = newRSSI;
  }

  // ***** compute average RSSI *****
  int averageRSSI(RSSI newRSSI) {
    if ((newRSSI.time - tabRSSI[0].time) > DELAY_REJECT_TRAM) {  // if last trame was detected more than x sec ago, return an average RSSI of -100
      updateRSSItab(newRSSI);
      return -100;
    } else {
      int averageRSSI = (newRSSI.val + tabRSSI[0].val + tabRSSI[1].val) / 3;  // compute the average RSSI using the last 3 RSSI values
      updateRSSItab(newRSSI);
      return averageRSSI;
    }
  }

  // ***** companyId *****
  unsigned short getCompanyId(BLEAdvertisedDevice device) {
    const unsigned short *pCompanyId = (const unsigned short *)&device
                                         .getManufacturerData()
                                         .c_str()[0];
    return *pCompanyId;
  }

  // ***** iBeacon Header *****
  unsigned short getIBeaconHeader(BLEAdvertisedDevice device) {
    const unsigned short *pHeader = (const unsigned short *)&device
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
    time(&t);
    Serial.printf("time:%d name:%s uuid:%s major:%d minor:%d rssi:%d\r",
                  t,
                  device.getName().c_str(),
                  getProxyUuid(device).toString().c_str(),
                  getMajor(device),
                  getMinor(device),
                  device.getRSSI());
  }
};

void setupSerial() {
#ifdef SERIAL_BAUDRATE
  delay(3000);                    // time for USB serial link to come up anew
  Serial.begin(SERIAL_BAUDRATE);  // Start serial for output
  Serial.setDebugOutput(true);
#endif
}

void setup() {
  //init serial port
  setupSerial();
  //init relay module
  pinMode(RELAY1, OUTPUT);
  //init state
  STATE = STATE_SCAN;
  //setup WiFi
  setUpWifi();
  yield();
  //setup OTA
  setupOTA();
  yield();
  ArduinoOTA.begin();
  yield();
  //init BLE
  BLEDevice::init("");
  yield();
  Serial.println("Ready");
}

void loop() {
  ArduinoOTA.handle();
  yield();
  
  if (STATE == STATE_OPEN_GATE) {  // if in STATE_OPEN_GATE (gate is already open)
    time(&t);                      // get time

    if ((t - tabRSSI[0].time) > GO_TO_SCAN_STATE_DELAY) {                                           // if last tram was received more than x seconds ago (GO_TO_SCAN_STATE_DELAY)
      STATE = STATE_SCAN;                                                                           // change state : go to STATE_SCAN
      Serial.printf("time:%d GO TO STATE_SCAN, last tram received at t:%d\n", t, tabRSSI[0].time);  // DEBUG
    } else {
      if ((t - tPulseGate) > DELAY_BETWEEN_PULSE) {  // send a pulse to keep the door open every X sec (DELAY_BETWEEN_PULSE)
        //Serial.printf("time:%d KEEPING GATE OPEN, last pulse sent at t:%d", t, tPulseGate);  // DEBUG
        openGate();
      }
    }
  }
  BLEScan *scan = BLEDevice::getScan(); // start scanning
  scan->setAdvertisedDeviceCallbacks(new IBeaconAdvertised(), true);
  scan->setActiveScan(true);
  scan->start(SCAN_TIME);
}
