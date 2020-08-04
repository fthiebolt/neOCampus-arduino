#include <WiFiManager.h>
#include <HTTPClient.h>
#include <LITTLEFS.h>
#include <ArduinoJson.h>
#include <Arduino.h>

#define log_debug(X,...) {Serial.print("[DBG] : ");Serial.println(X, ## __VA_ARGS__);Serial.flush();delay(1000);}
#define CRED_FILE "/cred.txt"
/* MAC vars */
uint8_t macAddr[6];
char strMacAddr[32];

/*WiFiManager vars */
char ssid[20]; /*WM ssid defined by device's class + part of MAC address*/
const char* pwd = "password"; /* needed 8 char pwd else wm do a failsafe and refuses to work.*/
WiFiManager wm;

/*HTTPS vars*/
HTTPClient httpsClient;
char url [90];
StaticJsonDocument<(JSON_OBJECT_SIZE(4)+4*32)> json_credentials;

/*
 * Retrieves the module's MAC address to use as AP reference when setting WiFi sor the first time
 * Also the MAC address is used as part of the authentification process to receive credentials
 */
void get_MAC(){
    WiFi.macAddress(macAddr);
    snprintf( strMacAddr, sizeof(strMacAddr), "%02X:%02X:%02X:%02X:%02X:%02X", macAddr[0],macAddr[1],macAddr[2],macAddr[3],macAddr[4],macAddr[5]);
    snprintf(ssid, sizeof(ssid),"%s%02X:%02X","airquality_",macAddr[4], macAddr[5]);
    log_debug(strMacAddr);
    log_debug(ssid);
    snprintf(url, sizeof(url), "%s%s","https://sensocampus.univ-tlse3.fr/device/credentials?mac=",strMacAddr);
    log_debug(url);
}

/*
 * WiFiManager handle the connecting to WiFi phase with the ssid and passwords provided
 * By connecting itself to this module AP, the user will be able to freely choose which network will be this module's reference for the rest of it's working life
 */
void set_WiFi(){
    log_debug("--- beg of set_WiFi ---");
    //wm.resetSettings();
    WiFi.mode(WIFI_STA);
    if(!wm.autoConnect(ssid,pwd)){
        log_debug("WiFiManager autoConnection OK");
    }else{
        log_debug("WiFiManager autoConnection KO");
    }
    log_debug("--- beg of set_WiFi ---");
}

/* 
 * Retrieval of credentials from auth server
 * If retrieval successful, credentials will be saved in "/cred.txt"
 */
void get_credentials(){
    log_debug("--- beg of get_credentials ---");
    if(httpsClient.begin(url)){
        int httpCode = httpsClient.GET();
        log_debug(httpCode);
        char log[64];
        String payload;
        //Check for the returning code
        if (httpCode > 0) { 
            log_debug("GET return code OK");
            payload = httpsClient.getString();
            /* Unboxing credentials from cred server */
            log_debug(json_credentials.capacity());
            log_debug(payload);
            DeserializationError error = deserializeJson(json_credentials, payload);
            log_debug("Credentials requested from auth");
            log_debug(url);
            // Test if parsing succeeds.
            if (error) {
                log_debug("deserializeJson() KO: ");
                log_debug(error.c_str());
            }else{
                log_debug("deserializeJson() OK");
                File file = LITTLEFS.open(CRED_FILE,FILE_WRITE);
                if(!file || file.isDirectory()){
                    snprintf(log, 64, "%s%s","Failed to open file ",CRED_FILE);
                    log_debug(log);
                    return;
                }
                snprintf(log,64,"%s%s","The following file has been opened ", CRED_FILE);
                log_debug(log);
                DeserializationError err = deserializeJson(json_credentials, file);
                if(err){
                    log_debug("writing to cred file KO");
                    return;
                }else{
                    log_debug("writing to cred file OK");
                    serializeJsonPretty(json_credentials,Serial);
                }
                file.close();
            }
        }else{
            log_debug("GET return code KO");
            return;
        }
        httpsClient.end();
    }
    log_debug("--- end of get_credentials ---");
}

void setup() {
    delay(3000);
    // put your setup code here, to run once:
    Serial.begin(115200);
    Serial.println(F("Hello ..."));
    bool success = LITTLEFS.begin(true);
    if(!success){
        log_debug("LittleFS Boot KO");
    }else{
        log_debug("LittleFS Boot OK");
        //LITTLEFS.mkdir("/");
        get_MAC();
        set_WiFi();
        /* for testing purposes, this file has to be rewritten everytime. However ths test SHALL BE in the neOScheduler library*/
        //if(!LITTLEFS.exists(CRED_FILE)){
            get_credentials();
        //}else{
        //    log_debug("credentials file OK");            
        //}

    }
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(1000);
  Serial.print(".");
  Serial.flush();
}
