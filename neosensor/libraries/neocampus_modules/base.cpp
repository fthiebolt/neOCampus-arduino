/*
 * neOCampus operation
 * 
 * Base for all kinds of module sensors (temperature, luminosity etc)
 * 
 * Thiebolt F. July 17
 * 
 */


/*
 * Includes
 */
#include <FS.h>
#if defined(ESP32)
  #include "SPIFFS.h"
#endif
#include <limits.h>

#include "neocampus.h"
#include "neocampus_debug.h"

#include "base.h"

#include "neocampus_utils.h"
#include "modulesMgt.h"


/*
 * Global shared variables/functions
 */
extern "C" modulesMgt modulesList;



/*
 * Definitions
 */
#define COMMAND_JSON_SIZE       (JSON_OBJECT_SIZE(5))


// constructor
base::base( void ) {
  // unitID: generate 'auto_xxxx' identity
  setIdentity();

  // call low-level base constructor
  _base();
}

// constructor
base::base( const char *identity ) {

  setIdentity( identity );
  //snprintf( unitID, sizeof(unitID), identity );
    
  // call low-level base constructor
  _base();
}

// low-level base constructor
void base::_base( void ) {
  _lastTX = 0;
  _sensors_count = 0;
  _sensoClient = NULL;
  
  pubTopic[0] = '\0';
  subTopic[0] = '\0';
}

/*
 * Module network startup procedure (MQTT)
 */
bool base::start( senso *sensocampus ) {

  bool _ret;
  
  // save pointer to sensOCampus object
  _sensoClient = sensocampus;
    
  // configure MQTT client
  mqttClient.setClient( _wifiClient );
  mqttClient.setServer( _sensoClient->getServer(), _sensoClient->getServerPort() );
  mqttClient.setCallback( [this] (char* topic, byte* payload, unsigned int length) { this->callback(topic, payload, length); });
    
  // launch MQTT connexion + subscriptions + ...
  _ret = reConnect();
  
  /* start lastmsg time measurement.
   * This way, we get sure to have at least a first msg! */
  _lastTX = ULONG_MAX/2;
  
  return _ret;
}

/*
 * Module stop  MQTT, save config (if any) and clean things
 */
bool base::stop( void ) {
  bool _ret = true;
    
  if( mqttClient.connected() ) {

    // unsubscribe
    log_info(F("\n\t[base] unsubscribe from topic: ")); log_info(subTopic);
    _ret = mqttClient.unsubscribe( subTopic );
    if( _ret ) {
      log_info(F("\n\t[base] topic successfully unsubscribed :)"));
    }
    
    // Stop MQTT connexion
    log_info(F("\n\t[base] disconnect from MQTT server ... "));
    mqttClient.disconnect();
  }
  else {
    log_info(F("\n\t[base] stop module while mqtt not connected ... "));
  }

  log_flush();

  return _ret;
}

/*
 * Callback: start processing MQTT messages received from broker
 */
void base::callback(char* topic, byte* payload, unsigned int length) {
  // verify that topic matches the subscribed one
  if( strncmp(topic, subTopic, strlen(subTopic)) ) {
    log_error(F("\n\t[base][callback] unknwown topic: ")); log_debug(topic); log_flush();
    return;
  }

  // does it matches our ID (i.e dest == "all" or dest == <unitID>)
  StaticJsonDocument<COMMAND_JSON_SIZE> root;
  
  auto err = deserializeJson( root, payload );
  if( err ) {
    log_error(F("\n\t[base][callback] ERROR parsing JSON payload @ callback: "));log_error(err.c_str()); log_flush();
    return;
  }

  // check for destID field
  if( (root.containsKey(F("dest"))==false) ) {
    log_error(F("\n\t[base][callback] ERROR no 'dest' field' in JSON payload @ callback ?!?!"));
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
}


/*
 * reConnect
 */
bool base::reConnect( void ) {
  // compute MQTT clientID
  String clientID;
  clientID += getAPname();
  clientID += "_";
  clientID += String(micros() & 0xffff, HEX);
  //clientID += String(random(0xffff), HEX);
  
  log_info(F("\n\t[base] (re)connect to MQTT server with CID = ")); log_debug(clientID.c_str()); log_flush();

  // Loop until we're reconnected
  uint8_t _retry=MQTT_CONNECT_MAX_RETRIES;
  while( !mqttClient.connected() ) {
  
    if( mqttClient.connect( (char*)clientID.c_str(), _sensoClient->getUser(), _sensoClient->getPassword() ) ) {
      yield();
      
      bool _ret;
      
      // success :)
      log_debug(F("\n\t[base] connected :)"));
      
      // Once connected, publish an announcement...
      //ret = mqttClient.publish("outTopic", "hello world");
      yield();
      
      // ... and resubscribe
      _ret = mqttClient.subscribe( subTopic );
      yield();
      if( _ret ) {
        log_debug(F("\n\t[base] topic subscribed: ")); log_debug( subTopic ); log_flush();
      }
      else {
        // we continue even upon a subscribe failure
        log_error(F("\n\t[base] ERROR unable to subscribe to topic: ")); log_error( subTopic ); log_flush();
      }
    }
    else {
      
      // failure :(
      log_error(F("\n\t[base] connect failed with rc = "));
      log_error(mqttClient.state(),DEC);
      
      if( _retry-- ) {
        log_debug(F("\n\t\t... sleeping a bit before retrying ..."));
        delay((MQTT_CONNECT_MAX_RETRIES-_retry)*10*1000);
      }
      else {
        log_error(F("\n\t[base] ERROR no more retries to connect with MQTT broker ... reboot"));
        /* call to reboot of our sensor
         * [jun.18] set flag that will ask for a reboot */
        // neOSensor_reboot();
        // delay(5000); // to avoid an infinite loop in case reboot fails
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
bool base::process( void ) {

  bool _ret;
  
  // check we're already connected
  if( !mqttClient.connected() ) {
    // if reconnect fails ESP will reboot so ne need to check return value
    reConnect();
  }
  
  // MQTT client loop() to process messages requiring handler to get called
  _ret = mqttClient.loop();
  
  if( !_ret ) {
    log_error(F("\n\t[base] ERROR process() with rcState = ")); log_error(mqttClient.state(),DEC);log_flush();
  }
  
  return _ret;
}



/*
 * Tells whether it's time to TX a new msg ... or not yet :)
 */
bool base::isTXtime( void ) {
  /*
   * look at https://arduino.stackexchange.com/questions/33572/arduino-countdown-without-using-delay/33577#33577
   * for an explanation about millis() that wrap around!
   */
  return ( (millis() - _lastTX) >= ((unsigned long)_freq)*1000UL );
}


/*
 * set data module's data acquisition frequency
 */
bool base::setFrequency( uint16_t freq, uint16_t min_freq, uint16_t max_freq ) {

  // check validity range
  if( freq < min_freq ) {
    freq = min_freq;
  }
  else if( freq > max_freq ) {
    freq = max_freq;
  }

  _freq = freq;

  log_debug(F("\n\t[base] set module's data acquisition frequency to ")); log_debug(_freq,DEC); log_flush();
  
  // reset _lastTX value (so we restart a whole new cycle
  _lastTX = millis();

  return true;
}


/*
 * set data module's data acquisition frequency
 */
boolean base::setIdentity( const char *identity ) {

  const char *mac=getMacAddress();
  uint8_t mac_len=strlen(mac);
  snprintf( unitID, sizeof(unitID), "%s_%c%c%c%c", (identity ? identity : "auto"), mac[mac_len-5],mac[mac_len-4],mac[mac_len-2],mac[mac_len-1]);

  //log_debug(F("\n\t[base] set module's identity to ")); log_debug(unitID); log_flush();
  
  return true;
}


/*
 * send MQTT messages
 */
bool base::sendmsg( JsonObject root ) {
  
  bool _ret = false;

  // static way to hold text json frame
  char jFrame[BASE_MQTT_MSG_MAXLEN];
  
  // add basic identity
  if( (root.containsKey(F("unitID"))==false) ) {
    root[F("unitID")] = unitID;
  }

  // print frame to temporary buffer
  if( serializeJson( root, jFrame, sizeof(jFrame) ) >= sizeof(jFrame) ) {
    log_error(F("\n[base] ERROR undersized BASE_MQTT_MSG_MAXLEN buffer !")); log_flush();
    return false;
  }
  
  /*
   * WARNING heavy debug, do not activate!!
   */
#ifdef MQTT_LOWLEVEL_DEBUG
  log_debug(F("\n[base] MQTT msg = ")); log_debug(jFrame); log_debug(F(" --> ")); log_debug(pubTopic); log_flush();
#endif /* MQTT_LOWLEVEL_DEBUG */

  // send message :)
  uint8_t _retry=3;
  while( --_retry ) {
    _ret = mqttClient.publish(pubTopic, jFrame);
    if( _ret ) break;
    
    // error sending message ... we'll retry
    log_debug(F("\n[base] WARNING failed MQTT msg delivery ... retrying ...")); log_flush();
    delay(100); // add delay before retrying
  }

  if( !_ret ) {
    log_error(F("\n[base] ERROR failure MQTT msg delivery :(")); log_flush();
  }

  // success or failure, we update lastTx field to avoid avalanche of sendmsg
  _lastTX = millis();
  yield();

  return _ret;
}


/*
 * Status report sending
 */
void base::status( JsonObject root ) {
  
  // upto now, everything is OK :)
  root[F("status")] = "OK";
  
  // frequency
  root[F("frequency")] = _freq;
  
  /* number of sensors
   * [jun.18] for a device, sensr count is the number of active modules ...
   */
  //root[F("sensors")] = _sensors_count;
  root[F("sensors")] = modulesList.count(); // remember that device is NOT a sensor (while it adds 1 to the number of modules)
  
  // current time
  root[F("time")] = getCurTime(); // warning: not reentrant function ...
  
  
}


/*
 * Save module's config in file
 */
bool base::saveConfig( const char *filename, JsonObject root ) {

  // add parameters to save here
  
  
  
  // is there at least one thing to save ?
  if( root.size()==0 ) {
    // nothing to save ... thus we delete the config file (if any)
    log_debug(F("\n[base] remove JSON config file: "));log_debug(filename);log_flush();
    return SPIFFS.remove(filename);
  }
  
  // ok, there's something to save at least
  File configFile = SPIFFS.open( filename, "w"); // truncate if exists or create otherwise
  if( !configFile ) {
    log_error(F("\n[base] error creating file: "));log_debug(filename);log_flush();
    return false;
  }
  
  // save JSON to file
  if( serializeJson( root, configFile )==0 ) {
    configFile.close();
    log_error(F("\n[base] error unable to write data to file: "));log_debug(filename);log_flush();
    return false;
  }
  
  // close file
  configFile.close();
  
  log_info(F("\n[base] successfully saved JSON config file: "));log_debug(filename);log_flush();
  
  return true;
}
