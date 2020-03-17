/*
 * neOCampus operation
 * 
 * sensOCampus client class for interactions with sensOCampus server
 * 
 * Thiebolt F. July 17
 * 
 */


#ifndef _SENSOCAMPUS_H_
#define _SENSOCAMPUS_H_

/*
 * Includes
 */

#include <Arduino.h>
//#include <ArduinoJson.h>

#include "neocampus.h"
#include "neocampus_debug.h"

#include "wifiParametersMgt.h"



/*
 * Definitions
 */
#define SENSO_GET_CREDENTIALS_URL       "http://sensocampus.univ-tlse3.fr/device/credentials?mac="
#define SENSO_GET_CONFIG_URL            "http://sensocampus.univ-tlse3.fr/device/config"
#define SENSO_HTTP_URL_MAXSIZE          128   // maximum size of a URL sent to sensOCampus
#define SENSO_HTTP_MAX_RESPONSE_SIZE    1024  // maximum size of a response from from an url sent to sensOCampus
#define SENSO_JSON_SIZE                 (JSON_OBJECT_SIZE(128))   // no more than 128 objects in any sensOCampus JSON response
#define SENSO_UNITID_SIZE               18    // unitID field maxsize (mac addr is 17 bytes)


/*
 * Class
 */
class senso {
  public:
    senso( void );
    senso( wifiParametersMgt * );
    
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

    /*
     * private attributes
     */
    // check if structure is valid
    bool _initialized;
    
    // global wifiParametersMgt
    wifiParametersMgt *_wifiParams;
    
    // grabbed from sensocampus sever
    char _mqtt_server[MQTT_SERVER_NAME_LENGTH];
    uint16_t _mqtt_port;
    char _mqtt_login[SENSO_MQTT_LOGIN_LENGTH];
    char _mqtt_passwd[SENSO_MQTT_PASSWD_LENGTH];
    char _mqtt_base_topic[MQTT_BASE_TOPIC_LENGTH];
};

#endif /* _SENSOCAMPUS_H */
