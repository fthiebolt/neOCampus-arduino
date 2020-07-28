#include <Arduino.h>
#include <ArduinoJson.h>
#include "src/mqttsclient.hpp"
#include "src/wifimanager.hpp"
#include "src/esp32_memory.hpp"
#include "src/neologger.hpp"
#include "src/base_class.hpp"

mqttsclient client;
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

base_class bc1("airquality",func1);
base_class bc2("temperature",func2);

void setup(){   
    delay(3000);
    Serial.begin(115200);
    Serial.println(F("Setup begin"));
    mem.begin();
    wm.setup();
    byte b[] = {0x43, 0x55,0x87};
    bc1.on_message(b, 0);
    bc1.add();
    //bc2.add();
    //client.subscribe();
    client.connect();
    client.serialize();
    
    log_debug("Attempting to read conf from mem");
    if(mem.exists(CRED_FILE)){
        jso = mem.read(CRED_FILE);
    }else{
        log_error("this file wasn't found in memory");
    }
    if(mem.exists(MQTT_FILE)){
        jso = mem.read(MQTT_FILE);
    }else{
        log_error("this file wasn't found in memory");
    }
    Serial.println(F("\nsetup end"));
}

void loop(){
    delay(1000);
    Serial.print(".");
}