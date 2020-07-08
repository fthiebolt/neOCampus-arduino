#ifndef MQTT_HPP
#define MQTT_HPP

#include <PubSubClient.h>
#include <WiFiClientSecure.h>

class mqtt{
public:
    /* setup of the board's MQTT client */
    mqtt();
    /*subscribing to all topics */
    bool subscribe(char* sensor, char* topic);
    /*publishing to a topic */
    bool publish(char* topic, char* payload);
    /* callback reaction to an update in a subscribed client*/
    bool callback(char* topic, byte* payload, unsigned int length);
private:
    WiFiClientSecure _wcsClient;
};

#endif