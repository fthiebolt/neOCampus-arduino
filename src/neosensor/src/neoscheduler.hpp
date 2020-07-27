#ifndef NEOSCHEDULER_HPP
#define NEOSCHEDULER_HPP

#include "const.hpp"
#include "esp32_memory.hpp"
#include "httpsclient.hpp"
#include "mqttsclient.hpp"
#include "neologger.hpp"
#include "wifimanager.hpp"

class neoscheduler{
public:
    neoscheduler(float d);

    /* function to call in the .ino setup 
    * User don't have to manually setup everything hence this function
    */
    void setup();

    /* function to call in the .ino loop 
    * User inputs how often the loop should repeat (in seconds)
    */
    void loop();

    /* add user's custom publish and on_message functions */
    void add_sen_type();
private:
    float _delay;
    wifimanager _wm;
    httpsclient _httpsclient;
    mqttsclient _mqttsclient;

};
#endif