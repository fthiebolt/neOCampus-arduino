#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <Arduino.h>
#include <ArduinoJson.h>
#include <FS.h>
#include <LITTLEFS.h>
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
};



#endif