#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include "cred.hpp"

const char* wifi_ssid = "livebox-5C78";
const char* wifi_pwd = "9FE23C6C67A54E26D3EAEF3AC5";
const char* root_ca = NULL;

const int conf_port = 10888;

cred c;

WiFiClientSecure *wcsClient = new WiFiClientSecure;
PubSubClient client(c.server(), c.port(),*wcsClient);

bool check(char* msg, bool test){
    Serial.print(msg);
    if(test)
        Serial.println(": success");
    else
        Serial.println(": failure");
    return test;
}

void begin(){
    WiFiClientSecure *wcsClient = new WiFiClientSecure;
    bool test = wcsClient;
    if(!check("setting up wcs client", test))
        return;
    wcsClient -> setCACert(root_ca);
    if(client.connect(c.server(),c.login(), c.password())){
        test = client.subscribe("test/airquality/command");
        check("subscribing to topic airquality",test);
        test = client.subscribe("test/temperature/command");
        check("subscribing to topic temperature",test);
    }
}


void loop(){
    
}