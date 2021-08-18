/*
 * neOCampus operation
 * 
 * ArduinoJSON tests
 * 
 * TODO: find a way to obtain jsonObject memory address
 * 
 * F.Thiebolt aug.21  initial release
 */
#include <ArduinoJson.h>
#include "neocampus.h"
#include "neocampus_debug.h"
#include "neocampus_utils.h"

// Definitions
#define MODULES_SHARED_JSON_SIZE  512
//StaticJsonDocument<MODULES_SHARED_JSON_SIZE> sharedRoot;    // stack allocation
DynamicJsonDocument sharedRoot(MODULES_SHARED_JSON_SIZE);   // heap allocation

void setup() {
  delay(3000);
  Serial.begin(115200);
  Serial.println(F("\n\n### Hello from ESP32 !!! ###"));
  Serial.flush();
/*
  char *myString = "Hello from esp32 !!!";
  byte *pointer = (byte*)myString;
  Serial.println( (unsigned long)pointer,HEX);
  hex_dump( myString, 64, true );
*/
  // put your setup code here, to run once
  delay(500);
  log_debug(F("\n[setup] inserting keys ..."));log_flush();
  JsonObject _obj = sharedRoot.to<JsonObject>();
  for( uint8_t i=0; i<10; i++ ) {
    String _key = "GPIO";
    _key += 42+i;
    _obj[_key] = 100+i;
  }

  // dump memory
  log_debug(F("\n[setup] ... now dump JSON document ..."));log_flush();
  delay(1000);
  hex_dump( (const char *)_obj, MODULES_SHARED_JSON_SIZE, true);
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(1000);
  log_debug(F(".")); log_flush();
}
