/*
 * neOCampus operation
 * 
 * MQTT client class.
 * This class leverage the needs for a single MQTT(s) connection shared across
 * various clients that can register a specific callback for each topic.
 * 
 * ---
 * TODO:
 * -
 * 
 * ---
 * F.Thiebolt   aug.20  set MQTT comm class as an independant module in order to
 *                      manage a single TCP(s) connexion with the MQTT server.
 * Thiebolt F. July 17
 * 
 */


#ifndef _NEOCAMPUS_COMM_H_
#define _NEOCAMPUS_COMM_H_

/*
 * Includes
 */

#include <Arduino.h>
#include <ArduinoJson.h>
#include "WiFi.h"

#include "neocampus.h"
#include "PubSubClient.h"
#include "sensocampus.h"


/*
 * Global, shared stuffs
 */
extern bool _need2reboot;


/*
 * Definitions
 */
#ifndef MQTT_CONNECT_MAX_RETRIES
#define MQTT_CONNECT_MAX_RETRIES        3     // maximum number of MQTT connect attempts
#endif

#ifndef BASE_MQTT_MSG_MAXLEN
#define BASE_MQTT_MSG_MAXLEN            256   // maximum length of a MQTT msg (json) sent to the broker
#endif



/*
 * Class
 */
class comm {
  public:
    // constructor(s)
    comm( void );

    boolean start( senso * );
    boolean stop( void );
    
    boolean isConnected( void );
    boolean process( void );

    /* publish */
    boolean publish(const char* topic, const char* payload);
    boolean publish(const char* topic, const uint8_t * payload, unsigned int plength);

    /* modules to register a callback tied to a topic */
    boolean register_cb( const char* topic, MQTT_CALLBACK_SIGNATURE );


    /* 
     * public attributes
     */


  private:
    /*
     * private methods
     */
    // low-level init for constructors
    void _comm( void );

    boolean reConnect( void );
    void msgHandler( char*, uint8_t*, unsigned int );
    void callback( char* topic, byte* payload, unsigned int length );

    /*
     * private attributes
     */
    // array of pointers to callbacks


    // MQTT
    senso *_sensoClient;
    WiFiClient _wifiClient;
    PubSubClient mqttClient;
};


#endif /* _NEOCAMPUS_COMM_H_ */
