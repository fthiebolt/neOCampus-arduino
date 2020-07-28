#ifndef WIFIMANAGER_HPP
#define WIFIMANAGER_HPP

#include <Arduino.h>
#include <WiFiManager.h>
#include "neologger.hpp"

class wifimanager{
public:
    wifimanager() {};
    
    inline char* setup(){  
        log_debug("--- beg of wifimanager::setup ---")
        _wm.resetSettings();
        Serial.print("ESP BOARD MAC Address: ");
        WiFi.macAddress(_macAddr);
        snprintf( _strMacAddr, sizeof(_strMacAddr), "%02X:%02X:%02X:%02X:%02X:%02X", _macAddr[0],_macAddr[1],_macAddr[2],_macAddr[3],_macAddr[4],_macAddr[5]);
        snprintf(_ssid, sizeof(_ssid),"%s%02X:%02X","airquality_",_macAddr[4], _macAddr[5]);
        log_debug("--- beg of wm ---")
        log_debug(_pwd);
        log_debug(_ssid);
        WiFi.mode(WIFI_STA);
        if(!_wm.autoConnect(_ssid,_pwd))
            Serial.println("Conn Doomed");
        else{
            Serial.println("Conn est");
        }
        log_debug("--- end of wifimanager::setup---\n");
        return _strMacAddr;
    }
private:
    WiFiManager _wm;
    char _ssid[20];
    const char* _pwd = "password"; 
    uint8_t _macAddr[6];
    char _strMacAddr[32];
};

#endif
