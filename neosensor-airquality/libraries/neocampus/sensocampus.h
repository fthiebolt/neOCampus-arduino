/*
 * neOCampus operation
 * 
 * sensOCampus client class for interactions with sensOCampus server
 *
 * ---
 * F.Thiebolt   aug.20  switched to httpS and make use of filesystem to store things
 * Thiebolt F. July 17
 * 
 */


#ifndef _SENSOCAMPUS_H_
#define _SENSOCAMPUS_H_

/*
 * Includes
 */

#include <Arduino.h>
#include <ArduinoJson.h>

#include "neocampus.h"
#include "neocampus_debug.h"

#include "wifiParametersMgt.h"



/*
 * Definitions
 */
#ifdef DISABLE_SSL
  #define SENSO_GET_CREDENTIALS_URL       "http://sensocampus.univ-tlse3.fr/device/credentials?mac="
  #define SENSO_GET_CONFIG_URL            "http://sensocampus.univ-tlse3.fr/device/config"
#else
  #define SENSO_GET_CREDENTIALS_URL       "https://sensocampus.univ-tlse3.fr/device/credentials?mac="
  #define SENSO_GET_CONFIG_URL            "https://sensocampus.univ-tlse3.fr/device/config"
#endif /* DISABLE_SSL */
#define SENSO_UNITID_MAXSIZE            24    // unitID field maxsize (mac addr is 17 bytes)
#define SENSO_SUBID_MAXSIZE             16    // subID field maxsize (mac addr is 17 bytes)
#define SENSO_HTTP_URL_MAXSIZE          128   // maximum size of a URL sent to sensOCampus
#define SENSO_HTTP_MAX_RESPONSE_SIZE    1024  // maximum size of a response sent from sensOCampus
#define SENSO_JSON_SIZE                 (JSON_OBJECT_SIZE(256))   // max number of objects in any sensOCampus JSON response



/*
 * Class
 */
class senso {
  public:
    senso( void );
    senso( wifiParametersMgt * );
    
    bool isValid( void );               // tell parameters have been initialized at least once
    bool loadConfigFile( void );
    bool saveConfigFile( void );        // only related to sensOCampus credentials

    boolean begin( const char * );
    bool http_getCredentials( const char * );
    bool http_getConfig( void );
    
    const char *getServer( void ) const;
    uint16_t getServerPort( void ) const;
    const char *getBaseTopic( void ) const;
    const char *getUser( void ) const;
    const char *getPassword( void ) const;
    boolean getModuleConf( const char*, JsonArray* );

private:
    /*
     * private methods
     */
    void _applyDefaults( void );
    bool _parseCredentials( char * );
    bool _parseConfig( const char * );    // VERY IMPORTANT 'const' as it force ArduinoJson to create a copy
    bool _loadConfig( JsonObject );       // interprets JSON buffer from config file
    bool _saveConfig( JsonObject );       // fill JSON buffer with things to save to config file

    /*
     * private attributes
     */
    bool _initialized;                    // check if structure is valid
    bool _updated;                        // any change in current parameters ?
    bool _defaults;                       // default parameters, hence no need to save
    
    wifiParametersMgt *_wp;               // global wifiParametersMgt

    DynamicJsonDocument _modulesJSON = DynamicJsonDocument(SENSO_JSON_SIZE);    // modules configuration

    // grabbed from sensocampus sever
    char _mqtt_server[MQTT_SERVER_NAME_LENGTH];
    uint16_t _mqtt_port;
    char _mqtt_login[SENSO_MQTT_LOGIN_LENGTH];
    char _mqtt_passwd[SENSO_MQTT_PASSWD_LENGTH];
    char _mqtt_base_topic[MQTT_BASE_TOPIC_LENGTH];
};

#endif /* _SENSOCAMPUS_H */

