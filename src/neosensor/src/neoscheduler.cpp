#include "neoscheduler.hpp"

neoscheduler::neoscheduler(float d) :_delay(d) {};

void neoscheduler::setup(){
    delay(3000);
    Serial.begin(115200);

}

void neoscheduler::loop(){
    delay(_delay);
}