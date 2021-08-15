/*
 * (c) neOCampus operation / Dr THIEBOLT François
 * 
 * Test output value memorization
 * We'll set an output with a value, switch to input, read input then
 * switch back to output.
 * 
 * Note: as of aug.21, when writing a value, it is set in the corresponding output
 * register; if you switch back to input (i.e disconnect from output register),
 * you read the real line value value but when you switch back to output mode, the
 * value of the line get reapplied from the corresponding output register :)
 */

// main output led
#ifndef LED
  #ifdef ESP8266
    #define LED             5   // GPIO5 (our main led + clear switch)
  #elif defined(ESP32)
    #define LED             15  // GPIO15 (our main led + clear switch)
                                // WARNING: GPIO5 on esp32 devkit V1 has 10k pull-up on 3v3
//  #define LED             INVALID_GPIO
  #endif
#endif

uint8_t _count = 0;
bool _led = HIGH;
bool _input;

void setup() {
  delay(3000);
  Serial.begin(115200);
  Serial.println(F("Hello from ESP32"));
  Serial.flush();

  // initialize digital pin LED as an output.
  pinMode(LED, OUTPUT);
  digitalWrite(LED, _led);
  Serial.print(F("\nset output to: "));Serial.print(_led); Serial.flush();
  //_led = not(_led);
  delay(1500);
}

void loop() {

  pinMode(LED, INPUT);
  _input = digitalRead(LED);
  Serial.print(F("\nset as input and read: "));Serial.print(_input); Serial.flush();
  delay(1500);

  pinMode(LED, OUTPUT);
  Serial.print(F("\nswitch back to output mode ..."));Serial.flush();
  delay(1500);

  if( _count++ >=5 ) {
    _count = 0;
    _led = not(_led);
    digitalWrite(LED, _led);
    Serial.print(F("\nchange output to: "));Serial.print(_led); Serial.flush();
  }
}
