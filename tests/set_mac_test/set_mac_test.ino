#include <WiFi.h>
#include <esp_wifi.h>
#include <M5StickCPlus.h>

// Set your new MAC Address
uint8_t newMACAddress[] = {0x4C, 0x75, 0x25, 0x9E, 0xA3, 0x77};

void setup(){
  M5.begin();
  Serial.begin(115200);
  Serial.println();
  
  WiFi.mode(WIFI_STA);
  
  Serial.print("[OLD] ESP32 Board MAC Address:  ");
  Serial.println(WiFi.macAddress());
  esp_wifi_set_mac(WIFI_IF_STA, &newMACAddress[0]);
  
  Serial.print("[NEW] ESP32 Board MAC Address:  ");
  Serial.println(WiFi.macAddress());
  M5.Lcd.print(WiFi.macAddress());
}
 
void loop(){

}