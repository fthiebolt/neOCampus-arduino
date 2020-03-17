/*
 * neOCampus operation
 * 
 * sensOCampus client class for interactions with sensOCampus server
 * 
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

#include "neocampus_eeprom.h"
#include "neocampus_utils.h"



/*
 * Class constructor
 */
senso::senso( void ) {
  _initialized = false;
  // set default values
  _applyDefaults();
}

senso::senso( wifiParametersMgt *p ) {
  _initialized = false;
  // set default values
  _applyDefaults();
  // register link to global wifi parameters & options
  _wifiParams = p;
}


/*
 * Begin ... means populate everything from HTTP server sensocampus
 * till the EEPROM
 */
boolean senso::begin( const char *mac ) {
  log_info(F("\n[sensOCampus] start with provided mac addr ")); log_info( mac); log_flush();

  // check global parameters
  if( _wifiParams and _wifiParams->isValid() and _wifiParams->isEnabledSandbox()== true ) {
    /*
     * sensOCampus is NOT enabled
     * ==> using SANDBOX mode
     */
    log_debug(F("\n[senso] >>> DISABLED: using sensOCampus SANDBOX mode <<<")); log_flush();
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
    
    // get device CONFIG
    if( http_getConfig( ) != true ) {
      log_error(F("\n[senso] error getting CONFIG from sensOCampus server ?!")); log_flush();
      _applyDefaults();
      return false;
    }
  }

  // both CREDENTIALS & CONFIG done :)
  _initialized = true;
  
  return true;
}



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



/*
 * --- Private methods -----------------------------------------------------------------------
 */

// apply defaults to sensOCampus parameters
void senso::_applyDefaults( void ) {
  _wifiParams = nullptr;
  
  // Note: snprintf automatically add NULL terminaison
  snprintf(_mqtt_server, sizeof(_mqtt_server), DEFL_MQTT_SERVER);
  _mqtt_port = DEFL_MQTT_STD_PORT;
  snprintf(_mqtt_login, sizeof(_mqtt_login), DEFL_MQTT_LOGIN);
  snprintf(_mqtt_passwd, sizeof(_mqtt_passwd), DEFL_MQTT_PASSWD);
  snprintf(_mqtt_base_topic, sizeof(_mqtt_base_topic), "%s/%s", DEFL_MQTT_TOPIC, getAPname());  
}

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
   * ... if we have a password field we apply both to senso & EEPROM
   */
  // check for password in JSON
  if( (root.containsKey(F("password"))==true) ) {
    snprintf(_mqtt_login,sizeof(_mqtt_login),"%s", (const char *)(root[F("login")]) );
    setEEPROMlogin( _mqtt_login );
    snprintf(_mqtt_passwd,sizeof(_mqtt_passwd),"%s", (const char *)(root[F("password")]) );
    setEEPROMpasswd( _mqtt_passwd );
    log_info(F("\n[senso] eeprom stored login & password :)"));
    return true;
  }

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

