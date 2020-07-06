#ifndef BASE_CLASS_HPP
#define BASE_CLASS_HPP

/*
 * Mother class from which all modules will inherit
 */
#include <Arduino.h>
#include "neologger.hpp"
class base_class{
public:
    /* constructor */
    base_class();
    /* sensor subscribing to a topic */
    bool subscribe(char *topic);
    /* sensor publishing a payload to a topic */
    bool publish(char* topic, char* payload);
    /* do sensor stuff... i guess? */
    void process();
private:
    /* type of sensor. Should I make a sensor type enum somewhere and use it instead of char *? */
    char *sen_type;
    /* how often does this sensor acquires data? */
    float acqusisition_frequency;
};





#endif