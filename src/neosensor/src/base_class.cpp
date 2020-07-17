#include "base_class.hpp"

/*base_class::base_class(char *t, void (*callback)()): _topic(t),_sensor_qty(0) {
    on_connect() = &callback();
};*/
base_class::base_class(): _topic(DEFT_TOPIC_BASE), _sensor_qty(0), on_message{NULL} {};
base_class::base_class(const char *t, void (*on_message)(byte *p, unsigned int l)): _topic{t}, _sensor_qty(0), on_message{on_message} {};

bool base_class::add(){
    _mqttclient.add(_topic, on_message);
    return false;
}

bool base_class::publish(char *topic){

    return false;
}
