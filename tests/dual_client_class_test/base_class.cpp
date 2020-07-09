#include "base_class.hpp"

base_class::base_class(char *t, void *(callback)): _topic(t),_sensor_qty(0),on_message(*callback) {};
