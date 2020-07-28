#include "esp32_memory.hpp"

esp32_memory::esp32_memory() { }

bool esp32_memory::begin(bool format_if_fail){
    bool success;
    log_debug("memory warming up FS");
    success = LITTLEFS.begin(format_if_fail);
    if(!success)
        log_error("failed to boot up LITTLEFS");
}

void esp32_memory::makedir(const char * path){
    char log[64];
    if(LITTLEFS.mkdir(path)){
        snprintf(log, 32, "%s%s%s", "Directory ", path," has been created");
        log_debug(log);
    } else {
        snprintf(log, 32, "%s%s", "Failed to create ", path);
        log_error(log);
    }
}

void esp32_memory::rmvdir(const char * path){
    char log[64];
    if(LITTLEFS.rmdir(path)){
        snprintf(log, 32, "%s%s%s", "Directory ", path," has been deleted");
        log_debug(log);
    } else {
        snprintf(log, 32, "%s%s", "Failed to delete ", path);
        log_error(log);
    }
}
/*
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
    char output[40];
    snprintf(output, 40, "%s%s","checking existence of ",file);
    log_debug(output);
    return LITTLEFS.exists(file);
}

void esp32_memory::write(const char* path, StaticJsonDocument<MAX_JSON_SIZE> buf){
    log_debug("--- beg of esp32_memory::write ---");
    char log[64];
    File file = LITTLEFS.open(path, FILE_WRITE);
    if(!file){
        snprintf(log, 64, "%s%s","Failed to open file ",path);
        log_error(log);
        return;
    }
    snprintf(log,64,"%s%s","The following file has been opened ", path);
    log_debug(log);
        bool err = serializeJson(buf,file);
    if(err){
        log_error("failed to write buffer to memory");
    }else{
        log_debug("file wrote from buffer");
        log_debug(serializeJsonPretty(buf,Serial));
    }
    file.close();
    log_debug("--- end of esp32_memory::write ---");
    return;
}

StaticJsonDocument<MAX_JSON_SIZE> esp32_memory::read(const char *path){
    log_debug("--- beg of esp32_memory::read ---");
    StaticJsonDocument<MAX_JSON_SIZE> json_buf;
    char log[64];
    File file = LITTLEFS.open(path);
    if(!file || file.isDirectory()){
        snprintf(log, 64, "%s%s","Failed to open file ",path);
        log_error(log);
        return json_buf;
    }
    snprintf(log,64,"%s%s","The following file has been opened ", path);
    log_debug(log);
    DeserializationError err = deserializeJson(json_buf, file);
    if(err){
        log_error("failed to read memory into buffer");
    }else{
        log_debug("buffer read from file");
        log_debug(serializeJsonPretty(json_buf,Serial));
    }
    file.close();
    log_debug("--- end of esp32_memory::read ---");
    return json_buf;
}
