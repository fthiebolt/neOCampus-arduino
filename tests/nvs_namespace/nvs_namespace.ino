/*
 * NVS namespace tests
 * Note: no esp8266 NVS support ... instead grab WiFi ssid/psk from 
 * 
 * ---
 * TODO:
 * - add support for JSON structure to embedds several WiFi credentials
 * ---
 * F.Thiebolt may.23  initial release
 */


/*
 * Includes
 */
#include <Arduino.h>

#ifdef ESP8266
  #warning "[esp8266] No NVS support, instead trying to read SSID/psk from last WiFi connection
#elif defined (ESP32)
  #include "Preferences.h"
#endif


/*
 * Definitions
*/

// Debug related definitions
#define SERIAL_BAUDRATE   115200

// NVS namespace
#define WIFI_NVS_NAMESPACE      "wifiCredentials"  // 15 chars max.
#define WIFI_NVS_SSID_KEY       "ssid"
#define WIFI_NVS_PASS_KEY       "pass"


/*
 * Global variables
 */
bool _need2reboot = false;              // flag to tell a reboot is requested

char mySSID[64] = "";
char myPSK[64]  = "";


/*
 * Functions
*/

void endLoop( void ) {
  static unsigned long _lastCheck = 0;    // elapsed ms since last check

  // check if a reboot has been requested ...
  if( _need2reboot ) {
    Serial.print(F("\n[REBOOT] a reboot has been asked ..."));Serial.flush();
    delay(500);
    ESP.restart();
    delay(5000); // to avoid an infinite loop
  }

  // a second elapsed ?
  if( ((millis() - _lastCheck) >= (unsigned long)1000UL) == true ) {
    _lastCheck = millis();

    // serial link activity marker ...
    Serial.print(F("."));
  }
}


// --- MAIN --------------------------------------------------------------------
void setup() {
  delay(5000);
  Serial.begin(115200);   // debug link
  Serial.println(F("\n\n\n[NVS] namespace demo ..."));Serial.flush();
  delay(1000);

#ifdef ESP32
  /* NVS @ ESP32 */
  Preferences nvs_wifi;
  if( nvs_wifi.begin(WIFI_NVS_NAMESPACE,true) ) {  // readonly mode
    Serial.print(F("\n[wifiParams] opened NVS WiFi credentials namespace ..."));Serial.flush();
    // retrieve SSID/psk
    nvs_wifi.getBytes(WIFI_NVS_SSID_KEY, mySSID, sizeof(mySSID)); 
    nvs_wifi.getBytes(WIFI_NVS_PASS_KEY, myPSK, sizeof(myPSK)); 
    if( strlen(mySSID) and strlen(myPSK) ) {
      // using retrived credentials to connect
      Serial.print(F("\n[NVS-WiFi] retreieved WiFi credentials from NVS :)"));Serial.flush();
    }
    else {
      Serial.print(F("\n[NVS-WiFi] SSID and/or PSK missing ... cancel :("));Serial.flush();
      mySSID[0] = '\0';
      myPSK[0] = '\0';
    }
#elif defined (ESP8266)
  /* ESP8266 retrieve WiFi credentials from previous connexion */

  #error "NOT YET IMPLEMENTED
  
#endif
  }
  
  // start WiFi connexion either with DEFAULTS WiFi credentials or from retrieved ones
  //to be continued

}


// --- LOOP --------------------------------------------------------------------
void loop() {


  /*
   * do something here ...
   */


  // fin de boucle
  endLoop();

  // do others stuff
  delay(250);
}
