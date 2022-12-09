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
#define HIGH4BITS_U16(x) (((x)&0b1111000000000000) >> 12)

#define RELAY1 32

#define NAVETTE_UUID "DEADDEAD-F88F-0042-F88F-010203040506"   // same UUID for all vehicles
#define FORCEGATEOPEN 0b0100                                  // minor high bits = 0b01 => force gate to open
#define CLEARGATECALIBRATION 0b1000                           // minor high bits = 0b10 => disable BLE scan and upload software
#define OTASWUPDATE 0b1100                                    // minor high bits = 0b11 => clear gate calibration

#define OTA_EXIT_AFTER 60*5             // after X sec if the software is not updating, getting out of the mode STATE_OTA
#define SCAN_TIME 1                   // scan period in second
#define GO_TO_SCAN_STATE_DELAY 3      // if no trame was received during X sec, go to STATE_SCAN
#define DELAY_REJECT_TRAM 3           // if the last tram was received more than X seconds ago, the average RSSI is not computed and returns an average RSSI of -100
#define PULSE_DURATION 500            // pulse to open gate. duration in ms
#define DELAY_BETWEEN_PULSE 5         // to keep the gate open emit a pulse every X seconds
#define RSSI_THRESHOLD_OPEN_GATE -95  // if the average RSSI is above this threshold the gate can be open
#define SERIAL_BAUDRATE 115200

// ***** definitions *****
// Wifi credentials
const char *ssid = "Gate";
const char *password = "neOCampus";

typedef enum {
  STATE_SCAN = 0, // STATE_SCAN : scanning iBeacon tram with UUID = NAVETTE_UUID. Opening door when the RSSI is high enough. Go to STATE_OPEN_GATE after opening the door.
  STATE_OPEN_GATE, // STATE_OPEN_GATE : door is open. Keep the door open while receiving tram. Go to STATE_SCAN when not receiving tram after GO_TO_SCAN_STATE_DELAY
  STATE_OTA // STATE_OTA : disable BLE and start uploading software with Wifi
} MACHINE_STATE;

MACHINE_STATE STATE;     // state of the system : can be either STATE_SCAN or STATE_OPEN_GATE or STATE_OTA
time_t t;      // time is seconds
time_t timerOTA;      // time OTA software update started
time_t tPulseGate;  

struct RSSI {  // table contening the RSSI and time of the last 2 trams. Used to compute the average RSSI.
  int val = -100;
  time_t time = 0;
} tabRSSI[3];

bool BLEScanDeactivated = false;

// ***** set up Serial Port *****
void setupSerial() {
#ifdef SERIAL_BAUDRATE
  delay(3000);                    // time for USB serial link to come up anew
  Serial.begin(SERIAL_BAUDRATE);  // Start serial for output
  Serial.setDebugOutput(true);
#endif
}

// ***** set up WiFi *****
void setUpWifi() {
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid,password);
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
  time(&t); // get time
  Serial.printf("time:%d OPENING GATE\n",t);  //DEBUG
  digitalWrite(RELAY1, HIGH);       //activate relay1 for 0.5 sec
  delay(PULSE_DURATION);
  digitalWrite(RELAY1, LOW);
  tPulseGate = t;  // save time of the last pulse
}

  // ***** update RSSI table *****
void updateRSSItab(RSSI newRSSI) {  // save RSSI and time in the table tabRSSI
    tabRSSI[2] = tabRSSI[1];
    tabRSSI[1] = tabRSSI[0];
    tabRSSI[0] = newRSSI;
}
  
// ***** compute average RSSI *****
int averageRSSI() {
  if ((tabRSSI[0].time - tabRSSI[1].time) > DELAY_REJECT_TRAM) {  // if last trame was detected more than x sec ago, return an average RSSI of -100
    return -100;
  } else {
    int avRSSI = (tabRSSI[2].val + tabRSSI[1].val + tabRSSI[0].val) / 3;  // compute the average RSSI using the last 3 RSSI values
    return avRSSI;
  }
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
    if (HIGH4BITS_U16(getMinor(device)) == FORCEGATEOPEN) {
      time(&t); // get time
      Serial.printf("time:%d FORCE OPEN TRAM",t);    // DEBUG
      openGate();
      return;
    }

        // received a trame OTA Software Update
    if (HIGH4BITS_U16(getMinor(device)) == OTASWUPDATE) {
      time(&t); // get time
      timerOTA = t;
      Serial.printf("time:%d OTA UPDATING",timerOTA);    // DEBUG
      STATE = STATE_OTA;
      return;
    }

    RSSI newRSSI = getRSSI(device);   // get RSSI of the received tram
    updateRSSItab(newRSSI);   // save received tram RSSI in the table
    printIBeacon(device);    // DEBUG
    manageGateState(true);    // manage gate opening
    
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
    // char buf[512];
    // memset(buf,0, sizeof (buf));
    // strncpy(buf, msg, (sizeof msg)-1);
    time(&t);
    Serial.printf("time:%d name:%s uuid:%s major:%d minor:%d rssi:%d, avRSSI:%d \r\n",
                  t,
                  device.getName().c_str(),
                  getProxyUuid(device).toString().c_str(),
                  getMajor(device),
                  getMinor(device),
                  device.getRSSI(),
                  averageRSSI());
  }
};

void manageGateState(bool receivedTram)
{
  time(&t); // get time
  
  switch (STATE) { //check gate state
    case STATE_OTA :
      BLEScanDeactivated = true; //disable bluetooth scanning
      if(t - timerOTA > OTA_EXIT_AFTER){
        STATE = STATE_SCAN; // go to STATE_SCAN
        BLEScanDeactivated = false; //enable bluetooth scanning
      }
    break;//end case STATE_OTA
    case STATE_OPEN_GATE:
      BLEScanDeactivated = false;
      if ((t - tabRSSI[0].time) > GO_TO_SCAN_STATE_DELAY) {  // if last tram was received more than x seconds ago (GO_TO_SCAN_STATE_DELAY)
        STATE = STATE_SCAN; // change state : go to STATE_SCAN
        Serial.printf("time:%d GO TO STATE_SCAN\n",t);  // DEBUG
      } 
      else if ((t - tPulseGate) > DELAY_BETWEEN_PULSE) {  // send a pulse to keep the door open every X sec (DELAY_BETWEEN_PULSE)
        //Serial.printf("time:%d KEEPING GATE OPEN, last pulse sent at t:%d", t, tPulseGate);  // DEBUG
        openGate();
      }
    break; //end case STATE_OPEN_GATE

    case STATE_SCAN:
    default:
      BLEScanDeactivated = false;
      if (receivedTram != true) { // return if no tram was received
        return ;
      }
      // if a tram was received check if gate needs to be open
      int avRSSI = averageRSSI();        // compute average RSSI (with 3 last values)
      if (avRSSI > RSSI_THRESHOLD_OPEN_GATE) {  // if the received signal power is above the threshold RSSI_THRESHOLD_OPEN_GATE
        openGate();               // open the gate
        STATE = STATE_OPEN_GATE;  // change state and go to state STATE_OPEN_GATE
      } else { // RSSI is too low
        STATE = STATE_SCAN;
      }
    break; //end case STATE_SCAN
    }
}
void manageBLEscan(){

   if (BLEScanDeactivated == false){
    BLEScan *scan = BLEDevice::getScan(); // start scanning
    scan->setAdvertisedDeviceCallbacks(new IBeaconAdvertised(), true);
    scan->setActiveScan(true);
    scan->start(SCAN_TIME);
  }
}

void setup() {
  //init serial port
  setupSerial();
  
  //init relay module
  pinMode(RELAY1, OUTPUT);
  
  //init state
  BLEScanDeactivated = false;
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
  
  Serial.println("Ready");
}

void loop() {
  ArduinoOTA.handle();
  
  manageGateState(false);
  
  manageBLEscan();
}