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
    char log[64];
    if(LITTLEFS.mkdir(path)){
        snprintf(log, 32, "%s%s%s", "Directory ", path," has been created");
        log_debug(log);
    } else {
        snprintf(log, 32, "%s%s", "Failed to create ", path);
        log_error(log);
    }
}

void esp32_memory::rmdir(const char * path){
    char log[64];
    if(LITTLEFS.rmdir(path)){
        snprintf(log, 32, "%s%s%s", "Directory ", path," has been deleted");
        log_debug(log);
    } else {
        snprintf(log, 32, "%s%s", "Failed to delete ", path);
        log_error(log);
    }
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

void esp32_memory::write(const char* file, StaticJsonDocument<MAX_JSON_SIZE> buf){
    char log[64];
    File f = fs.open(path, FILE_WRITE);
    if(!file){
        snprintf(log, 64, "%s%s","Failed to open file ",file);
        log_error(log);
        return;
    }
    snprintf(log,64,"%s%s","The following file has been opened ", file);
    log_debug(log);
        bool err; = serializeJson(buf,f);
    if(err){
        log_error("failed to write buffer to memory");
    }else{
        log_debug("file wrote from buffer");
        log_debug(serializeJsonPretty(json_buf,Serial));
    }
    f.close();
}

StaticJsonDocument<MAX_JSON_SIZE> esp32_memory::read(const char *file){
    StaticJsonDocument<MAX_JSON_SIZE> json_buf;
    char log[64];
    File f = LITTLEFS.open(path);
    if(!f || f.isDirectory()){
        snprintf(log, 64, "%s%s","Failed to open file ",file);
        log_error(log);
        return;
    }
    snprintf(log,64,"%s%s","The following file has been opened ", file);
    log_debug(log);
    DeserializationError err = deserializeJson(json_buf, f);
    if(err){
        log_error("failed to read memory into buffer");
    }else{
        log_debug("buffer read from file");
        log_debug(serializeJsonPretty(json_buf,Serial));
    }
    f.close();
    return json_buf;
}
