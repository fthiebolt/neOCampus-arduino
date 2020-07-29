#include "httpsclient.hpp"

httpsclient::httpsclient () {};

StaticJsonDocument<CRED_JSON_SIZE> httpsclient::get_credentials(uint8_t *mac_addr){
    log_debug("--- beg of httpsclient::get_credentials ---");
    WiFiClientSecure *wcsClient = new WiFiClientSecure;
    wcsClient->setCACert(_root_ca);

    //snprintf(_url, 90, "%s%s", AUTH_SERVER, CRED_REQ);
    //log_debug(_url)
    //snprintf(_url, 90, "%s%02X:%02X:%02X:%02X:%02X:%02X", _url, mac_addr[0],mac_addr[1],mac_addr[2],mac_addr[3],mac_addr[4],mac_addr[5]);
    //log_debug(_url);
    if(!_https.begin(*wcsClient, "https://sensocampus.univ-tlse3.fr/device/credentials?mac=24:0A:C4:02:8B:60")){
        log_error("Beginning of https communication failed" );
    }else{
        log_debug("https communication established")
    }
    log_debug("where's the core dump at?");
    int httpCode= _https.GET();
    delay(5000);
    String payload;
    if (httpCode > 0) { 
        //Check for the returning code
        payload = _https.getString();
    }
    /* Unboxing credentials from cred server */
    StaticJsonDocument<CRED_JSON_SIZE> cred_json;
    DeserializationError error = deserializeJson(cred_json, payload);
    Serial.println("Credentials requested from auth");
    log_debug(_url);
    _https.end();
    delete wcsClient;
    // Test if parsing succeeds.
    if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.c_str());
    }else{
        log_debug("--- beg of httpsclient::get_credentials ---");
    }
    return cred_json;
}