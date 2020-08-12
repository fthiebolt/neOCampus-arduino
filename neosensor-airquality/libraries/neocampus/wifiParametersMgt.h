/*
 * neOCampus operation
 * 
 * User's high-level parameters management class.
 * Most of those parameters come from WiFiManager parameters
 *
 * Thiebolt F. Jun.18 initial release
 * 
 */


#ifndef _WIFI_PARAMETERS_MGT_H_
#define _WIFI_PARAMETERS_MGT_H_

/*
 * Includes
 */
#include <Arduino.h>
#include <ArduinoJson.h>

#include "neocampus.h"




/*
 * Definitions
 */



/*
 * Class
 */
class wifiParametersMgt {
  public:
    wifiParametersMgt( void );
    ~wifiParametersMgt( void );

    // methods
    bool isValid( void );               // tell parameters have been initialized at least once
    bool loadConfigFile( void );
    bool saveConfigFile( void );
    
    const char * getWIFIssid( void );
    const char * getWIFIpass( void );
    bool _getWIFIsettings( void );       // read struct station to extract WIFI parameters
    
    // wrapper for options querries
    bool isEnabledSandbox( void ) { return _opt_sandboxMode; };       // tell if neOCampus sandbox apply or if sensocampus is enabled (default)
    bool _setopt_sandboxMode( bool );   // TODO: find a better way :|
    
    bool isEnabled7segTM1637( void ) { return _opt_7segTM1637; };     // tell if 7-segments TM1637 driver has been activated (neOClock)
    bool _setopt_7segTM1637( bool );    // TODO: find a better way :|
    
    bool isEnabledPIR( void ) { return _opt_PIRsensor; };             // tell if infrared detector has been activated
    bool _setopt_PIRsensor( bool );     // TODO: find a better way :|



    bool isEnabledEraseALL( void ) { return _opt_eraseALL; };        // tell sensor to clear everything (wifi, eeprom and spiffs)
    bool _setopt_eraseALL( bool );     // TODO: find a better way :|

  private:

    bool _initialized;
    bool _updated;                    // did WEB options have been saved ?

    char _ssid[32];
    char _pass[64];                   // for WEP keys ?? (yes ?)
    
    bool _opt_sandboxMode;            // sensOCampus NORMAL / SANDBOX mode
    bool _opt_7segTM1637;             // TM1737 7segment display available
    bool _opt_PIRsensor;              // PIR sensor available
    
    bool _opt_eraseALL;               // flag for whole destruction of WiFi credentials, eeprom, spiffs
    
    /*
     * private member functions
     */
    void _applyDefaults( void );
    bool _loadConfig( JsonObject );       // interprets JSON buffer from config file
    bool _saveConfig( JsonObject );       // fill JSON buffer with things to save to config file
};


#endif /* _WIFI_PARAMETERS_MGT_H_ */


