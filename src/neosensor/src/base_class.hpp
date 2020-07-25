#ifndef BASE_CLASS_HPP
#define BASE_CLASS_HPP

#include <Arduino.h>
#include "const.hpp"
#include "mqttsclient.hpp"

class base_class{
public:
    base_class(const char *t, void (*on_message)(byte *, unsigned int));
    base_class();
    void on_connect();
    bool add();
    bool publish(char *);
    void (*on_message)(byte *, unsigned int);

    inline const char * topic(){
        return _topic;
    }

private:
    char *_sensor_array;
    const char *_topic;
    int _sensor_qty;
    mqttsclient _mqttsclient;
};

#endif
