#ifndef MODULE_HPP
#define MODULE_HPP

#include <Arduino.h>
#include "base_class.hpp"
#include "cred.hpp"
#include "neologger.hpp"

#define MAC_SIZE 16
#define MAX_SENSORS 10
class module{
public:
    module();
    bool add();
    bool start();
    bool stop();
    int process_all();
private:
    static char _mac[MAC_SIZE];
    base_class *_sensorList[MAX_SENSORS];
};


#endif