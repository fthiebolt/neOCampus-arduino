#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <Arduino.h>
#include <ArduinoJson.h>
#include <SPIFFS.h>
#include "const.hpp"
#include "neologger.hpp"

class esp32_memory{
public:
    /* starting the file system*/
    esp32_memory();
    bool begin();
    /* checks if a file exists */
    bool exists(const char *file);
    /* Writes a buffer int a file */
    bool write(const char *file, StaticJsonDocument<MAX_JSON_SIZE> buf);
    /* Read from a file
     * return what has been read.
     */
    StaticJsonDocument<MAX_JSON_SIZE> read(const char *file);

    StaticJsonDocument<MAX_JSON_SIZE> read_configuration();
    StaticJsonDocument<MAX_JSON_SIZE> read_credentials();

    bool write_configuration(StaticJsonDocument<MAX_JSON_SIZE> buf);
    bool write_credentials(StaticJsonDocument<MAX_JSON_SIZE> buf);
};



#endif