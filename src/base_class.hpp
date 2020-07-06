#ifndef BASE_CLASS_HPP
#define BASE_CLASS_HPP

#include <Arduino.h>
#include "neologger.hpp"
class base_class{
public:
    base_class();
    bool subscribe();
    bool publish();
    void process();

};





#endif