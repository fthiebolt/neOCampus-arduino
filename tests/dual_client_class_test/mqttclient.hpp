#ifndef MQTTCLIENT_hpp
#define MQTTCLIENT_hpp

#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>
#include "const.hpp"
#include "neologger.hpp"
#include "base_class.hpp"

class mqttclient{
public:
    /* Default constructor leads to the sandbox mode */
    mqttclient(const char *login = DEFT_LOGIN, const char *pwd = DEFT_PWD, const char *server = DEFT_SERVER, unsigned int port = DEFT_PORT,const char *topic_base = DEFT_TOPIC_BASE, const char *topic = DEFT_TOPIC);
    /* destructor */
    ~mqttclient();
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
    const char *_login; 
    const char *_pwd; 
    const char *_server;
    const char *_topic_base;
    const char *_topic;
    const unsigned int _port;

    base_class _bc[MAX_CLASS];
    unsigned int _bc_cpt;
    /* From the long Topic string returned by the callback, get the topic token*/ 
    char *split_topic(char *topic_chain);


};


#endif