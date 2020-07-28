#include "mqttsclient.hpp"

mqttsclient::mqttsclient(const char *login, const char *pwd, const char *server, unsigned int port, const char *topic_base, const char *topic) : _login(login), _pwd(pwd), _server(server), _port(port), _topic_base(topic_base), _topic(topic), _numb_users(0) { };

mqttsclient::~mqttsclient(){
    delete(_topic);
    delete(_login);
    delete(_pwd);
    delete(_server);
    delete(_topic_base);
}

bool mqttsclient::connect(){
    bool success;
    log_debug("--- beg of mqttsclient::connect ---");
    if(_wcsClient){
        _client.setClient(*_wcsClient);
        _client.setServer(_server, _port);
        _client.setCallback([this](char *t, byte *p, unsigned int l){this->on_message(t,p,l);});
        if(_client.connect(_server, _login, _pwd)){
            log_info("mqttsclient connected")
            
            success = true;
        }else{
            success = false;
            log_error("mqttsclient failed to connect");
        }
    }else{
        success = false;
        log_error("_wcsClient failed to setup");
    }
    log_debug("--- end of mqttsclient::connect ---\n");
    Serial.flush();
    delay(1000);
    return success;
}

void mqttsclient::reconnect(){
    bool success = connect(); 
    log_debug("--- beg of mqttsclient::reconnect ---\n");
    while(!success){
        success= connect();
    }
    log_debug("--- end of mqttsclient::reconnect ---\n");
}

bool mqttsclient::subscribe(){
    log_debug("--- beg of mqttsclient::subscribe ---\n");
    char base_token[64];
    for(int i = 0; i < _numb_users; ++i){
        snprintf(base_token,64,"%s/%s/%s",DEFT_TOPIC_BASE,_users[i].topic,"command");
        log_debug(base_token);
        if(!_client.subscribe(base_token))
            return false;
    }
    log_debug("--- beg of mqttsclient::subscribe ---\n");
    return true;;
}

void mqttsclient::on_message(char *t_chain, byte *payload, unsigned int length){
    log_debug("--- beg of mqttsclient::on_message ---\n");
    //split topic token
    const char *topic = split_topic(t_chain);
    //use the right on_message function
    for(int i = 0; i < _numb_users; ++i){
        if(strncmp(topic,_users[i].topic,_users[i].topic_size)==0)
            (_users[i].on_message)(payload, length);
    }
    log_debug("--- end of mqttsclient::on_message ---\n");
    return;
}

bool mqttsclient::add(const char* topic, void(*on_message)(byte *playload, unsigned int length)){
    log_debug("--- beg of mqttsclient::add ---\n");
    bool success;
    char numb_usr[64];
    snprintf(numb_usr, 64, "%s%d%s","This module has ",_numb_users," different sensor types");
    log_debug(numb_usr);
    _users[_numb_users].topic = topic;
    log_debug(_users[_numb_users].topic);
    _users[_numb_users].topic_size = size_t(sizeof(topic));
    log_debug(_users[_numb_users].topic_size);
    _users[_numb_users].on_message = on_message;
    log_debug("sensor type added");
    ++ _numb_users;
    snprintf(numb_usr, 64, "%s%d%s","This module has now: ",_numb_users," different sensor types");
    log_debug(numb_usr);
    log_debug("--- end of mqttsclient::add ---\n");
    return success;
}

char * mqttsclient::split_topic(char *tok_str){
    char * topic;

    return tok_str;
}

void mqttsclient::serialize(){
    log_info("--- beg of mqttsclient::serialize ---");
    char login[48];
    snprintf(login, 48, "%s%s","login: ",_login);
    log_info(login);
    char pwd[80];
    snprintf(pwd, 80, "%s%s","password: ",_pwd);
    log_info(pwd);
    char server[48];
    snprintf(server, 48, "%s%s","server: ",_server);
    log_info(server);
    char port[16];
    snprintf(port, 16, "%s%d","port: ",_port);
    log_info(port);
    char bt[48];
    snprintf(bt, 48, "%s%s", "topic_base: ", _topic_base);
    log_info(bt);
    char status[48];
    snprintf(status, 48, "%s%d","mqttsclient status: ",_client.state());
    log_info(status);
    log_info("--- end of mqttsclient::serialize ---\n");
}