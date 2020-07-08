#include "memory.hpp"

memory::memory() {
    bool success;
    log_debug(F("memory warming up FS"));
    success = SPIFFS.begin();
    if(!success)
        log_error(F("failed to boot up SPIFFS"));
}

bool memory::exists(const char* file){
    bool success;
    log_debug(F("checking existsence of "));log_debug(Serial.print(file));
    success = SPIFFS.exists(file);
    if (!success)
        log_warning(F("file doesn't exist"));
    return success;
}

bool memory::write(const char* file, StaticJsonDocument<MAX_JSON_SIZE> buf){
    bool success;
    log_debug(F("writing to memory at "));log_debug(Serial.print(file));
    success = exists(file);
    if(!success)
        log_warning(F("file don't exist, creating it."));
    File f = SPIFFS.open(file, "r");
    if(!f){
        log_error(F("unable to open this file"));
        success = false;
    }else{
        log_debug(F("writing buffer"));
        success= serializeJson(buf,f);
        if(!success){
            log_error(F("failed to write buffer into memory"));
            abort();
        }else{
            log_debug(F("buffer written to file"));
        }
    }
    return success;
}
StaticJsonDocument<MAX_JSON_SIZE> memory::read(const char *file){
    StaticJsonDocument<MAX_JSON_SIZE> json_buf;
    log_debug(F("reading memory from "));log_debug(Serial.print(file));
    bool success = exists(file);
    if(!success)
        log_warning(F("file don't exist..."));
    File f = SPIFFS.open(file, "r");
    if(!f){
        log_error(F("unable to open this file"));
        success = false;
    }else{
        log_debug(F("reading memory"));
        DeserializationError err = deserializeJson(json_buf, f);
        if(err){
            log_error(F("failed to read memory into buffer"));
            success = false;
        }else{
            log_debug(F("buffer read from file"));
        }
    }
    return json_buf;
}
