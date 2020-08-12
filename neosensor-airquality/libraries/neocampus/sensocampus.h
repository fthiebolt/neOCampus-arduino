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
#ifdef DISABLED_SSL
  #define SENSO_GET_CREDENTIALS_URL       "http://sensocampus.univ-tlse3.fr/device/credentials?mac="
  #define SENSO_GET_CONFIG_URL            "http://sensocampus.univ-tlse3.fr/device/config"
#else
  #define SENSO_GET_CREDENTIALS_URL       "https://sensocampus.univ-tlse3.fr/device/credentials?mac="
  #define SENSO_GET_CONFIG_URL            "https://sensocampus.univ-tlse3.fr/device/config"
#endif /* DISABLED_SSL */
#define SENSO_HTTP_URL_MAXSIZE          128   // maximum size of a URL sent to sensOCampus
#define SENSO_HTTP_MAX_RESPONSE_SIZE    1024  // maximum size of a response sent from sensOCampus
#define SENSO_JSON_SIZE                 (JSON_OBJECT_SIZE(128))   // no more than 128 objects in any sensOCampus JSON response



/*
 * Class
 */
class senso {
  public:
    senso( void );
    senso( wifiParametersMgt * );
    
    bool isValid( void );               // tell parameters have been initialized at least once
    bool loadConfigFile( void );
    bool saveConfigFile( void );

    boolean begin( const char * );
    bool http_getCredentials( const char * );
    bool http_getConfig( void );
    
    const char *getServer( void ) const;
    uint16_t getServerPort( void ) const;
    const char *getBaseTopic( void ) const;
    const char *getUser( void ) const;
    const char *getPassword( void ) const;

private:
    /*
     * private methods
     */
    void _applyDefaults( void );
    bool _parseCredentials( char * );
    bool _parseConfig( char * );
    bool _loadConfig( JsonObject );       // interprets JSON buffer from config file
    bool _saveConfig( JsonObject );       // fill JSON buffer with things to save to config file

    /*
     * private attributes
     */
    bool _initialized;                    // check if structure is valid
    bool _updated;                        // any change in current parameters ?
    bool _defaults;                       // default parameters, hence no need to save
    
    // global wifiParametersMgt
    wifiParametersMgt *_wp;
    
    // grabbed from sensocampus sever
    char _mqtt_server[MQTT_SERVER_NAME_LENGTH];
    uint16_t _mqtt_port;
    char _mqtt_login[SENSO_MQTT_LOGIN_LENGTH];
    char _mqtt_passwd[SENSO_MQTT_PASSWD_LENGTH];
    char _mqtt_base_topic[MQTT_BASE_TOPIC_LENGTH];
};

#endif /* _SENSOCAMPUS_H */

