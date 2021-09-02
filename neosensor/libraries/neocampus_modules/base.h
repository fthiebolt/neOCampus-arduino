/*
 * neOCampus operation
 * 
 * Base module that all others module should inherit from
 * 
 * ---
 * TODO:
 * -
 * 
 * ---
 * F.Thiebolt   aug.21  added JSON variant and module level _trigger
 * F.Thiebolt   apr.21  removed BASE_MQTT_MSG_MAXLEN for MQTT_MAX_PACKET_SIZE
 * F.Thiebolt   Jul.17  initial release
 * 
 */


#ifndef _BASE_H_
#define _BASE_H_

/*
 * Includes
 */

#include <Arduino.h>
#include <ArduinoJson.h>
#if defined(ESP8266)
  #include <ESP8266WiFi.h>
#elif defined(ESP32)
  #include <WiFi.h>
#endif

#include "neocampus.h"
#include "PubSubClient.h"       // MQTT client with some definitions from neocampus.h
#include "sensocampus.h"


/*
 * Global, shared stuffs
 */
extern bool _need2reboot;


/*
 * Definitions
 */
#define MQTT_CONNECT_MAX_RETRIES        3     // maximum number of MQTT connect attempts

#define MODULE_CONFIG_FILE(_NAME_)      ( MCFG_FILE_PREFIX _NAME_ MCFG_FILE_SUFFIX )


/*
 * Class
 */
class base {
  public:
    // constructors
    base( void );           // base definition with unitID as 'auto_xxxx'
    base( const char* );    // base definition with unitID 'provided' (e.g mac addr for the device module)

    // destructor
    virtual ~base( void );

    // MQTT
    virtual bool start( senso *, JsonDocument& );
    virtual bool process( void );
    virtual bool stop( void );
    
    bool reConnect( void );
    bool isTXtime( void );
    void cancelTXslot( void );    // postpone TX to next slot
    bool setFrequency( uint16_t, uint16_t, uint16_t );
    boolean setIdentity( const char *identity=nullptr, boolean append_mac=true );  // set UnitID base field (from senso config for example)
                                            // ... will get added mac addr 2 last digits
                                            // e.g <identity>_<mac[5]mac[6]>
    bool sendmsg( JsonObject );
    virtual void status( JsonObject );

    void callback(char* topic, byte* payload, unsigned int length);
    virtual void handle_msg( JsonObject )=0;

    // module saves its config file
    bool saveConfig( const char*, JsonObject );

    // module load its sensOCampus config (if any)
    virtual boolean loadSensoConfig( senso * ) { return false; };

    /* 
     * public attributes
     */
    uint8_t _sensors_count;         // nb of sensors registered
    uint16_t _freq;                 // seconds between two consecutives messages (i.e coolDown parameter)
    char unitID[SENSO_UNITID_MAXSIZE];      // identity of the module
    char pubTopic[MQTT_BASE_TOPIC_LENGTH];  // topic to publish to
    char subTopic[MQTT_BASE_TOPIC_LENGTH];  // topic to subscribe to
    JsonVariant variant;            // JSON structure (pointer to) part of the global shared JSON structure
    // module level flag to specifiy that at least one digital input trigger has been activated ot that one
    // analog value need to get sent
    boolean _trigger;

  private:
    /*
     * private methods
     */
    // low-level init for constructors
    void _base( void );
    
    /*
     * private attributes
     */
    unsigned long _lastTX;          // elapsed ms since last message sent

    // MQTT
    senso *_sensoClient;
    WiFiClient _wifiClient;
    PubSubClient mqttClient;
};


#endif /* _BASE_H_ */


