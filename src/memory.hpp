#ifndef MEMORY_HPP
#define MEMORY_HPP

/*
 * Memory managment. Currently using SPIFFS
 * However, since SPIFFS has been flagged "deprecated", need to work on migrating to LittleFS
 */ 

#include <Arduino.h>
#include <SPIFFS.h>
//#include <FS.h>
#include <ArduinoJson.h>
#include "neologger.hpp"

#define CREDS "/creds.txt"
#define MQTT_CONF "/config.txt"
#define WIFI_CONF "/wconf.txt"


class memory{
public:
    memory();
    bool write(char* buf, int buf_size);
    char* read(char* doc);

};


#endif