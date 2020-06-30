#include <WiFi.h>
void setup() {
  delay(3000);
  Serial.begin(9600);
  Serial.println();
  Serial.print("ESP BOARD MAC Address: ");
  WiFi.macAddress(macAddr);
  snprintf( strMacAddr, sizeof(strMacAddr), "%02X:%02X:%02X:%02X:%02X:%02X", macAddr[0],macAddr[1],macAddr[2],macAddr[3],macAddr[4],macAddr[5]);
  Serial.println(strMacAddr);
  Serial.flush();
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print('.');Serial.flush();
  delay(1000);
}
