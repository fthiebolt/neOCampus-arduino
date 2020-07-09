#ifndef MQTTCLIENT_hpp
#define MQTTCLIENT_hpp

#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>
#include "neologger.hpp"
#include "base_class.hpp"

#define DEFT_LOGIN "test"
#define DEFT_PWD "test"
#define DEFT_SERVER "neocampus.univ-tlse3.fr"
#define DEFT_PORT 8883
#define DEFT_TOPIC_BASE "TestTopic"

#define MAX_CLASS 10

class mqttclient{
public:
    /* Default constructor leads to the sandbox mode */
    mqttclient(char *login = DEFT_LOGIN, char *pwd = DEFT_PWD, char *server = DEFT_SERVER, unsigned int port = DEFT_PORT, char *topic_base = DEFT_TOPIC_BASE);
    /* callback renamed to fit neOCampus logic? */
    void on_message(char *topic, byte *payload, unsigned int length); //TODO
    /* subscribe to topic */
    bool subscribe(const char* topic); //TODO
    /* publish */
    bool publish(); //TODO
    /* add a sensor class to the sensitivity list*/
    bool add(base_class bc); //TODO
    /* connection to the MQTT broker*/
    bool connect();
    /* reconnect if connection lost */
    void reconnect();
    /* print attributes to serial */
    void serialize();

private:
    WiFiClientSecure *_wcsClient = new WiFiClientSecure;
    PubSubClient _client; 
    const char *_topic;
    const char *_login; 
    const char *_pwd; 
    const char *_server;
    const char *_topic_base;
    const unsigned int _port;
    base_class _bc[MAX_CLASS];
    unsigned int _bc_cpt;
    /* From the long Topic string returned by the callback, get the topic token*/ 
    char *split_topic(char *topic_chain);


};


#endif