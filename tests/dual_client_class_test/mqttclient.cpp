#include "mqttclient.hpp"

mqttclient::mqttclient(char *login, char *pwd, char *server, unsigned int port, char *topic_base) : _login(login),_pwd(pwd),_server(server), _port(port),_topic_base(topic_base) { }

bool mqttclient::connect(){
    bool success;
    log_debug("--- beg of mqttclient::connect ---");
    if(_wcsClient){
        _client.setClient(*_wcsClient);
        _client.setServer(_server, _port);
        if(_client.connect(_server, _login, _pwd)){
            log_info("mqttclient connected")
            // TODO
            success = true;
        }else{
            success = false;
            log_error("mqttclient failed to connect");
        }
    }else{
        success = false;
        log_error("_wcsClient failed to setup");
    }
    log_debug("--- end of mqttclient::connect ---\n");
    return success;
}

void mqttclient::reconnect(){
    bool success = connect(); 
    log_debug("--- beg of mqttclient::reconnect ---\n");
    while(!success){
        success= connect();
    }
    log_debug("--- end of mqttclient::reconnect ---\n");
}

void callback(char* topic, byte* payload, unsigned int length){
    
}

bool mqttclient::add(base_class bc){
    bool success;
    log_debug("--- beg of mqttclient::add ---\n");
    if(_bc_cpt <= MAX_CLASS)
        _bc[_bc_cpt++] = bc;

    log_debug("--- end of mqttclient::add ---\n");
    return success;
}
void mqttclient::serialize(){
    //log_info("MQTT client status is :", _client.state());

    log_info("--- beg of mqttclient::serialize ---");
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
    snprintf(status, 48, "%s%d","mqttclient status: ",_client.state());
    log_info(status);
    log_info("--- end of mqttclient::serialize ---\n");
    Serial.println();Serial.flush();
}