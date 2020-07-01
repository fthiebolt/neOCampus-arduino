#include <WiFiManager.h>

/* MAC vars */
uint8_t macAddr[6];
char strMacAddr[32];

/*WiFiManager vars*/
char ssid[20];
//const char* ssid = "NEED_WM_CFG"; /* Does someone got a better name ?*/
const char* pwd = "password"; /* need 8 char pwd else autoConnect fails.*/
WiFiManager wm;

void get_MAC(){
  Serial.println();
  Serial.print("ESP BOARD MAC Address: ");
  WiFi.macAddress(macAddr);
  snprintf( strMacAddr, sizeof(strMacAddr), "%02X:%02X:%02X:%02X:%02X:%02X", macAddr[0],macAddr[1],macAddr[2],macAddr[3],macAddr[4],macAddr[5]);
  snprintf(ssid, sizeof(ssid),"%s%02X:%02X","airquality_",macAddr[4], macAddr[5]);
  Serial.println(strMacAddr);
  Serial.println(ssid);
  Serial.flush();
}

void set_WiFi(){
  wm.resetSettings();
  WiFi.mode(WIFI_STA);
  if(!wm.autoConnect(ssid,pwd))
    Serial.println("Conn Doomed");
  else{
    Serial.println("Conn est");
    Serial.print("New IP Addr: ");
    Serial.println(WiFi.localIP());
  }
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(1000);
  get_MAC();
  set_WiFi();
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(1000);
  Serial.print(".");
  Serial.flush();
}
