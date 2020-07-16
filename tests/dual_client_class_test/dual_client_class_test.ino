#include "mqttclient.hpp"
#include "wifimanager.hpp"
#include "esp32_memory.hpp"
#include "neologger.hpp"
#include <Arduino.h>
#include <ArduinoJson.h>
#include "base_class.hpp"

mqttclient client;
wifimanager wm;
esp32_memory mem;
StaticJsonDocument<(JSON_OBJECT_SIZE (128))> jso;
//base_class bc[MAX_SENSORS];

void func1(byte *p, unsigned int l){
    log_debug("func 1 was called");
    return;
}

void func2(byte *p, unsigned int l){
    log_debug("func 2 was called");
    return;
}
void setup(){
    delay(3000);
    Serial.begin(115200);
    Serial.println(F("Setup begin"));
    mem.begin();
    wm.setup();
    client.connect();
    base_class bc1("airquality",func1);
    base_class bc2("temperature",func2);
    bc1.add();
    bc2.add();
    //client.subscribe();
    client.serialize();
    log_debug("Attempting to read conf from mem");

    jso = mem.read(MQTT_FILE);
    
    Serial.println(F("\nsetup end"));
}

void loop(){
    delay(1000);
    Serial.print(".");
}