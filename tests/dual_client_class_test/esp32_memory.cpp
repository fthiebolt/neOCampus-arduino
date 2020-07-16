#include "esp32_memory.hpp"

esp32_memory::esp32_memory() { }

bool esp32_memory::begin(){
    bool success;
    log_debug("memory warming up FS");
    success = SPIFFS.begin();
    if(!success)
        log_error("failed to boot up SPIFFS");
}

bool esp32_memory::exists(const char* file){
    bool success;
    char output[40];
    snprintf(output, 40, "%s%s","checking existence of ",file);
    log_debug(output);
    success = SPIFFS.exists(file);
    if (!success)
        log_warning("file doesn't exist");
    return success;
}

bool esp32_memory::write(const char* file, StaticJsonDocument<MAX_JSON_SIZE> buf){
    bool success;
    char output[64];
    snprintf(output, 64, "%s%s","writing to memory at ",file);
    log_debug(output);
    //delete [] output;
    Serial.flush();
    success = exists(file);
    if(!success){
        log_warning("file don't exist, creating it.");
    }else{
        File f = SPIFFS.open(file, "r");
        if(!f){
            log_error("unable to open this file");
            success = false;
        }else{
            log_debug("writing buffer");
            success= serializeJson(buf,f);
            if(!success){
                log_error("failed to write buffer into memory");
                abort();
            }else{
                log_debug("buffer written to file");
            }
        }
    }
    return success;
}

StaticJsonDocument<MAX_JSON_SIZE> esp32_memory::read(const char *file){
    StaticJsonDocument<MAX_JSON_SIZE> json_buf;
    char output[64];
    snprintf(output, 64, "%s%s","reading memory from ",file);
    log_debug(output);
    //delete(output);
    Serial.flush();
 
    if(exists(file)){
        File f = SPIFFS.open(file, "r");
        if(!f){
            log_error("unable to open file");
        }else{
            log_debug("reading memory");
            DeserializationError err = deserializeJson(json_buf, f);
            if(err){
                log_error("failed to read memory into buffer");
            }else{
                log_debug("buffer read from file");
                log_debug(serializeJsonPretty(json_buf,Serial));
            }
        }
    }
    return json_buf;
}

StaticJsonDocument<MAX_JSON_SIZE> esp32_memory::read_configuration(){
    StaticJsonDocument<MAX_JSON_SIZE> json_buf;
    json_buf = read(MQTT_FILE);
    return json_buf;
}

StaticJsonDocument<MAX_JSON_SIZE> esp32_memory::read_credentials(){
    return read(CRED_FILE);
}

bool esp32_memory::write_configuration(StaticJsonDocument<MAX_JSON_SIZE> buf){
    return write(MQTT_FILE, buf);
}

bool esp32_memory::write_credentials(StaticJsonDocument<MAX_JSON_SIZE> buf){
    return write(CRED_FILE, buf);
}
