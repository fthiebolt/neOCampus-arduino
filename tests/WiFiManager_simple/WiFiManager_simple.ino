#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager

/* Global variables */
const char *mySSID="neOCampus";
const char *mySSIDpw="CHANGE_ME";

void setup() {
  delay(3000);  // time for USB serial link to come up anew
  Serial.begin(115200); // Start serial for output

  Serial.println(F("\n\n###\n### Starting simple WiFiManager connection test ...\n###"));

  // Arduino libs v2.4.1, to enable printf and debug messages output
  Serial.setDebugOutput( true );

  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
  // it is a good practice to make sure your code sets wifi mode how you want it.

   
  //WiFiManager, Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wm;
  wm.setDebugOutput(true);

    // reset settings - wipe stored credentials for testing
    // these are stored by the esp library
    // wm.resetSettings();

    // Automatically connect using saved credentials,
    // if connection fails, it starts an access point with the specified name ( "AutoConnectAP"),
    // if empty will auto generate SSID, if password is blank it will be anonymous AP (wm.autoConnect())
    // then goes into a blocking loop awaiting configuration and will return success result

  Serial.println(F("\tconnection timeout to 5mn ...");
  wm.setConnectTimeout(300);

    bool res;
    // res = wm.autoConnect(); // auto generated AP name from chipid
    // res = wm.autoConnect("AutoConnectAP"); // anonymous ap
    res = wm.autoConnect(mySSID,mySSIDpw); // password protected ap

    if(!res) {
        Serial.println("Failed to connect");
        // ESP.restart();
    } 
    else {
        //if you get here you have connected to the WiFi    
        Serial.println("connected...yeey :)");
    }

}

void loop() {
    // put your main code here, to run repeatedly:
    Serial.print("+"); delay(1000);   
}
