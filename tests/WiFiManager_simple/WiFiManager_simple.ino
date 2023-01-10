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
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include <esp_wifi.h>
#include <esp_log.h>

// Defines
#define SET_WIFI_PROTOCOL   1

/* Global variables */
const char* ssid = "freebox_Bonzo";
const char* password = "Hedgehog";
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


TO BE CONTINUED


/* 
 * Setup
 */
void setup() {
  delay(3000);  // time for USB serial link to come up anew
  Serial.begin(115200); // Start serial for output

  Serial.println(F("\n\n###\n### Starting simple WiFiManager connection test ...\n###"));

  // Arduino libs v2.4.1, to enable printf and debug messages output
  Serial.setDebugOutput( true );

  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
  // it is a good practice to make sure your code sets wifi mode how you want it.

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
 
  //WiFiManager, Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wm;
  wm.setDebugOutput(true);

  Serial.println(F("\tconnection timeout to 5mn ..."));
  wm.setConnectTimeout(300);

    bool res;
    // res = wm.autoConnect(); // auto generated AP name from chipid
    // res = wm.autoConnect("AutoConnectAP"); // anonymous ap
    res = wm.autoConnect(ssid,passwd); // password protected ap

    if(!res) {
        Serial.println("Failed to connect");
        // ESP.restart();
    } 
    else {
        //if you get here you have connected to the WiFi    
        Serial.println("connected...yeey :)");
    }

}


/* 
 * Loop
 */
void loop() {
    // put your main code here, to run repeatedly:
    Serial.print(".");
    delay(1000);   
}
