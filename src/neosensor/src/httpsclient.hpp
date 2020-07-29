#ifndef HTTPSCLIENT_HPP
#define HTTPSCLIENT_HPP

#include <ArduinoJson.h>
#include <HTTPClient.h>
#include "const.hpp"
#include "neologger.hpp"

class httpsclient{
public :     
    httpsclient();

    StaticJsonDocument<JSON_OBJECT_SIZE(4)> get_credentials(uint8_t *);
    
    void get_config();
private:
    HTTPClient _https;
    char _url[90];
    char *_root_ca = NULL;
    
};

#endif