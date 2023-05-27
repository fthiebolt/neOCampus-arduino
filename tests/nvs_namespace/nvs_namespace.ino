/*
 * NVS namespace tests
 * Note: no esp8266 NVS support ... instead grab WiFi ssid/psk from 
 * 
 * ---
 * TODO:
 * - add support for JSON structure to embedds several WiFi credentials
 *    e.g NVS-WiFi namespace --> key("credentials") = [ ["ssid1","pass1"], ["ssid2","pass2"], ... ]
 * - add encryption with MAC addr or APpassword for example
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
  char _answer, _tmp[256];
  String _str;

  delay(5000);
  Serial.begin(115200);   // debug link
  Serial.println(F("\n\n\n[NVS] namespace demo ..."));Serial.flush();
  delay(1000);

#ifdef ESP32
  /* NVS @ ESP32 */
  Preferences nvs_wifi;

  /*
   * Open NVS-WiFi namespace to retrieve SSID / PSK
   */
  if( not nvs_wifi.begin(WIFI_NVS_NAMESPACE,false) ) {  // R/Wf mode
    Serial.print(F("\n[NVS-WiFi] unable to open NVS namespace '"));Serial.print(WIFI_NVS_NAMESPACE);Serial.print(F("' ... reboot !"));Serial.flush();
    delay(2000);
    ESP.restart();
    delay(5000);
  }
  
  Serial.print(F("\n[NVS-WiFi] opened NVS WiFi credentials namespace ..."));Serial.flush();
  // retrieve SSID/psk
  mySSID[0]='\0'; myPSK[0]='\0';
  if( nvs_wifi.isKey(WIFI_NVS_SSID_KEY) ) {
    nvs_wifi.getBytes(WIFI_NVS_SSID_KEY, mySSID, sizeof(mySSID));
  }
  if( nvs_wifi.isKey(WIFI_NVS_PASS_KEY) ) {
    nvs_wifi.getBytes(WIFI_NVS_PASS_KEY, myPSK, sizeof(myPSK));
  }
  // close NVS namespace
  nvs_wifi.end();
  if( strlen(mySSID) and strlen(myPSK) ) {
    // using retrieved credentials to connect
    Serial.print(F("\n[NVS-WiFi] retrieved WiFi credentials from NVS :)"));Serial.flush();
    snprintf(_tmp,sizeof(_tmp), "\n\tSSID : %s\n\tPASS : %s", mySSID, myPSK);
    Serial.print(_tmp);Serial.flush();
    delay(1000);
    // do we need to change them ?
    Serial.setTimeout(10000);
    Serial.print(F("\n"));
    Serial.print(F("\nDo we need to change them (Y/n) ? "));Serial.flush();
    _answer='\0';
    if( not Serial.readBytes(&_answer,1) or (_answer!='Y' and _answer!='y') ) {
      Serial.print(F("\n\t... ok let's keep current WiFi credentials ... "));
      delay(1000);
      return;
    }
    Serial.begin(115200);   // flush input buffer
  }

  /*
   * Entering SSID / pass to save to NVS 
   */
  Serial.print(F("\n[NVS-WiFi] now setting WiFi credentials (15s timeout) ... "));Serial.flush();
  Serial.print(F("\n\tSSID --> "));Serial.flush();
  Serial.setTimeout(15000);
  _str = Serial.readStringUntil('\n');  //read until timeout
  _str.trim();                        // remove any \r \n whitespace at the end of the String
  if( not _str.length() ) {
    Serial.print(F("\n\tno input detected ... reboot !"));Serial.flush();
    delay(1000);
    ESP.restart();
    delay(5000);
  }
  strncpy(mySSID,_str.c_str(),sizeof(mySSID)); mySSID[sizeof(mySSID)-1]='\0';
  Serial.print(mySSID);Serial.flush();
  
  Serial.print(F("\n\tPASS --> "));Serial.flush();
  Serial.setTimeout(15000);
  _str = Serial.readStringUntil('\n');  //read until timeout
  _str.trim();                        // remove any \r \n whitespace at the end of the String
  if( not _str.length() ) {
    Serial.print(F("\n\tno input detected ... reboot !"));Serial.flush();
    delay(1000);
    ESP.restart();
    delay(5000);
  }
  strncpy(myPSK,_str.c_str(),sizeof(myPSK)); myPSK[sizeof(myPSK)-1]='\0';
  Serial.print(myPSK);Serial.flush();

  // save it to NVS ?
  Serial.print(F("\n"));
  Serial.print(F("\n[NVS-WiFi] entered credentials :)"));Serial.flush();
  snprintf(_tmp,sizeof(_tmp), "\n\tSSID : %s\n\tPASS : %s", mySSID, myPSK);
  Serial.print(_tmp);Serial.flush();
  delay(1000);
  Serial.setTimeout(10000);
  Serial.print(F("\nSave it to NVS (Y/n) ? "));Serial.flush();
  _answer='\0';
  if( not Serial.readBytes(&_answer,1) or (_answer!='Y' and _answer!='y') ) {
    Serial.print(F("\n\t... NOT saving WiFi credentials ... reboot ... "));
    delay(1000);
    ESP.restart();
    delay(5000);
  }
  Serial.begin(115200);   // flush input buffer

  // saving to namespace :)
  if( not nvs_wifi.begin(WIFI_NVS_NAMESPACE,false) ) {  // R/W mode
    Serial.print(F("\n[NVS-WiFi] unable to open NVS namespace '"));Serial.print(WIFI_NVS_NAMESPACE);Serial.print(F("' ... reboot !"));Serial.flush();
    delay(2000);
    ESP.restart();
    delay(5000);
  }
  
  Serial.print(F("\n[NVS-WiFi] save WiFi credentials to NVS namespace '"));Serial.print(WIFI_NVS_NAMESPACE);Serial.print(F("' ... "));Serial.flush();
  if( nvs_wifi.putBytes(WIFI_NVS_SSID_KEY,mySSID, strlen(mySSID)+1) != strlen(mySSID)+1 ) {
    Serial.print(F("\n[NVS-WiFi] ERROR while saving SSID to NVS ?!?!"));Serial.flush();
    delay(1000); ESP.restart(); delay(5000);
  }
  if( nvs_wifi.putBytes(WIFI_NVS_PASS_KEY,myPSK, strlen(myPSK)+1) != strlen(myPSK)+1 ) {
    Serial.print(F("\n[NVS-WiFi] ERROR while saving PSK to NVS ?!?!"));Serial.flush();
    delay(1000); ESP.restart(); delay(5000);
  }
  // close NVS namespace
  nvs_wifi.end();
  
  // successfully saved WiFi credentials to NVS :)
  Serial.print(F("\n\n[NVS-WiFi] successfully saved WiFi credentials to NVS :)"));  
  return;

  
#elif defined (ESP8266)
  /* ESP8266 retrieve WiFi credentials from previous connexion */

  #error "NOT YET IMPLEMENTED
  
#endif
  
  // start WiFi connexion either with DEFAULTS WiFi credentials or from retrieved ones
  //to be continued

}


// --- LOOP --------------------------------------------------------------------
void loop() {
  static unsigned long _lastMsg = 0;    // elapsed ms since last check

  // do we want to reboot ?
  if( ((millis() - _lastMsg) >= (unsigned long)45000UL) == true ) {
    _lastMsg = millis();

    Serial.begin(115200);   // flush input buffer
    Serial.print(F("\n\tDo you want to REBOOT (Y/n) ?"));
    Serial.setTimeout(5000);
    char _answer='\0';
    Serial.readBytes(&_answer,1);
    if( _answer=='Y' or _answer=='y' ) {
      Serial.print(F("\n\t... OK, reboot is on way ... "));Serial.flush();
      _need2reboot = true;
  }
  Serial.begin(115200);   // flush input buffer
    
  }

  /*
   * do something here ...
   */


  // fin de boucle
  endLoop();

  // do others stuff
  delay(250);
}
