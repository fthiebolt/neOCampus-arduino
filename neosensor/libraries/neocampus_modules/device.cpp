/*
 * neOCampus operation
 * 
 * Device module for high-level end-device management
 *
 * Thiebolt F. Nov.19   migrate to Arduino Json 6
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

#include "neocampus.h"
#include "neocampus_debug.h"

#include "device.h"

#include "neocampus_utils.h"
#include "neocampus_OTA.h"


/*
 * Definitions
 */
#define MQTT_MODULE_NAME        "device"  // used to build module's base topic
#define DATA_JSON_SIZE          (JSON_OBJECT_SIZE(20))
#define CONFIG_JSON_SIZE        (JSON_OBJECT_SIZE(3))   // config file contains: frequency



// constructor
device::device(): base( getMacAddress() ) 
{
  _freq = DEFL_DEVICE_FREQUENCY;

  // load json config file (if any)
  loadConfig();
}


/*
 * Module network startup procedure (MQTT)
 */
bool device::start( senso *sensocampus ) {

    log_info(F("\n[device] starting module ..."));
    // initialize module's publish & subscribe topics
    snprintf( pubTopic, sizeof(pubTopic), "%s/%s", sensocampus->getBaseTopic(), MQTT_MODULE_NAME);
    snprintf( subTopic, sizeof(subTopic), "%s/%s", pubTopic, "command" );
    return base::start( sensocampus );
}


/*
 * Handler for subscribed messages
 */
void device::handle_msg( JsonObject root ) {
  log_info(F("\n[device][callback] received COMMAND to process ...")); log_flush();
  
  const char *_key_order = NULL;    // current 'order'
  int _key_value;                   // current 'value' (as an integer)
  bool _key_value_valid  = false;   // is a key_value already get saved in _key_value ?
  const char *_key_svalue= NULL;    // current 'value' (as a string)

  // iterate over all [key,values] pairs
  for (JsonObject::iterator it=root.begin(); it!=root.end(); ++it) {

    {
      // skip 'dest'
      const char *_key = PSTR("dest");
      if( strncmp_P(it->key().c_str(), _key, strlen_P(_key))==0 ) {
        continue;
      }
    }
    
    {
      const char *_key = PSTR("order");
      if( strncmp_P(it->key().c_str(), _key, strlen_P(_key))==0 ) {
        // order received ... need to parse ...
        _key_order = it->value();
        continue;
      }
    }

    {
      const char *_key = PSTR("value");
      if( strncmp_P(it->key().c_str(), _key, strlen_P(_key))==0 ) {
        // value received ... 
        if( it->value().is<int>() ){
          _key_value = it->value().as<int>();
          _key_value_valid = true;
        }
        else if( it->value().is<const char*>() ){
          _key_svalue = it->value();
        }
        continue;
      }
    }

    // unknown key ?!?!
    log_error(F("\n[device][callback] unknown key: "));log_debug((const char*)(it->key().c_str())); log_flush();
  }

  // if an order has already been received ...
  if( _key_order ) _processOrder( _key_order, (_key_value_valid==true ? &_key_value : NULL), _key_svalue );

}


/*
 * process module's activites
 */
bool device::process( void ) {

  bool _ret = false;
  
  /* 1st step, call process from base because
   * it will check MQTT client connectivity
   * and call handler for MQTT received messages
   */
  _ret = base::process();

  // reached time to transmit ?
  if( !isTXtime() ) return _ret;

  /*
   * Time to send a new message
   */
  StaticJsonDocument<DATA_JSON_SIZE> _doc;
  JsonObject root = _doc.to<JsonObject>();

  /* add items to JSON object:
   * - current status
   */
  status( root );

  // send MQTT message
  if( !sendmsg( root ) ) {
    log_error(F("\n[device] ERROR failure MQTT msg delivery :(")); log_flush();
    return false;
  }
  
  log_info(F("\n[device] successfully published msg :)")); log_flush();
  return true;
}


/*
 * Status report sending
 */
void device::status( JsonObject root ) {
  
  // add base class status
  base::status( root );
    
  /*
   * TODO:
   * - active modules list
   * - uptime
   * - time (ntp)
   * - CPU core's temperature
   */
  root[F("heap")] = ESP.getFreeHeap();
#ifdef ESP8266
  root[F("hardware")] = F("esp8266");
#elif ESP32
  root[F("hardware")] = F("esp32");
#else
  root[F("hardware")] = F("unknown");
#endif
  root[F("firmware")] = getFirmwareRev();
  root[F("board")] = BOARD_NAME;
  root[F("board_rev")] = BOARD_REVISION;
  
}


/*
 * Module's sensOCampus config to load (if any)
 */
boolean device::loadSensoConfig( senso *sp ) {

  JsonArray _array;
  if( ! sp->getModuleConf( MQTT_MODULE_NAME, &_array ) ) {
    //log_debug(F("\n[device] no sensOCampus config found")); log_flush();
    return false;
  }

  log_debug(F("\n[device] loading sensOCampus config is NOT YET IMPLEMENTED!")); log_flush();
  return false;



// TO BE CONTINUED



  // (re)load the local config file (to override default parameters values from sensOCampus)
  log_debug(F("\n[device] (re)loading config file (if any)")); log_flush();
  loadConfig();

  return true;
}



/* ------------------------------------------------------------------------------
 * Private methods 
 */

/*
 * orders processing ...
 */
bool device::_processOrder( const char *order, int *value, const char *svalue ) {

  if( !order ) return false;
  
  {
    const char *_order = PSTR("status");
    if( strncmp_P(order, _order, strlen_P(_order))==0 ) {
      // required to send status ... so publishing while in callback :)
      StaticJsonDocument<DATA_JSON_SIZE> _doc;
      JsonObject root = _doc.to<JsonObject>();
      status( root );
      return sendmsg( root );
    }
  }

  {
    const char *_order = PSTR("frequency");
    if( strncmp_P(order, _order, strlen_P(_order))==0 ) {
      if( value ) {
        setFrequency( (uint16_t)(*value), DEVICE_MIN_FREQUENCY, DEVICE_MAX_FREQUENCY );
        StaticJsonDocument<DATA_JSON_SIZE> _doc;
        JsonObject root = _doc.to<JsonObject>();
        status( root );
        sendmsg( root );
        return saveConfig();
      }
      else return false;
    }
  }

  {
    const char *_order = PSTR("restart");
    if( strncmp_P(order, _order, strlen_P(_order))==0 ) {
        log_debug(F("\n[device] ORDER to restart application (will reboot in fact)... please wait ..."));
        _need2reboot = true;
    }
  }

  {
    const char *_order = PSTR("reboot");
    if( strncmp_P(order, _order, strlen_P(_order))==0 ) {
        log_debug(F("\n[device] ORDER to reboot whole device ... please wait ..."));
        _need2reboot = true;
    }
  }

  {
    const char *_order = PSTR("update");
    if( strncmp_P(order, _order, strlen_P(_order))==0 ) {
        log_debug(F("\n[device] ORDER to update json configuration from sensocampus ... please wait ..."));
        // retrieve JSON config from sensOCampus
        // TODO
        log_info(F("\nTODO: fetch latest JSON config from sensOCampus ..."));
        delay(1000);
    }
  }

  {
    const char *_order = PSTR("upgrade");
    if( strncmp_P(order, _order, strlen_P(_order))==0 ) {
        log_debug(F("\n[device] ORDER for a firmware upgrade ... please wait ..."));
        
        // any 'http' (url) included ??
        //char *url = strstr(svalue,"http");
        //if( url ) {
        if( svalue ) {
          return neOCampusOTA_url( svalue );
        }
        else {
          return neOCampusOTA();
        }
    }
  }

  log_error(F("\n[device][callback] unknown order: ")); log_debug(order); log_flush();
  return false;
}

/*
 * load an eventual module'specific config file
 */
bool device::loadConfig( void ) {
  
  if( ! SPIFFS.exists( MODULE_CONFIG_FILE(MQTT_MODULE_NAME) ) ) return false;

  File configFile = SPIFFS.open( MODULE_CONFIG_FILE(MQTT_MODULE_NAME), "r");
  if( !configFile ) return false;

  log_info(F("\n[device] load JSON config file")); log_flush();
  size_t size = configFile.size();
  // Allocate a buffer to store contents of the file.
  std::unique_ptr<char[]> buf(new char[size]);
  configFile.readBytes(buf.get(), size);
  configFile.close();

  // allocate JSON static buffer for module's config file
  StaticJsonDocument<CONFIG_JSON_SIZE> root;

  auto err = deserializeJson( root, buf.get() );
  if( err ) {
    log_error(F("\n[device] ERROR parsing module JSON config file!")); 
    log_error(F("\n[device] ERROR msg: ")); log_error(err.c_str()); log_flush();
    SPIFFS.remove( MODULE_CONFIG_FILE(MQTT_MODULE_NAME) );
    return false;
  }
#if (LOG_LEVEL >= LOG_LVL_DEBUG)
  serializeJsonPretty( root, Serial );
#endif

  // parse and apply JSON config
  return _loadConfig( root.as<JsonObject>() );
}

/*
 * low-level load JSON config
 */
bool device::_loadConfig( JsonObject root ) {
  
  // check for 'frequency' field
  if( root.containsKey(F("frequency")) ) {
    setFrequency( (uint16_t)(root[F("frequency")].as<unsigned int>()), DEVICE_MIN_FREQUENCY, DEVICE_MAX_FREQUENCY );
  }

  /*
   * Parse additional fields here
   */
  
  return true;
}

/*
 * save module'specific config file
 */
bool device::saveConfig( void ) {
  
  // static JSON document
  StaticJsonDocument<CONFIG_JSON_SIZE> _doc;
  JsonObject root = _doc.to<JsonObject>();

  // frequency
  if( _freq != (uint16_t)DEFL_DEVICE_FREQUENCY )
    root[F("frequency")] = _freq;

  // add additional parameters to save here
  
  
  // call parent save
  return base::saveConfig( MODULE_CONFIG_FILE(MQTT_MODULE_NAME), root );
}

