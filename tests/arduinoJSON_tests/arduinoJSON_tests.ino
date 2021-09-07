/*
 * neOCampus operation
 * 
 * ArduinoJSON tests:
 * - sharedJSON document reference across various modules (functions here)
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


void moduleTemperature( JsonDocument &sharedRoot ) {

  JsonVariant variant = sharedRoot.createNestedObject("temperature");
  JsonObject _obj = variant.as<JsonObject>();
  _obj[F("value_units")] = "°c";
/*
  JsonObject temperature = sharedRoot.createNestedObject("temperature");
  temperature["sensor1"] = "27.42";
*/
  JsonObject _obj2 = variant.as<JsonObject>();
  _obj2[F("value")] = "27.42";

  // now displaying global shared JSON structure
  log_debug(F("\nGlobal sharedJSON (within module):\n")); log_flush();
  serializeJsonPretty( sharedRoot, Serial );

  delay(1000);
}


// Setup
void setup() {
  delay(3000);
  Serial.begin(115200);
  Serial.println(F("\n\n### Hello from ESP32 !!! ###"));
  Serial.flush();

  moduleTemperature( sharedRoot );
/*  
  JsonObject temperature = sharedRoot.createNestedObject("temperature");
  temperature["sensor1"] = "27.42";
*/
  // now displaying global shared JSON structure
  log_debug(F("\nGlobal sharedJSON:\n")); log_flush();
  serializeJsonPretty( sharedRoot, Serial );
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(1000);
  log_debug(F(".")); log_flush();
}
