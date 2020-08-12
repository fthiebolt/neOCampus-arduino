/*
 * neOCampus operation
 * 
 * sensOCampus client class for interactions with sensOCampus server
 *
 * ---
 * F.Thiebolt  aug.20   removed EEPROM support
 *                      added SPIFFS support to load/save config file
 * Thiebolt F. Nov.19   migrate to Arduino Json 6
 * Thiebolt F. July 17
 * 
 */



/*
 * Includes
 */
#include <Arduino.h>
#include <ArduinoJson.h>

#include "sensocampus.h"

#include "neocampus_utils.h"


/*
 * Definitions
 */
/* --- SPIFFS related definitions
 * Note: remember there's no directory support @ SPIFFS level!
 */
#define CONFIG_FILE             "/sensocampus.json"     // senOCampus config file contains credentials and access to MQTT(s)
                                                        // note that config of various modules is NOT saved
#define CONFIG_JSON_SIZE        (JSON_OBJECT_SIZE(20))




/*
 * Class constructor
 */
senso::senso( void ) {
  _initialized = false;
  // set default values
  _applyDefaults();
  // no WiFi option provided (we check against sandbox mode)
  _wp = nullptr;
}

senso::senso( wifiParametersMgt *p ) {
  _initialized = false;
  // set default values
  _applyDefaults();
  // register link to global wifi parameters & options (to check against sandbox mode)
  _wp = p;
}


/*
 * check parameters validity
 * i.e they have been initialized at least once
 */
bool senso::isValid( void ) {
  if( ! _initialized ) {
    log_debug(F("\n[senso] someone queried the parameters status that is NOT yet initialized ..."));log_flush();
  }
  
  return _initialized;
}


/*
 * load parameters file
 */
bool senso::loadConfigFile( void ) {

  if( ! SPIFFS.exists(CONFIG_FILE) ) {
    log_info(F("\n[senso] sensOCampus config file ")); log_info(CONFIG_FILE); log_info(F(" does not exists"));
    log_info(F("\n[senso] applying DEFAULTS ..."));
    log_flush();
    _applyDefaults();
    return false;
  }
  
  // open wifi config file
  File configFile = SPIFFS.open(CONFIG_FILE, "r");
  if( ! configFile ) {
    log_error(F("\n[senso] error opening config file ")); log_error(CONFIG_FILE);
    log_error(F("\n[senso] removing file and applying DEFAULTS ..."));
    log_flush();
    SPIFFS.remove(CONFIG_FILE);
    _applyDefaults();
    return false;
  }

  // grab file content in JSON buffer
  log_debug(F("\n[senso] start reading config file ")); log_debug(CONFIG_FILE); log_flush();
  size_t size = configFile.size();
  // Allocate a buffer to store contents of the file.
  std::unique_ptr<char[]> buf(new char[size]);
  log_debug(F("\n\treading ")); log_debug(size,DEC); log_debug(F(" bytes from config file ")); log_flush();
  configFile.readBytes(buf.get(), size);

  // JSON buffer allocation
  if( size > (CONFIG_JSON_SIZE) ) {
    log_error(F("\n[senso] config file to read is too large for JsonBuffer to handle: ")); log_error(size,DEC); log_error(F(" bytes!")); log_flush();
    return false;
  }
  
  StaticJsonDocument<CONFIG_JSON_SIZE> root;

  auto err = deserializeJson( root, buf.get() );
  if( err ) {
    log_error(F("\n[senso] JSON format error in config file ")); log_error(CONFIG_FILE);
    log_error(F("\n[senso] ERROR msg: ")); log_error(err.c_str());
    log_error(F("\n[senso] removing file and applying DEFAULTS ..."));
    log_flush();
    SPIFFS.remove(CONFIG_FILE);
    _applyDefaults();
    return false;
  }
#if (LOG_LEVEL >= LOG_LVL_DEBUG)
  serializeJsonPretty( root, Serial );
#endif

  // ready to parse JSON objects and to extract values
  if( !_loadConfig(root.as<JsonObject>()) ) {
    log_warning(F("\n[senso] missing parameters in config file ... apply defaults")); log_flush();
    _applyDefaults();
    return false;
  }

  /* Note: we don't set the _initialized flag because those
   * retrieved credentials may be out-of-date ... hence the
   * need for the call to senso->begin( ... )
   */
  return true;
}


/*
 * save updated parameters to the config file
 */
bool senso::saveConfigFile( void ) {
  if( !isValid() ) return false;
  
  if( !_updated ) {
    log_debug(F("\n[senso] config has not been changed since last save ... thus nothing to save!")); log_flush();
    return false;
  }
  
  log_info(F("\n[senso] start to SAVE sensOCampus parameters in config file ...")); log_flush();
  
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
    log_debug(F("\n[senso] empty JSON structure ... thus remove config file: "));log_debug(CONFIG_FILE);log_flush();
    return SPIFFS.remove(CONFIG_FILE);
  }
  
  // ok, there's something to save at least
  File configFile = SPIFFS.open( CONFIG_FILE, "w"); // truncate if exists or create otherwise
  if( !configFile ) {
    log_error(F("\n[senso] error creating file: "));log_debug(CONFIG_FILE);log_flush();
    return false;
  }
  
  // save JSON to file
  
  if( serializeJson( root, configFile )==0 ) {
    configFile.close();
    log_error(F("\n[senso] error unable to write data to file: "));log_debug(CONFIG_FILE);log_flush();
    return false;
  }
  
  // close file
  configFile.close();
  
  log_info(F("\n[senso] successfully saved JSON config file: "));log_debug(CONFIG_FILE);log_flush();
  
  // all parameters saved --> hence no more updated fields
  _updated = false;

  return true;
}


/*
 * Begin ... means populate everything from HTTP(s) server sensocampus
 */
boolean senso::begin( const char *mac ) {
  log_info(F("\n[sensOCampus] start with provided mac addr ")); log_info( mac); log_flush();

  // check global parameters
  if( _wp and _wp->isValid() and _wp->isEnabledSandbox()== true ) {
    /*
     * sensOCampus is NOT enabled
     * ==> using SANDBOX mode
     * - we don't modify CONFIG_FILE
     * - we consider our config as valid (while being defaults at the same time)
     */
    log_debug(F("\n[senso] >>> DISABLED: using sensOCampus SANDBOX mode <<<")); log_flush();
    _applyDefaults();
    _mqtt_port = DEFL_MQTT_ABROAD_PORT;   // probably because disabled mode means abroad ;)
    delay(1000);
  }
  else {
    /*
     * sensOCampus normal operation
     */
    // get device CREDENTIALS
    if( http_getCredentials( mac ) != true ) {
      log_error(F("\n[senso] error getting CREDENTIALS from sensOCampus server ?!")); log_flush();
      _applyDefaults();
      return false;
    }
    // ok, credentials properly grabbed --> no more defaults
    _defaults = false;
    
    // get device CONFIG
    if( http_getConfig( ) != true ) {
      log_error(F("\n[senso] error getting CONFIG from sensOCampus server ?!")); log_flush();
      _applyDefaults();
      return false;
    }
    // since we don't save modules config, we don't set the _defaults flag to false
  }

  // both CREDENTIALS & CONFIG done :)
  _initialized = true;
  
  return true;
}


TO BE CONTINUED


// obtain CREDENTIALS from sensOCampus server
bool senso::http_getCredentials( const char *mac ) {
  log_debug(F("\n[sensOCampus] start http getCredentials ... ")); log_flush();

  // get JSON response to url credentials http get
  char url[SENSO_HTTP_URL_MAXSIZE];
  char buf[SENSO_HTTP_MAX_RESPONSE_SIZE];
  snprintf(url, sizeof(url),"%s%s", SENSO_GET_CREDENTIALS_URL, mac );
  if( !http_get( url, buf, sizeof(buf) ) ) {
    return false;
  }
  log_debug(F("\n[senso] GET response <"));log_debug(buf);log_debug(F(">"));log_flush();

  // [JSON] deserialize credentials
  if( !_parseCredentials(buf) )
    return false;

  return true;
}


// obtain CONFIG from sensOCampus server
bool senso::http_getConfig( void ) {
  log_debug(F("\n[sensOCampus] start http getConfig ... ")); log_flush();

  // get JSON response to url config http get
  char url[SENSO_HTTP_URL_MAXSIZE];
  char buf[SENSO_HTTP_MAX_RESPONSE_SIZE];
  snprintf(url, sizeof(url),"%s", SENSO_GET_CONFIG_URL );
  if( !http_get( url, buf, sizeof(buf), _mqtt_login, _mqtt_passwd ) ) {
    return false;
  }
  log_debug(F("\n[senso] GET response <"));log_debug(buf);log_debug(F(">"));log_flush();

  // [JSON] deserialize config
  if( !_parseConfig(buf) )
    return false;

  return true;
}


END


/*
 * --- Private methods -----------------------------------------------------------------------
 */

// apply defaults to sensOCampus parameters
void senso::_applyDefaults( void ) {  
  // Note: snprintf automatically add NULL terminaison
  snprintf(_mqtt_server, sizeof(_mqtt_server), DEFL_MQTT_SERVER);
  _mqtt_port = DEFL_MQTT_STD_PORT;
  snprintf(_mqtt_login, sizeof(_mqtt_login), DEFL_MQTT_LOGIN);
  snprintf(_mqtt_passwd, sizeof(_mqtt_passwd), DEFL_MQTT_PASSWD);
  snprintf(_mqtt_base_topic, sizeof(_mqtt_base_topic), "%s/%s", DEFL_MQTT_TOPIC, getAPname());

  _defaults = true;
  _updated = false;
}


/*
 * fill JSON structure with things to save
 */
bool senso::_saveConfig( JsonObject root ) {
  
  if( _defaults ) {
    log_debug(F("\n[senso] NO NEED to save defaults parameters !")); log_flush();
    return false;
  }

  // parameters have been updated ?
  if( _updated ) {
    
    root["mqtt_server"]       = _mqtt_server;
    root["mqtt_port"]         = _mqtt_port;
    root["mqtt_login"]        = _mqtt_login;
    root["mqtt_passwd"]       = _mqtt_passwd;
    root["mqtt_base_topic"]   = _mqtt_base_topic;

    /*
     * add additional parameters to save here
     */
  }
  else {
    log_debug(F("\n[senso] no change in options to save ..."));log_flush();
  }
  
  // note: _updated flag will get set to false in caller
  return true;
}


/*
 * parse JSON objects that contain config
 */
bool senso::_loadConfig( JsonObject root ) {
  log_debug(F("\n[senso] start parsing JSON config ..."));

  bool _loginSet = false;
  bool _passwdSet = false;
  bool _baseTopicSet = false;

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
      const char *_key = PSTR("mqtt_server");
      if( strncmp_P(it->key().c_str(), _key, strlen_P(_key))==0 ) {
        // MQTT_SERVER is present :)
        if( it->value().is<const char*>() ) {
          strncpy( _mqtt_server, it->value(), sizeof(_mqtt_server) );
          log_debug(F("\n[senso][config_file] mqtt_server = ")); log_debug(_mqtt_server); log_flush();
        }
        else {
          log_error(F("\n[senso] mqtt_server is not of const char* type ?!?!")); log_flush();
        }
        continue;
      }
    }

    {
      const char *_key = PSTR("mqtt_port");
      if( strncmp_P(it->key().c_str(), _key, strlen_P(_key))==0 ) {
        // MQTT_PORT is present :)
        if( it->value().is<int>() ) {
          _mqtt_port = (uint16_t)it->value();
          log_debug(F("\n[senso][config_file] mqtt_port = ")); log_debug(_mqtt_port,DEC); log_flush();
        }
        else {
          log_error(F("\n[senso] mqtt_port is not of integer type ?!?!")); log_flush();
        }
        continue;
      }
    }

    {
      const char *_key = PSTR("mqtt_login");
      if( strncmp_P(it->key().c_str(), _key, strlen_P(_key))==0 ) {
        // MQTT_LOGIN is present :)
        if( it->value().is<const char*>() ) {
          _loginSet = true;
          strncpy( _mqtt_login, it->value(), sizeof(_mqtt_login) );
          log_debug(F("\n[senso][config_file] mqtt_login = ")); log_debug(_mqtt_login); log_flush();
        }
        else {
          log_error(F("\n[senso] mqtt_login is not of const char* type ?!?!")); log_flush();
        }
        continue;
      }
    }

    {
      const char *_key = PSTR("mqtt_passwd");
      if( strncmp_P(it->key().c_str(), _key, strlen_P(_key))==0 ) {
        // MQTT_PASSWD is present :)
        if( it->value().is<const char*>() ) {
          _passwdSet = true;
          strncpy( _mqtt_passwd, it->value(), sizeof(_mqtt_passwd) );
          log_debug(F("\n[senso][config_file] mqtt_passwd = ")); log_debug(_mqtt_passwd); log_flush();
        }
        else {
          log_error(F("\n[senso] mqtt_passwd is not of const char* type ?!?!")); log_flush();
        }
        continue;
      }
    }

    {
      const char *_key = PSTR("mqtt_base_topic");
      if( strncmp_P(it->key().c_str(), _key, strlen_P(_key))==0 ) {
        // MQTT_BASE_TOPIC is present :)
        if( it->value().is<const char*>() ) {
          _baseTopicSet = true;
          strncpy( _mqtt_base_topic, it->value(), sizeof(_mqtt_base_topic) );
          log_debug(F("\n[senso][config_file] mqtt_base_topic = ")); log_debug(_mqtt_base_topic); log_flush();
        }
        else {
          log_error(F("\n[senso] mqtt_base_topic is not of const char* type ?!?!")); log_flush();
        }
        continue;
      }
    }

    // unknown key ?!?!
    log_error(F("\n[senso] unknown key: "));log_debug((const char*)(it->key().c_str())); log_flush();
  }
  
  // over :)
  return ( _loginSet==true and _passwdSet==true and _baseTopicSet==true );
}



TO BE CONTINUED
- ne pas oublier de positionner le flag _updated !



// parse JSON buffer that contains credentials
bool senso::_parseCredentials( char *json ) {
  log_debug(F("\n[senso] start parsing JSON credentials ..."));

  StaticJsonDocument<SENSO_JSON_SIZE> root;
  
  auto err = deserializeJson( root, json );
  if( err ) {
    log_error(F("\n[senso] ERROR parsing JSON credentials :"));log_error(err.c_str()); log_flush();
    return false;
  }

  // check for "server" (i.e MQTT broker) in JSON buffer
  if( (root.containsKey(F("server"))==true) ) {
    snprintf(_mqtt_server, sizeof(_mqtt_server),"%s", (const char *)(root[F("server")]) );
    log_info(F("\n[senso] found 'server' = "));log_info(_mqtt_server); log_flush();
  }
  
  // check for "port" (i.e MQTT broker port) in JSON buffer
  if( (root.containsKey(F("port"))==true) ) {
    _mqtt_port = (uint16_t)root[F("port")];
    log_info(F("\n[senso] found 'port' = "));log_info(_mqtt_port,DEC); log_flush();
  }

  // check for "login" in JSON buffer
  if( (root.containsKey(F("login"))==false) ) {
    log_error(F("\n[senso] JSON credentials without 'login' ?!?!"));
    return false;
  }
  /* ok, we have a 'login' field ...
   * ... if we have a password field we save both
   */
  // check for password in JSON
  if( (root.containsKey(F("password"))==true) ) {
    snprintf(_mqtt_login,sizeof(_mqtt_login),"%s", (const char *)(root[F("login")]) );
    snprintf(_mqtt_passwd,sizeof(_mqtt_passwd),"%s", (const char *)(root[F("password")]) );
    log_info(F("\n[senso] stored login & password :)"));
    return true;
  }


TODO: we need to save sensOCampus credentials because passwd is only sent on first attempt!!


  /*
   * So far, we have a login field but without a password field ...
   * ... thus it means that this later ought to get stored within
   * EEPROM along with corresponding login in EEPROM too
   */
  // retrieve login from EEPROM
  if( !getEEPROMlogin( _mqtt_login, sizeof(_mqtt_login) ) ) {
    log_error(F("\n[senso] login not found in EEPROM ?!?!")); log_flush();
    return false;
  }
  size_t len1=strlen(_mqtt_login);
  size_t len2=strlen(root[F("login")]);
  if( (len1!=len2) or strncmp(_mqtt_login, root[F("login")], max(len1,len2)) ) {
    log_error(F("\n[senso] error login does not match the one stored in EEPROM ?!")); log_flush();
    return false;
  }

  /*
   * ok, JSON credentials provided login is the same as the one within EEPROM ...
   * ... now retrieve saved password from EEPROM
   */
  if( !getEEPROMpasswd( _mqtt_passwd, sizeof(_mqtt_passwd) ) ) {
    log_error(F("\n[senso] error passwd not found in EEPROM !!")); log_flush();
    return false;
  }

  // success :)
  return true;
}


// parse JSON buffer that contains config
bool senso::_parseConfig( char *json ) {
  log_debug(F("\n[senso] start parsing JSON config ..."));

  StaticJsonDocument<SENSO_JSON_SIZE> root;

  auto err = deserializeJson( root, json );
  if( err ) {
    log_error(F("\n[senso] ERROR parsing JSON config: "));log_error(err.c_str()); log_flush();
    return false;
  }

  // check for "topic" in JSON buffer
  if( (root.containsKey(F("topics"))==false) ) {
    log_error(F("\n[senso] JSON config without base 'topic' ?!?!"));
    return false;
  }
  log_info(F("\n[senso] WARNING: only 1st topic taken into account!"));
  //strcpy(_mqtt_base_topic,root[F("topics")][0]);
  snprintf( _mqtt_base_topic, sizeof(_mqtt_base_topic), "%s", (const char *)(root[F("topics")][0]) );
  log_info(F("\n[senso] base TOPIC = ")); log_debug(_mqtt_base_topic); log_flush();

  // check for server (MQTT)
  if( (root.containsKey(F("server"))==true) ) {
    snprintf( _mqtt_server, sizeof(_mqtt_server), "%s", (const char *)root[F("server")] );
    log_info(F("\n[senso] set MQTT server to ")); log_debug(_mqtt_server); log_flush();  
  }

  // check for server (MQTT)
  if( (root.containsKey(F("server"))==true) ) {
    snprintf( _mqtt_server, sizeof(_mqtt_server), "%s", (const char *)root[F("server")] );
    log_info(F("\n[senso] set MQTT server to ")); log_debug(_mqtt_server); log_flush();  
  }

  // check for server port (MQTT)
  if( (root.containsKey(F("port"))==true) ) {
    _mqtt_port = (uint16_t)root[F("port")];
    log_info(F("\n[senso] set MQTT port to ")); log_debug(_mqtt_port,DEC); log_flush();  
  }

  // parse additional parameters here :)

  return true;
}


// Send back server name
const char * senso::getServer( void ) const {
  return _mqtt_server;
}

// Send back port number
uint16_t senso::getServerPort( void ) const {
  return _mqtt_port;
}

// Send back BaseTopic
const char *senso::getBaseTopic( void ) const {
  return _mqtt_base_topic;
}

// Send back MQTT username
const char *senso::getUser( void ) const {
  return _mqtt_login;
}

// Send back MQTT password
const char *senso::getPassword( void ) const {
  return _mqtt_passwd;
}

