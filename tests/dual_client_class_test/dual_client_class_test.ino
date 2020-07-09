#include "mqttclient.hpp"
#include "wifimanager.hpp"
#include "neologger.hpp"
#include <Arduino.h>
mqttclient client;
wifimanager wm;

void setup(){
    delay(3000);
    Serial.begin(115200);
    Serial.println(F("Setup begin"));
    wm.setup();
    client.connect();
    client.serialize();
    Serial.println(F("setup end"));
}

void loop(){
    delay(1000);
    Serial.print(".");
}