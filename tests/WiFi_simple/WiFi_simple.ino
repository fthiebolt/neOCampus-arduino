/*
 * Simple WiFi connect test intended to ESP32
 * i.e NO WifiManager involved
 * 
 * NOTE: WIFI PROTOCOL IS PERSISTENT ACROSS REBOOT and FLASH !!
 */

#if !defined(ESP32)
#error "Wrong architecture ... ought to be ESP32"
#endif

#include <Arduino.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <esp_log.h>

// Defines
#define SET_WIFI_PROTOCOL   1

/* Global variables */
#error "Change SSID/passwd !!"
const char* ssid = "neOCampus";
const char* password = "XXXXXXX";
esp_err_t _err;


/* Functions */
void display_wifi_protocol( wifi_interface_t ifx=WIFI_IF_STA ) {

  uint8_t cur_protocols;
  _err = esp_wifi_get_protocol(ifx, &cur_protocols);
  if( _err != ESP_OK ) {
    Serial.println("Could not get protocol!");
    //log_e("Could not get protocol! %d", _err);
    delay(1000);
  }
  Serial.print(F("\tWIFI protocols = 0x"));Serial.println(cur_protocols,HEX);Serial.flush();
  if( cur_protocols & WIFI_PROTOCOL_11N ) {
    Serial.println(F("\t\tWiFi_Protocol_11n"));
  }
  if( cur_protocols & WIFI_PROTOCOL_11G ) {
    Serial.println(F("\t\tWiFi_Protocol_11g"));
  }
  if( cur_protocols & WIFI_PROTOCOL_11B ) {
    Serial.println(F("\t\tWiFi_Protocol_11b"));
  }
  return;
}


/* 
 * Setup
 */
void setup(){
  Serial.begin(115200);
  delay(3000);

  Serial.print(F("\n###\nConnecting to SSID "));Serial.println(ssid);Serial.flush();

  /* WARNING: WiFi mode resets the protocol definition ...
   *  hence est_wifi_set_protocol is set AFTER */
  WiFi.mode(WIFI_STA); // Optional
  Serial.println(F("\nDefault WiFi protocols at startup"));
  display_wifi_protocol();
  delay(500);

#ifdef SET_WIFI_PROTOCOL
  Serial.println(F("\tsetting WiFi protocol ..."));Serial.flush();
  // set WiFi mode 802.11b
  //_err = esp_wifi_set_protocol(WIFI_IF_STA, WIFI_PROTOCOL_11B | WIFI_PROTOCOL_11G);
  _err = esp_wifi_set_protocol(WIFI_IF_STA, WIFI_PROTOCOL_11B);
  if( _err != ESP_OK ) {
    //258 = ESP_ERR_INVALID_ARG
    Serial.print("\tCould not set protocol! \terr_code: ");Serial.print(_err);Serial.flush();
    delay(1000);
  }
  // esp_wifi_config_11b_rate(WIFI_IF_STA,true); // to suppress 802.11B if you set 802.11bg for example
#endif

  display_wifi_protocol();
  delay(500);

  Serial.println(F("\tChanging to AP mode ..."));
  WiFi.mode(WIFI_AP);
  display_wifi_protocol(WIFI_IF_AP);
  delay(500);

  Serial.println(F("\tSet WIFI_STA mode one more time ..."));
  WiFi.mode(WIFI_STA); // Optional
  display_wifi_protocol();
  delay(500);

  // Start connection
  WiFi.begin(ssid, password);

  uint8_t count=240;
  while(WiFi.status() != WL_CONNECTED && count ){
      Serial.print(".");
      delay(250);
      count--;
  }

 if( WiFi.status() != WL_CONNECTED ) {
      Serial.println(F("Failed to connect ... let's restart ..."));
      delay(2000);
      ESP.restart();
  }
  
  //if you get here you have connected to the WiFi    
  Serial.println("connected ...yeey :)");
  
  Serial.print("\nConnected to the WiFi network ");Serial.println(ssid);
  Serial.print("Local ESP32 IP: ");
  Serial.println(WiFi.localIP());
}



/* 
 * Loop
 */
void loop() {
  // put your main code here, to run repeatedly:
  Serial.print(".");
  delay(1000);
}
