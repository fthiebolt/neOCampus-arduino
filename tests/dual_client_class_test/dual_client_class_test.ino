#include "mqttclient.hpp"
#include "wifimanager.hpp"
#include "esp32_memory.hpp"
#include "neologger.hpp"
#include <Arduino.h>
#include <ArduinoJson.h>

mqttclient client;
wifimanager wm;
esp32_memory mem;
StaticJsonDocument<(JSON_OBJECT_SIZE (128))> jso;

void setup(){
    delay(3000);
    Serial.begin(115200);
    Serial.println(F("Setup begin"));

    //SPIFFS.begin();
    mem.begin();
    wm.setup();
    client.connect();
    client.serialize();
    log_debug("Attempting to read conf from mem");

    jso = mem.read(MQTT_FILE);
    
    Serial.println(F("\nsetup end"));
}

void loop(){
    delay(1000);
    Serial.print(".");
}