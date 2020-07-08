#include <WiFiManager.h>
#include <HTTPClient.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <SPIFFS.h>

#define CREDENTIALS "/cred.txt"
#define CONFIG "/conf.txt"

#define DEFT_LOGIN "test"
#define DEFT_PWD "test"
#define DEFT_SERVER "neocampus.univ-tlse3.fr"
#define DEFT_PORT "10888"
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
const char* config_server = "https://sensocampus.univ-tlse3.fr/device/config";

/* JSon Vars */
StaticJsonDocument<200> JSON_cred;
StaticJsonDocument<1000> JSON_conf;/*
const char* cred_login = JSON_cred["login"];
const char* cred_pwd = JSON_cred["password"];
const char*  cred_server = JSON_cred["server"];
int cred_port = JSON_cred["port"];*/


/* MQTTS vars */
WiFiClientSecure *WCSClient = new WiFiClientSecure;
PubSubClient client(*WCSClient);

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
  }
}

void get_credentials(){
  Serial.println("getting credentials");
  httpsClient.begin(url,root_ca);
    int httpCode = httpsClient.GET();
    if (httpCode > 0) { //Check for the returning code
      String payload = httpsClient.getString();
      Serial.println(httpCode);
      Serial.println(payload);
      /* Unboxing credentials from cred server */
      DeserializationError error = deserializeJson(JSON_cred, payload);
      Serial.println("Credentials requested from auth");
      // Test if parsing succeeds.
      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.c_str());
        return;
      }
      
      /* Saving credentials to SPIFFS */
      File file = SPIFFS.open(CREDENTIALS, "w");
      if (!file) {
        Serial.println("Error opening file for writing");
        return;
      }
      if (serializeJson(JSON_cred, file) == 0) {
        Serial.println(F("Failed to write to file"));
      } else {
        Serial.println(F("Credentials successfully saved"));
      }
      file.close();
    }else {
      Serial.println(F("Error on HTTPs request"));
    }
    httpsClient.end();
}

void get_conf(){
  Serial.println("Attempting to get conf");
  const char* cred_login = JSON_cred["login"];
  const char* cred_pwd = JSON_cred["password"];
  const char*  conf_server = JSON_cred["server"];
  int conf_port = JSON_cred["port"];
  httpsClient.begin(config_server, root_ca );
  httpsClient.setAuthorization(cred_login, cred_pwd);
  int httpCode = httpsClient.GET();
   if (httpCode > 0) { //Check for the returning code
     String payload = httpsClient.getString();
     Serial.println(httpCode);
     //Serial.println(payload);
     /* Unboxing credentials from cred server */
     DeserializationError error = deserializeJson(JSON_conf, payload);
     // Test if parsing succeeds.
     if (error) {
       Serial.print(F("deserializeJson() failed: "));
       Serial.println(error.c_str());
       return;
     }
     serializeJsonPretty(JSON_conf,Serial);
     File file = SPIFFS.open(CONFIG, "w");
      if (!file) {
        Serial.println("Error opening file for writing");
        return;
      }
      if (serializeJson(JSON_conf, file) == 0) {
        Serial.println(F("Failed to write to config.txt"));
      } else {
        Serial.println(F("Config successfully saved"));
      }
      file.close();
   } 
   httpsClient.end();
}
void callback(char* topic, byte* payload, unsigned int length){
  // byte* p = (byte*)malloc(length);
  // memcpy(p, payload,length);
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  
}

void mqtts(){
  Serial.println("Attempting to get conf");
  if(WCSClient) {
    WCSClient -> setCACert(root_ca);
    const char* cred_login = JSON_cred["login"];
    const char* cred_pwd = JSON_cred["password"];
    const char*  conf_server = JSON_cred["server"];
    int conf_port = JSON_cred["port"]; 
    client.setServer(conf_server, conf_port);
    if(client.connect(conf_server,cred_login, cred_pwd)){
      const char* topic_base = JSON_conf["topic"];
      //Serial.println("Topic found:");
      //Serial.println(topic_base);
      const char* topic_class = JSON_conf["module"];
      //Serial.println("class found:");
      //Serial.println(topic_class);
      if(!client.subscribe("testTopic/airquality/command")){
        Serial.println("Failed to subscribe");
      }else{
        Serial.println("subscribing done");
      }
      client.setCallback(callback);
      //snprintf(topic,sizeof(topic),"%s/%s",topic, "/command");
      //Serial.print(F("Topic: "));
      //Serial.println(F(topic));
      //if(!client.publish("device/command")){
      //  Serial.println("Failed to subscribe");  
      //}else{
      //  Serial.println("subscribing done");
      //}
    }
  }else{
    Serial.println("Failed to set up WiFiClientSecure");
  }
}

void reconnect(){
  while(client.connected()){
    Serial.print(F("MQTT connection pending..."));
    if(client.connect(DEFT_SERVER)){
      Serial.println(F("Connected"));
      client.publish("test/Airquality","hello world");
      client.subscribe("airquality/command");
    }else{
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  delay(3000);
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println(F("Hello ..."));
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
  /* Do I have my credentials? */
  if(!SPIFFS.exists(CREDENTIALS))
    get_credentials();
  else{
  /* Yes i do */
    File file = SPIFFS.open(CREDENTIALS);
    if(!file){
      Serial.println("Failed to open file for reading");
      return;
    }
  /* credentials are stored as StaticJsonDocument, let's deserialize it*/
    DeserializationError error = deserializeJson(JSON_cred, file);
  // Test if parsing succeeds.
    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.c_str());
      return;
    }
    file.close();
    Serial.println("Internal credentials found");
  } 
  Serial.println("I Have those credentials: ");
  serializeJsonPretty(JSON_cred,Serial);
  if(!SPIFFS.exists(CONFIG))
    get_conf(); 
  else{
  /* Yes i do */
    File file = SPIFFS.open(CONFIG);
    if(!file){
      Serial.println("Failed to open config for reading");
      return;
    }
  /* config is stored as StaticJsonDocument, let's deserialize it*/
    DeserializationError error = deserializeJson(JSON_conf, file);
  // Test if parsing succeeds.
    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.c_str());
      return;
    }
    file.close();
    Serial.println("Internal config found");
    serializeJsonPretty(JSON_conf,Serial);
  }
  mqtts();
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(1000);
  Serial.print(".");
  Serial.flush();
  if(!client.connected()){
    reconnect();
  }
  client.loop();
}
