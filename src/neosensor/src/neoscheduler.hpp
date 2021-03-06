#ifndef NEOSCHEDULER_HPP
#define NEOSCHEDULER_HPP


#include <ArduinoJson.h>
#include "const.hpp"
#include "esp32_memory.hpp"
#include "httpsclient.hpp"
#include "mqttsclient.hpp"
#include "neologger.hpp"
#include "wifimanager.hpp"

class neoscheduler {
public:
    neoscheduler( void );
    ~neoscheduler( void );

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
    wifimanager _wm;
    httpsclient _https;
    mqttsclient _mqtts;
    esp32_memory _mem;
    StaticJsonDocument<(JSON_OBJECT_SIZE (128))> _jso;
    uint8_t _mac_addr[6];
};
#endif