/*
 * neOCampus operation
 * 
 * User's high-level parameters management class.
 * Most of those parameters come from WiFiManager parameters
 *
 * Thiebolt F. Jun.18 initial release
 * 
 */


/*
 * Includes
 */
#include <FS.h>
#if defined(ESP32)
  #include "SPIFFS.h"
#endif

/*
 * ESP8266 advanced ops
 */
#if defined(ESP8266)
  extern "C" {
    #include "user_interface.h"           // wifi_station_dhcpc_start, ...
  }
  #include <ESP8266WiFi.h>
#elif defined(ESP32)
  #include <WiFi.h>
#endif /* ESP8266 */

/* NVS namespace @ EPS32 */
#ifdef ESP32
  #include "Preferences.h"          // NVS storage (instead of the DEPRECATED eeprom)
#elif defined (ESP8266)
  #warning "[esp8266] no NVS namespace available ... grab WiFi ssid/psk from last connection"
#endif

/* neOCampus related includes */
#include "neocampus.h"

#include "neocampus_debug.h"
#include "neocampus_utils.h"

#include "wifiParametersMgt.h"


/*
 * Global shared variables/functions
 */
extern bool _need2reboot;


/*
 * Definitions
 */
/* --- SPIFFS WiFi related definitions
 * Note: remember there's no directory support @ SPIFFS level!
 */
#define WIFI_CONFIG_FILE        "/wifi.json"        // WiFi configuration file to store credentials and locales confs

#define CONFIG_JSON_SIZE        (JSON_OBJECT_SIZE(20))

// NVS namespace for WiFi credentials
#define WIFI_NVS_NAMESPACE      "wifiCredentials"  // 15 chars max.


// constructor
wifiParametersMgt::wifiParametersMgt( void ) {
  _initialized = false;
  
  // apply default values
  _applyDefaults();
}

// destructor
wifiParametersMgt::~wifiParametersMgt( void ) {
  // free dynamically allocated memory
}



/*
 * check parameters validity
 * i.e they have been initialized at least once
 */
bool wifiParametersMgt::isValid( void ) {
  if( ! _initialized ) {
    log_debug(F("\n[wifiParams] someone queried the parameters status that is NOT yet initialized ..."));log_flush();
  }
  
  return _initialized;
}



/*
 * load parameters file
 */
bool wifiParametersMgt::loadConfigFile( void ) {

  if( ! SPIFFS.exists(WIFI_CONFIG_FILE) ) {
    log_info(F("\n[wifiParams] WiFi config file ")); log_info(WIFI_CONFIG_FILE); log_info(F(" does not exists"));
    log_info(F("\n[wifiParams] applying DEFAULTS ..."));
    log_flush();
    _applyDefaults();
    return false;
  }
  
  // open wifi config file
  File configFile = SPIFFS.open(WIFI_CONFIG_FILE, "r");
  if( ! configFile ) {
    log_error(F("\n[wifiParams] error opening WiFi config file ")); log_error(WIFI_CONFIG_FILE);
    log_error(F("\n[wifiParams] removing file and applying DEFAULTS ..."));
    log_flush();
    SPIFFS.remove(WIFI_CONFIG_FILE);
    _applyDefaults();
    return false;
  }

  // grab file content in JSON buffer
  log_debug(F("\n[wifiParams] start reading WiFi config file ...")); log_debug(WIFI_CONFIG_FILE); log_flush();
  size_t size = configFile.size();
  // Allocate a buffer to store contents of the file.
  std::unique_ptr<char[]> buf(new char[size]);
  log_debug(F("\n\treading ")); log_debug(size,DEC); log_debug(F(" bytes from config file ")); log_flush();
  configFile.readBytes(buf.get(), size);

  // JSON buffer allocation
  if( size > (CONFIG_JSON_SIZE) ) {
    log_error(F("\n[wifiParams] config file to read is too large for JsonBuffer to handle: ")); log_error(size,DEC); log_error(F(" bytes!")); log_flush();
    return false;
  }
  
  StaticJsonDocument<CONFIG_JSON_SIZE> root;

  auto err = deserializeJson( root, buf.get() );
  if( err ) {
    log_error(F("\n[wifiParams] JSON format error in WiFi config file ")); log_error(WIFI_CONFIG_FILE);
    log_error(F("\n[wifiParams] ERROR msg: ")); log_error(err.c_str());
    log_error(F("\n[wifiParams] removing file and applying DEFAULTS ..."));
    log_flush();
    SPIFFS.remove(WIFI_CONFIG_FILE);
    _applyDefaults();
    return false;
  }
#if (LOG_LEVEL >= LOG_LVL_DEBUG)
  serializeJsonPretty( root, Serial );
#endif

  // ready to parse JSON objects and to extract values
  return _loadConfig( root.as<JsonObject>() );
}



/*
 * save updated parameters to the config file
 */
bool wifiParametersMgt::saveConfigFile( void ) {
  if( !isValid() ) return false;
  
  if( !_updated ) {
    log_debug(F("\n[wifiParams] saved options from WEB portal didn't modified any of the default values for options ... thus nothing to save!")); log_flush();
    return false;
  }
  
  log_info(F("\n[wifiParams] start to SAVE wifi parameters in config file ...")); log_flush();
  
  // create JSON structure
  StaticJsonDocument<CONFIG_JSON_SIZE> _doc;
  JsonObject root = _doc.to<JsonObject>();
  
  
  // fill-in JSON structure
  _saveConfig( root );
  
  
  /*
   * save JSON to config file
   */
  if( root.size()==0 ) {
    // nothing to save ... thus we delete the config file (if any)
    log_debug(F("\n[wifiParams] empty JSON structure ... thus remove config file: "));log_debug(WIFI_CONFIG_FILE);log_flush();
    return SPIFFS.remove(WIFI_CONFIG_FILE);
  }
  
  // ok, there's something to save at least
  File configFile = SPIFFS.open( WIFI_CONFIG_FILE, "w"); // truncate if exists or create otherwise
  if( !configFile ) {
    log_error(F("\n[wifiParams] error creating file: "));log_debug(WIFI_CONFIG_FILE);log_flush();
    return false;
  }
  
  // save JSON to file
  
  if( serializeJson( root, configFile )==0 ) {
    configFile.close();
    log_error(F("\n[wifiParams] error unable to write data to file: "));log_debug(WIFI_CONFIG_FILE);log_flush();
    return false;
  }
  
  // close file
  configFile.close();
  
  log_info(F("\n[wifiParams] successfully saved JSON config file: "));log_debug(WIFI_CONFIG_FILE);log_flush();
  
  _updated = false;
  return true;
}


  
/*
 * Grab WIFI settings through Wifi global var or from parameters
 *  and set within private attributes
 */
bool wifiParametersMgt::setWIFIsettings( const char* ssid, const char* pass ) {

  const char *tmpSSID = ssid;
  const char *tmpPASS = pass;

  if( !tmpSSID ) tmpSSID = WiFi.SSID().c_str();
  log_debug(F("\n[wifiParams] retrieved current ssid = ")); log_debug(tmpSSID);

  if( !tmpPASS ) tmpPASS = WiFi.psk().c_str();
  log_debug(F("\n[wifiParams] retrieved current pass = ")); log_debug(tmpPASS);

  log_flush();
  
  if( !strlen(tmpSSID) ) {
    log_debug(F("\n[wifiParams] empty SSID ...")); log_flush();
    return false;
  }

  if( strncmp(_ssid, tmpSSID, sizeof(_ssid)) or
      strncmp(_pass, tmpPASS, sizeof(_pass)) ) {
  
    log_debug(F("\n[wifiParams] new credentials detected ... update!")); log_flush();
    strncpy( _ssid, tmpSSID, sizeof(_ssid) );
    strncpy( _pass, tmpPASS, sizeof(_pass) );
    _updated = true;
  }
  else {
    log_debug(F("\n[wifiParams] already matching wifi credentials thus nothing todo !")); log_flush();
  }

  return true;
}



/*
 * get WiFi SSID
 */
const char *wifiParametersMgt::getWIFIssid( void ) {
  if( !_initialized ) return nullptr;
  return _ssid;
}



/*
 * get WiFi PASSWORD
 */
const char *wifiParametersMgt::getWIFIpass( void ) {
  if( !_initialized ) return nullptr;
  return _pass;
}



/*
 * sensOCampus SANDBOX option
 */
bool wifiParametersMgt::_setopt_sandboxMode( bool value ) {
  if( value != _opt_sandboxMode ) {
    _opt_sandboxMode = value;
    _updated = true;
  }
  return _opt_sandboxMode;
}



/*
 * neOClock TM1637 7 segment display driver
 */
bool wifiParametersMgt::_setopt_7segTM1637( bool value ) {
  if( value != _opt_7segTM1637 ) {
    _opt_7segTM1637 = value;
    _updated = true;
  }
  return _opt_7segTM1637;
}



/*
 * PIR sensor option
 */
bool wifiParametersMgt::_setopt_PIRsensor( bool value ) {
  if( value != _opt_PIRsensor ) {
    _opt_PIRsensor = value;
    _updated = true;
  }
  return _opt_PIRsensor;
}



/*
 * Dangerous: Erase ALL option
 */
bool wifiParametersMgt::_setopt_eraseALL( bool value ) {
    _opt_eraseALL = value;
    return _opt_eraseALL;
}



/* ------------------------------------------------------------------------------
 * Private methods 
 */



/*
 * apply DEFAULTS values
 * Note: options ought to get false as defaults
 */
void wifiParametersMgt::_applyDefaults( void ) {

  // will get set to true once we'll have options set though our captive WEB portal
  _updated = false;
  
  // WiFi settings
  _ssid[0] = '\0';
  _pass[0] = '\0';
  
  // as default, senSOcampus sandbox mode is DISABLED
  _opt_sandboxMode = false;
  
  // TM1637 7segment based clock
  _opt_7segTM1637 = false;

  // PIR sensor set as an option because it can't get detected
  _opt_PIRsensor = false;
  
  
  
  
  // Whole destruction option
  _opt_eraseALL = false;
  
  /*
   * [may.23] now trying to access wifi credentias NVS namespace
   */
  Preferences nvs_area;
  if( nvs_area.begin(WIFI_NVS_NAMESPACE,true) ) {  // readonly mode
    log_debug(F("\n[wifiParams] opened NVS WiFi credentials namespace ..."));log_flush();

    // TO BE CONTINUED

  }

  /*
   * finally structure is initialized
   */
  _initialized = true;
}



/*
 * fill JSON structure with things to save
 */
bool wifiParametersMgt::_saveConfig( JsonObject root ) {
  
  // save WiFi parameters
  if( strlen(_ssid) ) {
    root["ssid"] = _ssid;
    root["pass"] = _pass;
  }
  else {
    log_debug(F("\n[wifiParams] no WIFI credentials to save ..."));log_flush();
  }
  
  // save options ?
  if( _updated ) {
    
    root["opt_sandboxMode"]   = _opt_sandboxMode;
    root["opt_7segTM1637"]    = _opt_7segTM1637;
    root["opt_PIRsensor"]     = _opt_PIRsensor;

    /*
    * add additional options here
    */
  }
  else {
    log_debug(F("\n[wifiParams] no change in options to save ..."));log_flush();
  }
  
  return true;
}



/*
 * parse JSON objects that contain config
 */
bool wifiParametersMgt::_loadConfig( JsonObject root ) {
  log_debug(F("\n[wifiParams] start parsing JSON config ..."));

  bool _wifiSet = false;

/*
// using C++98 syntax (for older compilers):
for (JsonObject::iterator it=root.begin(); it!=root.end(); ++it) {
    Serial.println(it->key().c_str());
    Serial.println(it->value().as<char*>());
}
*/

  // iterate over all [key,values] pairs
  for (JsonObject::iterator it=root.begin(); it!=root.end(); ++it) {

    {
      const char *_key = PSTR("ssid");
      if( strncmp_P(it->key().c_str(), _key, strlen_P(_key))==0 ) {
        // SSID is present :)
        if( it->value().is<const char*>() ) {
          _wifiSet = true;
          strncpy( _ssid, it->value(), sizeof(_ssid) );
          log_debug(F("\n[wifiParams][config_file] ssid = ")); log_debug(_ssid); log_flush();
        }
        else {
          log_error(F("\n[wifiParams] wifi_ssid is not of const char* type ?!?!")); log_flush();
        }
        continue;
      }
    }
    
    {
      const char *_key = PSTR("pass");
      if( strncmp_P(it->key().c_str(), _key, strlen_P(_key))==0 ) {
        // PASS is present :)
        if( it->value().is<const char*>() ){
          _wifiSet = true;
          strncpy( _pass, it->value(), sizeof(_pass) );
          log_debug(F("\n[wifiParams][config_file] pass = ")); log_debug(_pass); log_flush();
        }
        else {
          log_error(F("\n[wifiParams] wifi_pass is not of const char* type ?!?!")); log_flush();
        }
        continue;
      }
    }

    {
      const char *_key = PSTR("opt_sandboxMode");
      if( strncmp_P(it->key().c_str(), _key, strlen_P(_key))==0 ) {
        // sensOCampus sandbox mode
        if( it->value().is<bool>() ) {
          _opt_sandboxMode = it->value();
          log_debug(F("\n[wifiParams][config_file] sandboxMode = ")); log_debug(_opt_sandboxMode,DEC); log_flush();
        }
        else {
          log_error(F("\n[wifiParams] opt_sandboxMode is not a bool ?!?!")); log_flush();
        }
        continue;
      }
    }

    {
      const char *_key = PSTR("opt_7segTM1637");
      if( strncmp_P(it->key().c_str(), _key, strlen_P(_key))==0 ) {
        // TM1737 based 7 segment display
        if( it->value().is<bool>() ) {
          _opt_7segTM1637 = it->value();
          log_debug(F("\n[wifiParams][config_file] 7segTM1637 = ")); log_debug(_opt_7segTM1637,DEC); log_flush();
        }
        else {
          log_error(F("\n[wifiParams] opt_7segTM1637 is not a bool ?!?!")); log_flush();
        }
        continue;
      }
    }

        {
      const char *_key = PSTR("opt_PIRsensor");
      if( strncmp_P(it->key().c_str(), _key, strlen_P(_key))==0 ) {
        // PIR sensor
        if( it->value().is<bool>() ) {
          _opt_PIRsensor = it->value();
          log_debug(F("\n[wifiParams][config_file] PIR sensor = ")); log_debug(_opt_PIRsensor,DEC); log_flush();
        }
        else {
          log_error(F("\n[wifiParams] opt_PIRsensor is not a bool ?!?!")); log_flush();
        }
        continue;
      }
    }

    // unknown key ?!?!
    log_error(F("\n[wifiParams] unknown key: "));log_debug((const char*)(it->key().c_str())); log_flush();
  }

  /*
   * check if wifi connexion parameters have been read ...
   * ... otherwise we'll extract them from struct station
   */
  if( _wifiSet ) return true;
  
  // grab WiFi from previous settings (Wifi global var)
  setWIFIsettings();
  
  // over :)
  return true;
}

