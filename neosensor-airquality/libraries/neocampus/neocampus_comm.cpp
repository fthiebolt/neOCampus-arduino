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


/*
 * Includes
 */
#include "neocampus.h"
#include "neocampus_comm.h"
#include "neocampus_utils.h"
#include "neocampus_debug.h"

//#include "modulesMgt.h"


/*
 * Global shared variables/functions
 */



/*
 * Definitions
 */



// constructor
comm::comm( void ) {

  // call low-level base constructor
  _comm();
}

// low-level base constructor
void comm::_comm( void ) {
  _sensoClient = nullptr;
}


/*
 * MQTT communications startup procedure
 */
boolean comm::start( senso *sensocampus ) {

  if( not sensocampus ) return false;

  bool _ret;
  
  // save pointer to sensOCampus object
  _sensoClient = sensocampus;
    
  // configure MQTT client
  mqttClient.setClient( _wifiClient );
  mqttClient.setServer( _sensoClient->getServer(), _sensoClient->getServerPort() );
  mqttClient.setCallback( [this] (char* topic, byte* payload, unsigned int length) { this->callback(topic, payload, length); });
    
  // launch MQTT connexion + subscriptions + ...
  _ret = this->reConnect();
    
  return _ret;
}

/*
 * Module stop  MQTT, save config (if any) and clean things
 */
boolean comm::stop( void ) {
  bool _ret = true;
    
  if( mqttClient.connected() ) {
/*
    // unsubscribe
    log_info(F("\n\t[comm] unsubscribe from topic: ")); log_info(subTopic);
    _ret = mqttClient.unsubscribe( subTopic );
    if( _ret ) {
      log_info(F("\n\t[comm] topic successfully unsubscribed :)"));
    }
*/    
    // Stop MQTT connexion
    log_info(F("\n\t[comm] disconnect from MQTT server ... "));
    mqttClient.disconnect();
  }
  else {
    log_info(F("\n\t[comm] stop module while mqtt not connected ... "));
  }

  log_flush();

  return _ret;
}

/*
 * Callback: start processing MQTT messages received from broker
 */
void comm::callback(char* topic, byte* payload, unsigned int length) {
#if 0

TODO later

  // verify that topic matches the subscribed one
  if( strncmp(topic, subTopic, strlen(subTopic)) ) {
    log_error(F("\n[base][callback] unknwown topic: ")); log_debug(topic); log_flush();
    return;
  }

  // does it matches our ID (i.e dest == "all" or dest == <unitID>)
  StaticJsonDocument<COMMAND_JSON_SIZE> root;
  
  auto err = deserializeJson( root, payload );
  if( err ) {
    log_error(F("\n[base][callback] ERROR parsing JSON payload @ callback: "));log_error(err.c_str()); log_flush();
    return;
  }

  // check for destID field
  if( (root.containsKey(F("dest"))==false) ) {
    log_error(F("\n[base][callback] ERROR no 'dest' field' in JSON payload @ callback ?!?!"));
    return;
  }
  /* ok, there's a dest, does it matches us 
   * (i.e dest='all' or dest=<our unitID> ??)
   */
  // warning, we have a pointer to an inner part of a json message ...
  char *_dest = (char*)(root[F("dest")].as<char*>());
  // ... then convert to lowercase :)
  for( uint8_t i=0; i<strlen(_dest); i++ ) _dest[i] = tolower(_dest[i]);

  const char *generic_dest = PSTR("all");
  if( strncmp_P(_dest, generic_dest, strlen_P(generic_dest))!=0 and
      strncmp(_dest, unitID, strlen(unitID))!=0 ) {

    // not for us
    return;
  }
    
  // call handler
  handle_msg( root.as<JsonObject>() );
#endif
}


/*
 * reConnect
 */
boolean comm::reConnect( void ) {
  // compute MQTT clientID
  String clientID;
  clientID += getAPname();
  clientID += "_";
  clientID += String(micros() & 0xffff, HEX);
  //clientID += String(random(0xffff), HEX);
  
  log_info(F("\n\t[comm] (re)connect to MQTT server with CID = ")); log_debug(clientID.c_str()); log_flush();

  // Loop until we're reconnected
  uint8_t _retry=MQTT_CONNECT_MAX_RETRIES;
  while( !mqttClient.connected() ) {
  
    if( mqttClient.connect( (char*)clientID.c_str(), _sensoClient->getUser(), _sensoClient->getPassword() ) ) {
      yield();
      
      bool _ret;
      
      // success :)
      log_debug(F("\n\t[comm] connected :)"));
      
      // Once connected, publish an announcement...
      //ret = mqttClient.publish("outTopic", "hello world");
      yield();
#if 0
TODO: subscriptions      
      // ... and resubscribe
      _ret = mqttClient.subscribe( subTopic );
      yield();
      if( _ret ) {
        log_debug(F("\n\t[comm] topic subscribed: ")); log_debug( subTopic ); log_flush();
      }
      else {
        // we continue even upon a subscribe failure
        log_error(F("\n\t[comm] ERROR unable to subscribe to topic: ")); log_error( subTopic ); log_flush();
      }
#endif
    }
    else {
      
      // failure :(
      log_error(F("\n\t[comm] connect failed with rc = "));
      log_error(mqttClient.state(),DEC);
      
      if( _retry-- ) {
        log_debug(F("\n\t\t... sleeping a bit before retrying ..."));
        delay((MQTT_CONNECT_MAX_RETRIES-_retry)*10*1000);
      }
      else {
        log_error(F("\n\t[comm] ERROR no more retries to connect with MQTT broker ... reboot"));
        _need2reboot = true;
        return false;
      }
    }
  }
  
  return true;
}


/*
 * loop to process module's messages requiring callback call
 */
boolean comm::process( void ) {

  bool _ret;
  
  // check we're already connected
  if( !mqttClient.connected() ) {
    // if reconnect fails ESP will reboot so ne need to check return value
    reConnect();
  }
  
  // MQTT client loop() to process messages requiring handler to get called
  _ret = mqttClient.loop();
  
  if( !_ret ) {
    log_error(F("\n[comm] ERROR process() with rcState = ")); log_error(mqttClient.state(),DEC);log_flush();
  }
  
  return _ret;
}
