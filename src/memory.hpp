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

#define CRED_FILE "/creds.txt"
#define MQTT_FILE "/config.txt"
#define WIFI_FILE "/wconf.txt"

#define FILE_SIZE 10
#define MAX_JSON_SIZE (JSON_OBJECT_SIZE(128))
class memory{
public:
    /* starting the file system*/
    memory();
    /* checks if a file exists */
    bool exists(const char *file);
    /* Writes a buffer int a file */
    bool write(const char *file, StaticJsonDocument<MAX_JSON_SIZE> buf);
    /* Read from a file
     * return what has been read.
     */
    StaticJsonDocument<MAX_JSON_SIZE> read(const char *file);
};


#endif