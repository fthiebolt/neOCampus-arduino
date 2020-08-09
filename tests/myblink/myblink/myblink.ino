/*
  myBlink

  Simple blink with serial message.

  Notes:
  Select ESP32 Nodes32s board (features a built-in led <-- GPIO2)
  [aug.20] beware that printing F('.') i.e with single quotes ==> exception !
*/

// TESTS TESTS TESTS
// #define LED_BUILTIN 2   // @Node32s board

// the setup function runs once when you press reset or power the board
void setup() {
  delay(3000);
  Serial.begin(115200);
  Serial.println(F("Hello from ESP32"));
  Serial.flush();
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  Serial.print(F("."));Serial.flush();

  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
  Serial.print(F("."));Serial.flush();

  // [aug.20] the latter F(' ') will lead to exception while F(" ") is OK !
  //Serial.print(F("."));Serial.flush();
}
