/*
 * neOCampus operation
 * 
 * Utilities functions like
 * - WiFi setup
 * - OTA updates
 * - SPIFFS related
 * - JSON encoding / decoding functions
 * - ...
 * 
 * ---
 * F.Thiebolt   aug.20  removed EEPROM support
 * F.Thiebolt   Nov.19  set 30s WiFi connexion timeout with previous credentials
 * Thiebolt F. July 17
 * 
 */


/* ----------------------------------------------------------------------------
 * --- Includes ---------------------------------------------------------------
 * ----------------------------------------------------------------------------
 */
#include <FS.h>
#if defined(ESP32)
  #include "SPIFFS.h"
#endif

#include <Arduino.h>
#include <Esp.h>                  // ESP specific API for watchdog and others functionnalities

#ifdef ESP8266
  #include <ESP8266HTTPClient.h>
#elif defined(ESP32)
  #include <HTTPClient.h>
#endif

//needed for WiFiManager library
#include <DNSServer.h>            // for captive portal
#include <WiFiManager.h>          // https://github.com/tzapu/WiFiManager
#include <ArduinoJson.h>          // https://github.com/bblanchon/ArduinoJson

#include <Dns.h>                  // getHostByName
#include <time.h>                 // time() ctime()

#include "neocampus.h"

#include "neocampus_debug.h"
#include "neocampus_utils.h"



/* ----------------------------------------------------------------------------
 * --- Global variables -------------------------------------------------------
 * ----------------------------------------------------------------------------
 */
bool _WMsaveAddonConfigFlag = false;



/* ----------------------------------------------------------------------------
 * --- Functions --------------------------------------------------------------
 * ----------------------------------------------------------------------------
 */

// --- CLEAR switch management ------------------------------------------------

/* check clear switch for CLEAR procedure
 * if pin == -1 --> check disabled
 */
bool checkCLEARswitch( uint8_t sw_pin ) {
  if (sw_pin == INVALID_GPIO) {
    log_debug(F("\n[CLEAR] no CLEAR_SW defined !")); log_flush();
    return false;
  }

  // set pin as input
  pinMode( sw_pin, INPUT );
  
  log_debug(F("\n[CLEAR] check for clear_switch activation ...")); log_flush();
  
  bool _res = true;
  // ... then read input for a specific time
  for( uint8_t cpt=10; cpt > 0; cpt-- ) {
    if ( digitalRead( sw_pin ) != HIGH ) {
      // at least read as LOW one time --> cancel clear operation
      _res = false;
      break;
    }
    delay(50);  // sleep for 50ms (allowing ESP to manage internals)
  }
  
  /* okay, delay for clear switch acquisition is now over.
   * If we need to clear things, we want to wait for sw_pin
   * LOW (i.e user stopped to press switch)
   */
  if( _res==true ) {
    log_info(F("\n[CLEAR] activate CLEAR procedure ..."));
    log_debug(F("\n[CLEAR] release SWITCH for clear to start ..."));
    log_flush();
  }
  while( digitalRead( sw_pin )==HIGH ) {
    delay(250);
  }
  
  return _res;
}



// ----------------------------------------------------------------------------
// --- WiFi setup -------------------------------------------------------------

/*
 * compute WiFi Access Point Name
 */
const char *getAPname( void ) {
  static char _apName[16] = "";
  
  if( !strlen(_apName) ) {
    const char *mac=getMacAddress();
    uint8_t mac_len=strlen(mac);
//    snprintf(_apName,sizeof(_apName),"neOSensor_%c%c%c%c",mac[mac_len-5],mac[mac_len-4],mac[mac_len-2],mac[mac_len-1]);
    snprintf(_apName,sizeof(_apName),"%s_%c%c%c%c",BOARD_NAME,mac[mac_len-5],mac[mac_len-4],mac[mac_len-2],mac[mac_len-1]);
  }
  
  return (const char*)_apName;
}

/* 
 * WiFiManager : callback upon exit of AP mode (i.e success)
 * - callback notifying us of the need to save additional parameters
 */
void cb_exitConfigMode( void ) {
  log_debug(F("\n[WiFiManager] exit config mode --> save addon parameters flag is set ..."));
  _WMsaveAddonConfigFlag = true;
}

/*
 * WiFiManager : callback upon entering AP mode (will start to configure network)
 */
void cb_enterConfigMode( WiFiManager *myWiFiManager ) {
  log_debug(F("\n[WiFiManager] entering WiFi AP config mode"));
  log_debug(F("\n\tIP ")); log_debug(WiFi.softAPIP());
  log_debug(F("\n\tSSID ")); log_debug(myWiFiManager->getConfigPortalSSID());
  log_flush();
}

/*
 * WiFi setup
 */
bool setupWiFi( wifiParametersMgt *wp ) {

  if( _need2reboot ) return false;
  
  log_info(F("\n[WiFi] start WiFiManager ..."));

  // reload config file
  if( wp ) {
    // load WiFi parameters and options
    wp->loadConfigFile();
  }

  // WiFi parameters availables ?
  if( strlen(wp->getWIFIssid()) ) {

    log_debug(F("\n[WiFi] found ssid = ")); log_debug(wp->getWIFIssid());
    log_debug(F("\n[WiFi] found pass = ")); log_debug(wp->getWIFIpass());

    // destroy saved parameters
    WiFi.mode(WIFI_STA);
    WiFi.disconnect(true);  // or WiFiManager.resetSettings();
    delay(1000);
    log_debug(F("\n[WiFi] start to reconnect using those credentials ..."));log_flush();
    WiFi.begin( wp->getWIFIssid(), wp->getWIFIpass() );
    
    // retry for xx seconds using previously retrieved credentials
    uint8_t _retry=WM_CONNECTION_ATTEMPT_TIMEOUT;
    while( WiFi.status() != WL_CONNECTED and _retry-- ) {
      delay(1000);
      log_debug(F("+"));log_flush();
    }
  }

  // WiFiManager local instance ... no need to keep once business is done
  WiFiManager wifiManager;
  
  // debug messages
  wifiManager.setDebugOutput( true );
  
  // set enter config mode callback
  wifiManager.setAPCallback( cb_enterConfigMode );

  // set config save notify callback: only usefull for additional parameters
  wifiManager.setSaveConfigCallback( cb_exitConfigMode );

  // set minimum signal level
  wifiManager.setMinimumSignalQuality();      // min. default is 8%

  // set custom ip for AP portal
  //wifiManager.setAPStaticIPConfig(IPAddress(10,0,1,1), IPAddress(10,0,1,1), IPAddress(255,255,255,0));

  // set to exit portal after config
  wifiManager.setBreakAfterConfig( true );



  /*
   * Declare WiFiManager additional parameters
   * 
   * [Jun.18] WARNING --> use Ken Taylor WiFiManager verion:
   *    https://github.com/kentaylor/WiFiManager
   * 
   * - hint reminder
   * - TM1637 7 segment display with optional pins
   * 
   * Note: HACK related below is because i don't want to allocate memory on the heap
   * ... because you ought to free it at the end and it is much more secure to allocate 
   * on the stack ;)
   */
  // Just a quick hint
  WiFiManagerParameter p_hint("<small>*Hint: if you want to reuse the currently active WiFi credentials, leave SSID and Password fields empty</small>");

  // kinds of custom HTML
  const char _customHtml_checkbox[]         = "type=\"checkbox\"";
  const char _customHtml_checkbox_checked[] = "type=\"checkbox\" checked";
  
  // enable / disable sensOCampus sandbox
  WiFiManagerParameter p_sandbox("sandbox", "neOCampus sandbox", "T", 2, _customHtml_checkbox, WFM_LABEL_AFTER);
  if( wp->isEnabledSandbox() ) {
    // [Jun.18] HACK that requires modification in WiFiManager
    p_sandbox._customHTML = _customHtml_checkbox_checked;
  }
  
  // TM1637 7 segment display parameters
  WiFiManagerParameter p_sevenSegTM1637("sevenSegTM1637", "TM1637 7-seg (DIO=2,CLK=SCL)", "T", 2, _customHtml_checkbox, WFM_LABEL_AFTER);
  if( wp->isEnabled7segTM1637() ) {
    // [Jun.18] HACK that requires modification in WiFiManager
    p_sevenSegTM1637._customHTML = _customHtml_checkbox_checked;
  }
  
  // PIR sensor
  WiFiManagerParameter p_PIRsensor("PIRsensor", "PIR sensor (pin=5)", "T", 2, _customHtml_checkbox, WFM_LABEL_AFTER);
  if( wp->isEnabledPIR() ) {
    // [Jun.18] HACK that requires modification in WiFiManager
    p_PIRsensor._customHTML = _customHtml_checkbox_checked;
  }

  
  /*
   *WARNING: Clear sensor ALL CONFIG (wifi, spiffs) option
   */
  WiFiManagerParameter p_eraseALL("eraseALL", "ERASE ALL", "T", 2, _customHtml_checkbox, WFM_LABEL_AFTER);
  
  
  /*
   * add all parameters to wifiManager
   */
  wifiManager.addParameter(&p_hint);
  wifiManager.addParameter(&p_sandbox);
  wifiManager.addParameter(&p_sevenSegTM1637);
/*
  wifiManager.addParameter(&p_pinTM1637dio);
  wifiManager.addParameter(&p_pinTM1637clk);
*/
  wifiManager.addParameter(&p_PIRsensor);
  wifiManager.addParameter(&p_eraseALL);

  
  //
  // set timeouts
  if( WiFi.status() != WL_CONNECTED ) {
    log_debug(F("\n[WiFi] AP will get open for "));log_debug(WM_CONFIG_PORTAL_TIMEOUT,DEC);log_debug(F(" seconds"));
    wifiManager.setConfigPortalTimeout(WM_CONFIG_PORTAL_TIMEOUT);
  }
  else {
    log_debug(F("\n[WiFi] AP opened in CONFIG MODE for ")); log_debug(WM_CONFIG_PORTAL_TIMEOUT/6,DEC);log_debug(F(" seconds"));
    wifiManager.setConfigPortalTimeout(WM_CONFIG_PORTAL_TIMEOUT/6);
  }
  // set a timeout trying to connect to a specified AP
  log_debug(F("\n[WiFi] will try a connexion for maximum "));log_debug(WM_CONNECTION_ATTEMPT_TIMEOUT,DEC);log_debug(F(" seconds"));
  wifiManager.setConnectTimeout(WM_CONNECTION_ATTEMPT_TIMEOUT);

  
  /*
   * if it does not connect it starts an access point with the specified name
   * and goes into a blocking loop awaiting configuration
   */
  log_debug(F("\n"));
  wifiManager.startConfigPortal( getAPname(), WIFI_AP_CREDENTIALS ); // blocking call
  delay(500);
  
  // whatever the result of the connection ... we save all parameters :)
  // Getting posted form values and overriding local variables parameters
  // Config file is written regardless the connection state
  // TODO: find a way to parse ALL parameters added in wifi manager
  if( _WMsaveAddonConfigFlag ) {
    
    // save wifi settings to wifi parameters object
    wp->_getWIFIsettings();
    
    // save OPTIONS to parameters object
    wp->_setopt_sandboxMode( strncmp(p_sandbox.getValue(),"T",1)==0 ? true : false );
    wp->_setopt_7segTM1637( strncmp(p_sevenSegTM1637.getValue(),"T",1)==0 ? true : false );
    wp->_setopt_PIRsensor( strncmp(p_PIRsensor.getValue(),"T",1)==0 ? true : false );

    /*
    strcpy(thingspeakApiKey, p_thingspeakApiKey.getValue());
    sensorDht22 = (strncmp(p_sensorDht22.getValue(), "T", 1) == 0);
    pinSda = atoi(p_pinSda.getValue());
    pinScl = atoi(p_pinScl.getValue());
    */
    
    // Dangerous option !!
    wp->_setopt_eraseALL( strncmp(p_eraseALL.getValue(),"T",1)==0 ? true : false );
    
    // Writing JSON config file to flash for next boot
    wp->saveConfigFile();
  }

  if( WiFi.status() != WL_CONNECTED ) {
    log_error(F("\n[WiFi] failed to connect and hit timeout ... restart"));
    // reboot our sensor
    _need2reboot=true;
    delay(1000);
    return false;
  }
 
  //if you get here you have connected to the WiFi
  log_info(F("\n[WiFi] connected :)"));
  log_debug(F("\n\tSSID ")); log_debug(WiFi.SSID());
  log_debug(F("\n\tIP ")); log_debug(WiFi.localIP());
  log_debug(F("\n\tRSSI ")); log_debug(WiFi.RSSI());

  // well done :)
  return true;
}


/*
 * setup SPIFFS
 */
bool setupSPIFFS( void ) {
  log_info(F("\n[SPIFFS] start ... "));
  if( !SPIFFS.begin() ) {
    log_info(F("\n[SPIFFS] uninitialized file system ... format :|"));
    delay(1000);
    return formatSPIFFS();
  }
  else {
    log_info(F("\n[SPIFFS] mounted file system :)"));
  }
  return true;
}



// --- utilities functions ----------------------------------------------------
// hex dump of a buffer
void hex_dump( const char *buf, size_t bufsize, bool ascii ) {
  log_debug(F("\n### HEX DUMP "));log_debug(bufsize,DEC);log_debug(F(" bytes ###")); log_flush();
  uint8_t max_lines = bufsize/16;
  max_lines = ( max_lines==0 ? 1 : max_lines );
  for( uint8_t l=0; l < max_lines; l++ ) {
    log_debug(F("\n[0x"));log_debug(l,HEX);log_debug(F("]"));
    for( uint8_t r=0; r+l*16 < (uint8_t)bufsize; r++ ) {
      char _byte[6];
      snprintf(_byte, sizeof(_byte)," 0x%02X", buf[l*16+r]);
      log_debug(_byte);
    }
    log_flush();
  }
  for( uint8_t i; i < (uint8_t)bufsize and ascii; i++ ) {
    char _line[32];
    snprintf(_line,sizeof(_line),"\nbuf[%u] = 0x%02x '%c'", i, buf[i], buf[i]);
    log_debug(_line); delay(20); log_flush();
  }
}


/* --- 
 * --- Format SPIFFS filesystem
 */
bool formatSPIFFS( void ) {
  log_info(F("\n[SPIFFS] start formatting ... "));
  bool _res = SPIFFS.format();
  if( _res==true ) {
    log_debug(F("success :)"));
  } else log_error(F("[FAILURE] !!"));
  log_flush();
  return _res;
}


// retrieve mac address
const char *getMacAddress( void ) {
  static bool _initialized = false;
  // mac address array format
  static mac_addr_t _mac;
  // mac address string format
  static mac_str_t _mac_sta;

  if( _initialized == false ) {
    //WiFi.softAPmacAddress( _mac );  // beware that AP mac addr is different from STA mac addr!
    WiFi.macAddress( _mac );
    snprintf(_mac_sta, sizeof(_mac_sta), "%02x:%02x:%02x:%02x:%02x:%02x", _mac[0], _mac[1], _mac[2], _mac[3], _mac[4], _mac[5]);
    _initialized = true;
  }
  return (const char *)_mac_sta;
}



// retrieve current domain name
const char *getDomainName( void ) {
  //static char _domainName[32] = "";
  
  if( ! WiFi.isConnected() ) return NULL;
    
  // esp8266 affects a default hostname if not set by DHCP server
  const char *_esp_defl_hostname = PSTR("ESP_");
#ifdef ESP8266
  const char *cur_hostname = WiFi.hostname().c_str();
#elif defined(ESP32)
  const char *cur_hostname = WiFi.getHostname();
#endif
  
  /* is hostname one of the defaults esp8266 hostname ?
   * i.e it starts with ESP_XXXXXX */
  if( not strncmp_P( cur_hostname, _esp_defl_hostname, strlen_P(_esp_defl_hostname) ) ) {
    log_debug(F("\n[getDomainName] hostname is a default one: ")); log_debug(cur_hostname); log_flush();

    /*
     * [Jun.18] DHCP_OPTION_HOSTNAME does not exist in dhcp.c (client)
     * neither DHCP_OPTION_IDX_HOSTNAME
     * TODO!!
     */

    return nullptr;
  }
  
  /* Does hostname contains domain name too ?
   * e.g esp8266-bu-hall.neocampus.univ-tlse3.fr */
  // lets find first dot
  const char *cur_domain = strchr( cur_hostname, '.' );
  if( cur_domain == NULL ) return nullptr;
  // ... and at least a second dot ?
  if( strchr( cur_domain+1, '.' ) == NULL ) return nullptr;
  
  // ok we finally found a hostname with AT LEAST 2 dots ==> FQDN
  return cur_domain+1;
}


/*
 * Get current time
 * WARNING: static variable inside (i.e single thread)
 */
const char *getCurTime( const char *fmt ) {

  // STATIC variable !
  static char _tmpbuf[64];
  
  // default format
  const char defl_fmt[] = "%Y-%m-%d %H:%M:%S %z";
  
  if( !fmt ) fmt=defl_fmt;

  struct tm *_tm;
  time_t _curTime;
  
  time( &_curTime );
  _tm = localtime( &_curTime );   // Weird part ... localtime function *corrects* time to match timezone ... :s
  strftime( _tmpbuf, sizeof(_tmpbuf), fmt, _tm);

  return (const char *)_tmpbuf;
}


/*
 * low-level HTTP GET
 */
bool _http_get( const char *url, char *buf, size_t bufsize, const char *login, const char *passwd ) {
    
  HTTPClient http;

  http.begin(url);
  log_debug(F("\n[HTTP] GET url : ")); log_debug(url); log_flush();
  
  // authentication ?
  if( login!=NULL and passwd!=NULL ) {
    http.setAuthorization( login, passwd);
  }
  
  // perform GET
  int httpCode = http.GET();

  // connexion failed to server ?
  if( httpCode < 0 ) {
    log_error(F("\n[HTTP] connexion error code : ")); log_debug(httpCode,DEC); log_flush();
    _need2reboot = true;
    return false;
  }
  
  // check for code 200
  if( httpCode == HTTP_CODE_OK ) {
    String payload = http.getString();
    snprintf( buf, bufsize, "%s", payload.c_str() );
  }
  else {
    log_error(F("\n[HTTP] GET retcode : ")); log_debug(httpCode,DEC); log_flush();
  }
  
  // close connexion established with server
  http.end();

  yield();
  
  return ( httpCode == HTTP_CODE_OK );
}

// ---
// HTTP get
bool http_get( const char *url, char buf[], size_t bufsize ) {
    return _http_get( url, buf, bufsize, NULL, NULL );
}

// ---
// HTTP get with credentials
bool http_get( const char *url, char *buf, size_t bufsize, const char *login, const char *passwd ) {
    return _http_get( url, buf, bufsize, login, passwd );
}

// ---
// Reset configuration and restart (delete module's config files and reboot)
bool neOSensor_reset( void ) {

  log_info(F("\n[reset] neOSensor reset procedure will clean your FS and then reboot ..."));
  
  /* 
   * Delete ALL modules JSON config files
   * 
   * Note: it works because modules' config files are ONLY WRITTEN
   * upon config change ... not at the end of the module's life ;)
   */
  const char *_prefix = PSTR(MCFG_FILE_PREFIX);
  const char *_suffix = PSTR(MCFG_FILE_SUFFIX);
#ifdef ESP8266
#warning "code not tested!"
  Dir dir = SPIFFS.openDir("/");
  while( dir.next() ) {
    log_debug(F("\n[reset] found file: "));log_debug(dir.fileName());log_flush();
    if( not strstr_P( dir.fileName().c_str(), _prefix ) ) continue;
    if( not strstr_P( dir.fileName().c_str(), _suffix ) ) continue;
    // delete matching file
    log_debug(F("\n[reset] deleting file : "));log_debug(dir.fileName());log_flush();
    if( !SPIFFS.remove(dir.filename()) ) {
      log_warning(F("\n[reset] failed to delete file: "));log_warning(dir.fileName());log_flush();
    }
  }
#elif defined(ESP32)
#warning "code not tested!"
  File root = SPIFFS.open("/");
  if( root and root.isDirectory() ) {
    File file;
    while( file=root.openNextFile() ) {
      log_debug(F("\n[reset] found file: "));log_debug(file.name());log_flush();
      if( strstr_P( file.name(), _prefix ) ) continue;
      if( strstr_P( file.name(), _suffix ) ) continue;
      // delete matching file
      log_debug(F("\n[reset] deleting file : "));log_debug(file.name());log_flush();
      if( !SPIFFS.remove(file.name()) ) {
        log_warning(F("\n[reset] failed to delete file: "));log_warning(file.name());log_flush();
      }
    }
  }
  else {
    log_error(F("\n[reset] failed to open ROOT directory ?!?!")); log_flush();
  }
#endif  
  // reboot
  neOSensor_reboot();
  
  // unreachable zone
  return true;
}

// ---
// proper restart
void neOSensor_reboot( void ) {
  
  log_info(F("\n[reboot] neOSensor is about to reboot in a few seconds ..."));
  
  // properly end stuffs (e.g eeprom)

  // SPIFFS close
  SPIFFS.end();
  
  // call to restart
  delay( 2000 + int(random(10000)) ); // random reboot delay
  ESP.restart();
  delay(5000);
}

