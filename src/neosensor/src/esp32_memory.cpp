#include "esp32_memory.hpp"

esp32_memory::esp32_memory() { }

bool esp32_memory::begin(){
    bool success;
    log_debug("memory warming up FS");
    success = LITTLEFS.begin();
    if(!success)
        log_error("failed to boot up LITTLEFS");
}
/*
void esp3_memory::mkdir(const char * path){
    char dir[64];
    if(LITTLEFS.mkdir(path)){
        snprintf(dir, 32, "%s%s%s", "Directory ", path, " has been created");
        log_debug("directory  created");
    } else {
        snprintf(dir, 32, "%s%s", "Failed to create ", path);
        log_error("mkdir failed");
    }
}

void esp32_memory::rmdir(const char * path){
    if(fs.rmdir(path)){
        Serial.println("Dir removed");
    } else {
        Serial.println("rmdir failed");
    }
}

void readFile(fs::FS &fs, const char * path){
    Serial.printf("Reading file: %s\r\n", path);

    File file = fs.open(path);
    if(!file || file.isDirectory()){
        Serial.println("- failed to open file for reading");
        return;
    }

    Serial.println("- read from file:");
    while(file.available()){
        Serial.write(file.read());
    }
    file.close();
}

void writeFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Writing file: %s\r\n", path);

    File file = fs.open(path, FILE_WRITE);
    if(!file){
        Serial.println("- failed to open file for writing");
        return;
    }
    if(file.print(message)){
        Serial.println("- file written");
    } else {
        Serial.println("- write failed");
    }
    file.close();
}

void appendFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Appending to file: %s\r\n", path);

    File file = fs.open(path, FILE_APPEND);
    if(!file){
        Serial.println("- failed to open file for appending");
        return;
    }
    if(file.print(message)){
        Serial.println("- message appended");
    } else {
        Serial.println("- append failed");
    }
    file.close();
}

void renameFile(fs::FS &fs, const char * path1, const char * path2){
    Serial.printf("Renaming file %s to %s\r\n", path1, path2);
    if (fs.rename(path1, path2)) {
        Serial.println("- file renamed");
    } else {
        Serial.println("- rename failed");
    }
}

void deleteFile(fs::FS &fs, const char * path){
    Serial.printf("Deleting file: %s\r\n", path);
    if(fs.remove(path)){
        Serial.println("- file deleted");
    } else {
        Serial.println("- delete failed");
    }
}
*/
bool esp32_memory::exists(const char* file){
    bool success;
    char output[40];
    snprintf(output, 40, "%s%s","checking existence of ",file);
    log_debug(output);
    success = LITTLEFS.exists(file);
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
        File f = LITTLEFS.open(file, "r");
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
    if(exists(file)){
        File f = LITTLEFS.open(file, "r");
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
