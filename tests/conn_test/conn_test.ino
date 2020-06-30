#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
/*#include <WiFiManager.h>*/
/* MAC vars */
uint8_t macAddr[6];
char strMacAddr[32];

/* WiFi vars */
const char * wifi_ssid; //TODO
const char* wifi_pwd;   //TODO
WiFiClientSecure wifiClient;
PubSubClient client(wifiClient);


/* MQTTS vars */
char* mqtt_id;
char* mqtt_pwd;
const char* mqtt_server = "neocampus.univ-tlse3.fr";
const int  mqtt_port = 1883;

/* HTTPS vars */
const char* root_ca = NULL;
char url [90];

void setup_wifi(){
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
  Serial.println();
  Serial.print("ESP BOARD MAC Address: ");
  WiFi.macAddress(macAddr);
  snprintf( strMacAddr, sizeof(strMacAddr), "%02X:%02X:%02X:%02X:%02X:%02X", macAddr[0],macAddr[1],macAddr[2],macAddr[3],macAddr[4],macAddr[5]);
  Serial.println(strMacAddr);
  snprintf(url, sizeof(url), "%s%s%s","https://sensocampus.univ-tlse3.fr/device/credentials?mac=<",strMacAddr,">");
  Serial.println(url);
  Serial.flush();
  setup_wifi();

}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print('.');Serial.flush();
  delay(1000);
  if((WiFi.status() == WL_CONNECTED)){
    HTTPClient http;
    http.begin(url, root_ca);
    int httpCode = http.GET();
    if (httpCode > 0) { //Check for the returning code
      String payload = http.getString();
      Serial.println(httpCode);
      Serial.println(payload);
    }else {
      Serial.println("Error on HTTPs request");
    }
    http.end();
  }
}
