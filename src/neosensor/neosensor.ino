#include <Arduino.h>
#include <ArduinoJson.h>

#include "src/neoscheduler.hpp"
#include "src/base_class.hpp"
#include "src/neologger.hpp"

neoscheduler neo = neoscheduler();

void func1(byte *p, unsigned int l){
    log_debug("func 1 was called");
    return;
}

void func2(byte *p, unsigned int l){
    log_debug("func 2 was called");
    return;
}

base_class bc1("airquality",func1);
base_class bc2("temperature",func2);


void setup() {
  Serial.begin(115200);
  delay(3000);

  neo.setup();
}

void loop(){
   neo.loop();

   delay(250);
}
