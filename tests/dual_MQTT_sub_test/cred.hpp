#ifndef CRED_HPP
#define CRED_HPP

#include <Arduino.h>
#include <ArduinoJson.h>

#define JSON_CRED_SIZE 256
#define LOGIN_SIZE 32
#define PWD_SIZE 64
#define SERVER_SIZE 32

#define DEFT_LOGIN "test"
#define DEFT_PWD "test"
#define DEFT_SERV "https://neocampus.univ-tlse3.fr"
#define DEFT_PORT 10888

class cred{
public:
    cred();
    cred(StaticJsonDocument<JSON_CRED_SIZE>);
    cred(const cred *c);

    inline const char *login(){ return _login; }
    inline const char *password(){return _password;}
    inline const char *server(){ return _server;}
    inline const int port() { return _port;}

private:
    const char *_login;
    const char *_password;
    const char *_server;
    const int _port;
};

#endif;