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
    /* starting the file system*/
    memory();
    /* Writes a buffer int a file 
     * return true if no errors
     */
    bool write(char* file, char* buf, int buf_size);
    /* Read from a file
     * return what has been read.
     */
    char* read(char* file);

};


#endif