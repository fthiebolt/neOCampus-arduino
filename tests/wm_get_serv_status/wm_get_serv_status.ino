#include <WiFiManager.h>
#include <HTTPClient.h>

/* MAC vars */
uint8_t macAddr[6];
char strMacAddr[32];

/*WiFiManager vars */
char ssid[20]; /*WM ssid defined by device's class + part of MAC address*/
const char* pwd = "password"; /* need 8 char pwd else autoConnect fails.*/
WiFiManager wm;

/*HTTPS vars*/
HTTPClient httpsClient;
char url [90];

void get_MAC(){
  Serial.println();
  Serial.print("ESP BOARD MAC Address: ");
  WiFi.macAddress(macAddr);
  snprintf( strMacAddr, sizeof(strMacAddr), "%02X:%02X:%02X:%02X:%02X:%02X", macAddr[0],macAddr[1],macAddr[2],macAddr[3],macAddr[4],macAddr[5]);
  snprintf(ssid, sizeof(ssid),"%s%02X:%02X","airquality_",macAddr[4], macAddr[5]);
  Serial.println(strMacAddr);
  Serial.println(ssid);
  snprintf(url, sizeof(url), "%s%s","https://sensocampus.univ-tlse3.fr/device/credentials?mac=",strMacAddr);
  Serial.println(url);
  Serial.flush();
}

void set_WiFi(){
  //wm.resetSettings();
  WiFi.mode(WIFI_STA);
  if(!wm.autoConnect(ssid,pwd))
    Serial.println("Conn Doomed");
  else{
    Serial.println("Conn est");
  }
}

void get_credentials(){
    Serial.println("getting credentials");
    httpsClient.begin(url);
    int httpCode = httpsClient.GET();
    Serial.println(httpCode);
    httpsClient.end();
}

void setup() {
  delay(3000);
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println(F("Hello ..."));
  delay(1000);
  get_MAC();
  set_WiFi();
  get_credentials();
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(1000);
  Serial.print(".");
  Serial.flush();
}