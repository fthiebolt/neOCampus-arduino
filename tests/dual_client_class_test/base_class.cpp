#include "base_class.hpp"

/*base_class::base_class(char *t, void (*callback)()): _topic(t),_sensor_qty(0) {
    on_connect() = &callback();
};*/
base_class::base_class(): _topic(DEFT_TOPIC_BASE), _sensor_qty(0) {};
base_class::base_class(const char *t): _topic{*t},_sensor_qty(0) {};

