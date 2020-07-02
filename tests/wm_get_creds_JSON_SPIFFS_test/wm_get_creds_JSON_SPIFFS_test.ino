#include <WiFiManager.h>
#include <HTTPClient.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <SPIFFS.h>

/* MAC vars */
uint8_t macAddr[6];
char strMacAddr[32];

/*WiFiManager vars */
char ssid[20]; /*WM ssid defined by device's class + part of MAC address*/
const char* pwd = "password"; /* need 8 char pwd else autoConnect fails.*/
WiFiManager wm;

/*HTTPS vars*/
HTTPClient httpsClient;
const char* root_ca = NULL;
char url [90];

/* JSon Vars */
StaticJsonDocument<200> JSON_cred;

/* MQTTS vars */
WiFiClientSecure wcs;

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

void get_credentials(){
  Serial.println("getting credentials");
  httpsClient.begin(url);
    int httpCode = httpsClient.GET();
    if (httpCode > 0) { //Check for the returning code
      String payload = httpsClient.getString();
      Serial.println(httpCode);
      Serial.println(payload);
      DeserializationError error = deserializeJson(JSON_cred, payload);

      // Test if parsing succeeds.
      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.c_str());
        return;
      }
      const char* login = JSON_cred["login"];
      const char* pwd = JSON_cred["password"];
      const char* server = JSON_cred["server"];
      int port = JSON_cred["port"];
      Serial.println(login);
      Serial.println(pwd);
      Serial.println(server);
      Serial.println(port);
      File file = SPIFFS.open("/credentials.txt", "w");
 
      if (!file) {
        Serial.println("Error opening file for writing");
        return;
      }
     
      int bytesWritten = file.print("Login");
      if (serializeJson(doc, file) == 0) {
        Serial.println(F("Failed to write to file"));
      }

      if (bytesWritten > 0) {
      Serial.println("File was written");
      Serial.println(bytesWritten);
   
      } else {
      Serial.println("File write failed");
    }
   
    file.close();
    }
    }else {
      Serial.println("Error on HTTPs request");
    }
    httpsClient.end();
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(1000);
  bool success = SPIFFS.begin();
 
  if (success) {
    Serial.println("File system mounted with success");
  } else {
    Serial.println("Error mounting the file system");
    return;
  }
  get_MAC();
  set_WiFi();
  if(!SPIFFS.exists("/creds.txt"));
    get_credentials();
  else{
    
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(1000);
  Serial.print(".");
  Serial.flush();
}
