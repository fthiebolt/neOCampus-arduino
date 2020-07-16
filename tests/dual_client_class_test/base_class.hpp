#ifndef BASE_CLASS_HPP
#define BASE_CLASS_HPP

#include "const.hpp"

class base_class{
public:
    //base_class(char *t, void (*callback)());
    base_class();
    base_class(const char *t);
    void on_connect();
    inline const char * topic(){
        return _topic;
    }
private:
    char *_sensor_array;
    const char _topic[TOPIC_SIZE];
    int _sensor_qty;
};

#endif
