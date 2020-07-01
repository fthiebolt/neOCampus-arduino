#include <WiFi.h>
#define wifi_ssid "toto"
#define wifi_pwd "toto"
/* MAC vars */
uint8_t macAddr[6];
char strMacAddr[32];

void get_MAC(){
  Serial.println();
  Serial.print("ESP BOARD MAC Address: ");
  WiFi.macAddress(macAddr);
  snprintf( strMacAddr, sizeof(strMacAddr), "%02X:%02X:%02X:%02X:%02X:%02X", macAddr[0],macAddr[1],macAddr[2],macAddr[3],macAddr[4],macAddr[5]);
  Serial.println(strMacAddr);
  Serial.flush();
}

void get_IP(){
  delay(30);
  Serial.println();
  Serial.print("Connecting to : ");
  Serial.println(wifi_ssid);
  WiFi.begin(wifi_ssid,wifi_pwd);
  while(WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.println('.');
    Serial.flush();
  }
  Serial.println("Connected to the WiFi Network");
  Serial.println("IP Adress : ");
  Serial.println(WiFi.localIP());
}
void setup() {
  delay(3000);
  Serial.begin(9600);
  get_MAC();
  get_IP();
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print('.');Serial.flush();
  delay(1000);
}
