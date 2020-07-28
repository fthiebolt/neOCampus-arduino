#ifndef HTTPSCLIENT_HPP
#define HTTPSCLIENT_HPP

#include <ArduinoJson.h>
#include <HTTPClient.h>
#include "const.hpp"
#include "neologger.hpp"

class httpsclient{
public :     
    httpsclient();

    StaticJsonDocument<JSON_OBJECT_SIZE(4)> get_credentials(char *);

    void get_config();
private:
    HTTPClient _client;
    char _url [90];
    const char* _root_ca = NULL;
};

#endif