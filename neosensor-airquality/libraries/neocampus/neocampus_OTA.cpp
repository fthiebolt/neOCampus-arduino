/*
 * neOCampus operation
 * 
 * Over The Air (OTA) firmware upgrade utilities
 * 
 * ---
 * F.Thiebolt   aug.20  removed EEPROM support (useless with ESP chips ;) )
 * Thiebolt F. Nov.19   upgrade to Arduino JSON6
 * Thiebolt F. August 17
 * 
 */



/* ----------------------------------------------------------------------------
 * --- Includes ---------------------------------------------------------------
 * ----------------------------------------------------------------------------
 */
#include <Arduino.h>
#include <ArduinoJson.h>          // https://github.com/bblanchon/ArduinoJson
#ifdef ESP8266
  #include <ESP8266httpUpdate.h>
#else /* ESP32 */
  #include <HTTPUpdate.h>
#endif

#include "neocampus.h"
#include "neocampus_debug.h"

#include "neocampus_OTA.h"
#include "neocampus_utils.h"



/* ----------------------------------------------------------------------------
 * --- Global shared variables/functions --------------------------------------
 * ----------------------------------------------------------------------------
 */
extern bool _need2reboot;



/* ----------------------------------------------------------------------------
 * --- Definitions ------------------------------------------------------------
 * ----------------------------------------------------------------------------
 */
#define OTA_JSON_SIZE       (JSON_OBJECT_SIZE(10))    // no more than 10 fields in JSON structure



/* ----------------------------------------------------------------------------
 * --- Functions --------------------------------------------------------------
 * ----------------------------------------------------------------------------
 */

/*
 * Main firmware update function
 */
bool neOCampusOTA( void ) {
  
  if( _need2reboot ) return false;
  
  log_info(F("\n[OTA] start checking for firmware update ..."));
  
  // build url to json file
  char ota_url[OTA_URL_LENGTH];
  unsigned int _wlen;
  _wlen=snprintf(ota_url, sizeof(ota_url),
                 "%s/%s/%s/%s", OTA_BASE_URL,getHardware(),getBoardName(),OTA_JSON_FILE);
  if( _wlen >= sizeof(ota_url) ) {
    log_error(F("\n[OTA] error OTA_URL buffer undersized!")); log_flush();
    return false;
  }
  
  // get url's ArduinoJson file content
  char ota_json[OTA_JSON_LENGTH];
  if( http_get( ota_url, ota_json, sizeof(ota_json) ) != true ) {
    return false;
  }

  // get firmware revision from json and compare to current
  StaticJsonDocument<OTA_JSON_SIZE> root;
  auto err = deserializeJson( root, ota_json );
  if( err ) {
    log_error(F("\n[OTA] ERROR parsing JSON file!"));
    log_error(F("\n[OTA] ERROR msg: ")); log_error(err.c_str()); log_flush();
    return false;
  }

  // check for revision field
  if( (root.containsKey(F("revision"))==false) ) {
    log_error(F("\n[OTA] ERROR no 'revision' field' in JSON file ?!?!"));
    return false;
  }
  // ok, let's acquire this revision field 
  firmwareRev_t remoteFWver = (firmwareRev_t)(root[F("revision")].as<unsigned int>());
  
  if( remoteFWver <= getFirmwareRev() ) {
    // nothing todo, we're already with the latest version
    log_info(F("\n[OTA] current firmware version "));
    log_info(getFirmwareRev(),DEC); log_info(F(" is equal or higher than remote ")); log_info(remoteFWver,DEC);
    log_flush();
    return false;
  }

  // check for image field
  if( (root.containsKey(F("image"))==false) ) {
    log_error(F("\n[OTA] ERROR no 'image' field' in JSON file ?!?!"));
    return false;
  }

  log_info(F("\n[OTA] on way to Firmware upgrade from "));
  log_info(getFirmwareRev(),DEC); log_info(F(" revision to new version ")); log_info(remoteFWver,DEC);
  log_flush();
  
  yield();
  /*
   * WARNING: root["image"] is a pointer to ArduinoJson inner buffer
   */
  return neOCampusOTA_url(root[F("image")]);
}


// upgrade from image's url
bool neOCampusOTA_url( const char *img_url ) {

  bool status = false;
  
  log_debug(F("\n[OTA] choosen firmware: "));log_debug(img_url);log_flush();
  // disable reboot on update
  ESPhttpUpdate.rebootOnUpdate(false);

  t_httpUpdate_return ret = ESPhttpUpdate.update(img_url);
  switch(ret) {
    case HTTP_UPDATE_FAILED:
      log_error(F("[OTA] ERROR update failed (!!): ")); log_error(ESPhttpUpdate.getLastErrorString());
      log_error(F(" ... continuing ...")); log_flush();
      break;
    case HTTP_UPDATE_NO_UPDATES:
      log_error(F("[OTA] no update ?!?!: ")); log_error(ESPhttpUpdate.getLastErrorString());
      log_error(F("... continuing ...")); log_flush();
      break;
    case HTTP_UPDATE_OK:
      log_info(F("\n[OTA] Update ok :)")); log_flush();
      status = true;
      break;
  }

  // check status
  if( ! status ) {
    return status;
  }
  
  // closing things before restart
  log_info(F("\n[OTA] prepare things before restarting with new firmware ..."));
  _need2reboot = true;
  
  return status;
}


// get current firmwre revision
firmwareRev_t getFirmwareRev() {
  return BOARD_FWREV;
}

// get hardware (e.g ESP8266, ESP32, unknown ...)
const char *getHardware( void ) {
#ifdef ESP8266
  static const char hardware[] = "esp8266";
#elif ESP32
  static const char hardware[] = "esp32";
#else
  static const char hardware[] = "unknown";
#endif
  return hardware;
}

// get board name (e.g neOSensor)
const char *getBoardName( void ) {
  return BOARD_NAME;
}

// get board revision
uint8_t getBoarRev( void ) {
  return (uint8_t)(BOARD_REVISION);
}


