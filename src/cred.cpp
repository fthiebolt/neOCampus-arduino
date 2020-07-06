#include "cred.hpp"

cred::cred() : _login(DEFT_LOGIN),_password(DEFT_PWD), _server(DEFT_SERV), _port(DEFT_PORT) {}
cred::cred(StaticJsonDocument<JSON_CRED_SIZE> c) : _login{c["login"]}, _password{c["password"]},_server{c["server"]},_port(c["port"]){}