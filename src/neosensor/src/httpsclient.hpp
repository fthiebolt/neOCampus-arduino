#ifndef HTTPSCLIENT_HPP
#define HTTPSCLIENT_HPP

#include <Arduino.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include "const.hpp"
#include "neologger.hpp"

class httpsclient{
public :     
    httpsclient();

    StaticJsonDocument<CRED_JSON_SIZE> get_credentials(uint8_t *);
    
    void get_config();
private:
    HTTPClient _https;
    char _url[90];
    char *_root_ca = NULL;
    
};

#endif