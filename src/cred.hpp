#ifndef CRED_HPP
#define CRED_HPP
/*
 * A convenient wait to manage all the credentials stuff.
 * 
 */
#include <Arduino.h>
#include <ArduinoJson.h>
#include "neologger.hpp"
/* credential answer size */
#define JSON_CRED_SIZE (JSON_OBJECT_SIZE(4))
/* credential field sizes */
#define LOGIN_SIZE 32
#define PWD_SIZE 64
#define SERVER_SIZE 32

#define SANDBOX_MODE true 
/* sandbox values */
#define DEFT_LOGIN "test"
#define DEFT_PWD "test"
#define DEFT_SERV "https://neocampus.univ-tlse3.fr"
#define DEFT_PORT 10888

class cred{
public:
    /* default constructor, mainly for testing purposes*/
    cred();
    /* initialising the fields from a JSON document fetched either from the auth server or internal file */
    cred(StaticJsonDocument<JSON_CRED_SIZE>);

    /* accessors only because no-touchy the credentials */
    inline const char *login(){ return _login; }
    inline const char *password(){return _password;}
    inline const char *server(){ return _server;}
    inline const int port() { return _port;}

private:
    const char _login[LOGIN_SIZE];
    const char _password[PWD_SIZE];
    const char _server[SERVER_SIZE];
    const int _port;
};

#endif;