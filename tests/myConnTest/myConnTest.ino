#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <Arduino.h>

/* MAC vars */
uint8_t macAddr[6];
char strMacAddr[32];

/* WiFi vars */
const char *wifi_ssid = "YOUR_SSID"; //TODO
const char *wifi_pwd = "YOUR_PSK";  //TODO


/* HTTPS vars */
const char* root_ca = NULL;
char url[90];

void setup_wifi(){
  delay(30);
  Serial.println();
  Serial.print("Connecting to : ");
  Serial.println(wifi_ssid);
  WiFi.begin(wifi_ssid,wifi_pwd);
  while(WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.println('.');
    Serial.flush();
  }
  Serial.println("Connected to the WiFi Network");
  Serial.println("IP Adress : ");
  Serial.println(WiFi.localIP());
  Serial.print(F("WiFi SSID: "));Serial.println(WiFi.SSID());Serial.flush();
  Serial.print(F("WiFi PSK: "));Serial.println(WiFi.psk());Serial.flush();
}

void setup() {
	delay(3000);
	Serial.begin(112500);
	Serial.println();
	Serial.print("ESP BOARD MAC Address: ");
	WiFi.macAddress(macAddr);
	snprintf( strMacAddr, sizeof(strMacAddr), "%02X:%02X:%02X:%02X:%02X:%02X", macAddr[0],macAddr[1],macAddr[2],macAddr[3],macAddr[4],macAddr[5]);
	Serial.println(strMacAddr);
	snprintf(url, sizeof(url), "%s%s%s","https://sensocampus.univ-tlse3.fr/device/credentials?mac=<",strMacAddr,">");
	Serial.println(url);
	Serial.flush();
 
	setup_wifi();
}

void loop() {
	// put your main code here, to run repeatedly:
	Serial.print('.');Serial.flush();
	delay(1000);
#if 0
	//WiFiClientSecure *wcs;
	//if(wcs){
		//wcs->setCACert(root_ca);
		HTTPClient http;
		if(http.begin(url)){
			Serial.print(F("Begin ok, calling GET"));
			int httpCode = http.GET();
		/*if (httpCode > 0) { //Check for the returning code
			String payload = http.getString();
			Serial.println(httpCode);
			Serial.println(payload);
			}else {
		Serial.println("Error on HTTPs request");
			}*/
			Serial.println(F(httpCode));
			
		http.end();
		}else{
			Serial.println("Failed to init HTTPS");
		}
	//}else{
	//  Serial.println("Failed to init WiFiClientSecure");
	//}
	//delete wcs;
#endif /* 0 */
}
