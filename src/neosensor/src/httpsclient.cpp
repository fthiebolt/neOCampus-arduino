#include "httpsclient.hpp"

httpsclient::httpsclient () {};
StaticJsonDocument<CRED_JSON_SIZE> httpsclient::get_credentials(char* str_mac_addr){
    log_debug("--- beg of httpsclient::get_credentials ---");
    StaticJsonDocument<CRED_JSON_SIZE> cred_json;
    snprintf(_url, 90, "%s%s%s", AUTH_SERVER, CRED_REQ,str_mac_addr);
    log_debug(_url);
    String payload;
    _client.begin(_url,root_ca);
    int httpCode = _client.GET();
    if (httpCode > 0) { //Check for the returning code
        payload = _client.getString();
    }
    /* Unboxing credentials from cred server */
    DeserializationError error = deserializeJson(cred_json, payload);
    Serial.println("Credentials requested from auth");
    _client.end();
    // Test if parsing succeeds.
    if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.c_str());
    }else{
        log_debug("--- beg of httpsclient::get_credentials ---");
        return cred_json;
    }
}