/* -----------------------------------------------------------------------------
 *
 * neOCampus operation
 * 
 * neOSensor firmware for various ESP8266 and ESP32 based boards:
 * - neoSensor boards based on either esp32 or esp8266
 * - neOSensor-Airquality board based on esp32
 * 
 * (c) F.Thiebolt / neOCampus operation - Université Toulouse 3
 * 
 * ---
 * NOTES:
 * - you need to 'deploy' our boards definitions (run the deploy.sh script)
 * - select your board from the Arduino IDE boards menu (located end of list)
 * - a compilation flag tells which bord it is (i.e NEOSENSOR_AIRQUALITY)
 * - as of aug.20, CONFIG_LWIP_MAX_ACTIVE_TCP=16
 * 
 * ---
 * KNOWN ISSUES:
 * - ONLY ONE SHT3x per neOSensor supported.
 *    This stems from the fact that the SHT3x driver features static variables 
 *    shared across all instances.
 *
 * ---
 * TODO:
 * - esp8266 now features a configTzTime() in newer API
 * - loadSensoConfig --> avoid data duplication, implement an iterator
 * - check sntp!=IPADDR_ANY works with ESP8266 (line 400)
 * - remove DISABLE_SSL compilation flag
 * - as the number of modules is increasing, implement a list of modules in the setup()
 * ---
 * F.Thiebolt   nov.21  corrected timezone definition for esp32
 * F.Thiebolt   sep.21  added display module support (e.g oled or 7segment displays)
 * F.Thiebolt   aug.21  added digital inputs support (e;g PIR sensor)
 *                      added support for shared JSON document for data exchange 
 *                      between modules
 * F.Thiebolt   aug.20  initial port from neOSensor based on ESP8266
 *                      added esp32 adc calibration support
 *                      setupNTP called AFTER network setup
 * ----------------------------------------------------------------------------- */



/* #############################################################################
 * ###                                                                       ###
 * ###             ARDUINO'S IDE BOARD HARDWARE SELECTION                    ###
 * ###                                                                       ###
 * 
 *      Proper board will be selected according to the selection you made 
 *      within the Arduino board selector
 * 
 * >>>     Don't forget to update BOARD_FWREV from your 'board/neOXxx.h'     <<<
 * 
 * ############################################################################# */



/*
 * Includes
 */
#ifdef ESP8266
  /* specify timezone; daylight will automatically follow :) */
  #include <TZ.h>
#endif

#include <Arduino.h>
#include <Wire.h>
#include <Ticker.h>
#if defined(ESP8266)
  #include <ESP8266WiFi.h>
#elif defined(ESP32)
  #include <WiFi.h>
#endif
#include <lwipopts.h>                     // for ESP32 max tcp connections CONFIG_LWIP_MAX_ACTIVE_TCP

/* As of esp8266 arduino lib >=2.4.0, time is managed via local or sntp along with TZ support :) */ 
#include <time.h>                         // time() ctime()
#include <sntp.h>
#ifdef ESP8266
  #include <coredecls.h>                  // settimeofday_cb(), tune_timeshift64()
#endif

/*
 * ESP8266 advanced ops
 */
#if defined ESP8266
  extern "C" {
    #include "user_interface.h"           // wifi_station_dhcpc_start, ...
  }
  extern "C" {
    #include "espconn.h"                  // espconn_tcp_set_max_con() set maximum number of simultaneous TCP connexions
  }
#endif /* ESP8266 */

/*
 * ESP32 advanced ops:
 * - enhanced ADC (calibration)
 */
#if defined(ESP32) && !defined(DISABLE_ADC_CAL)
  #include <esp_adc_cal.h>
  #define DEFL_ESP32_ADC_VREF       1100  // default 1100mv thay will get used ONLY if efuse vref is not set (i.e uncalibrated esp32 ---before Q1-18)
  esp_adc_cal_value_t esp_adc_cal_src       = (esp_adc_cal_value_t)(-1);
  esp_adc_cal_characteristics_t *adc_chars  = new esp_adc_cal_characteristics_t;
#endif /* ESP32 adcanced ADC */



/* neOCampus related includes */
#include "neocampus.h"

#include "neocampus_debug.h"
#include "neocampus_utils.h"
#include "neocampus_i2c.h"
#include "sensocampus.h"
#include "neocampus_OTA.h"
//#include "neocampus_comm.h"               // future MQTT(s) comm module: a single MQTTclient shared with multiple subscribers featuring different callbacks

/* neOCampus modules
 * !! WARNING !!
 * Each module makes use of a TCP connection to the neOCampus server (MQTT link)
 * check that there's enough TCP links available !!
 */
#include "device.h"
#include "temperature.h"
#include "humidity.h"
#include "luminosity.h"
#include "noise.h"
#include "neoclock.h"
#include "airquality.h"
#include "digital.h"
#include "display.h"

// modules management
#include "modulesMgt.h"

// WiFi parameters management
#include "wifiParametersMgt.h"



/*
 * Definitions
*/

// Debug related definitions
#define SERIAL_BAUDRATE   115200
#ifdef ESP8266
  #if (LOG_LEVEL > 0) || defined(DEBUG_WIFI_MANAGER)
    #include <SoftwareSerial.h>
  #endif
#endif /* ESP8266 */

// sensOCampus related definitions
#define SENSO_MAX_RETRIES         5   // maximum number of sensOCampus configuration retrieval retries

// Time related definitions
#ifdef ESP8266
  #define MYTZ                    TZ_Europe_Paris
#elif defined(ESP32)
  //#define MYTZ                    "Europe/Paris" [nov.21] why is it not working ?!?!
  #define MYTZ                    "CET-1CEST,M3.5.0/2,M10.5.0/3"
  //const long  gmtOffset_sec = 3600; [nov.21] daylight changing does not work
  //const int   daylightOffset_sec = 3600;
#endif
#define NTP_DEFAULT_SERVER1       "pool.ntp.org"      // DNS location aware
//#define NTP_DEFAULT_SERVER1       "time.nist.gov"     // DNS location aware
//#define NTP_DEFAULT_SERVER2       "0.fr.pool.ntp.org" // DNS location aware
#define NTP_DEFAULT_SERVER2       "ntp.irit.fr"       // UT3-IRIT specific
#define NTP_DEFAULT_SERVER3       "ntp.univ-tlse3.fr" // UT3 specific


/*
 * various led mods to express status of neOSensor
 * device while in setup()
 */
#ifdef ESP8266
  // since 3.X sdk, default range of analogWrite is now 255 while it used to be 1023
  #define ESP8266_PWMRANGE    1023
#endif
typedef enum {
  WIFI,
  DISABLE
} enum_ledmode_t;


/*
 * Global variables
 */
bool _need2reboot = false;              // flag to tell a reboot is requested

// WiFi parameters management statically allocated instance
wifiParametersMgt wifiParameters = wifiParametersMgt();

// sensOCampus statically allocated instance with link to global wifi parameters (to check against sandbox mode)
senso sensocampus = senso( &wifiParameters );

// modules management statically allocated instance
modulesMgt modulesList = modulesMgt();

// device statically allocated instance
device deviceModule = device();

// temperature class module
temperature *temperatureModule      = nullptr;

// humidity class module
humidity *humidityModule            = nullptr;

// luminosity class module
luminosity *luminosityModule        = nullptr;

// noise class module
noise *noiseModule                  = nullptr;
// noise module ISR :(
void IRAM_ATTR noiseDetectISR() {             /* https://community.particle.io/t/cpp-attachinterrupt-to-class-function-help-solved/5147/2 */
  if( noiseModule ) noiseModule->noiseDetectISR();
}

// neoclock class module
neoclock *clockModule               = nullptr;

// airquality class module
airquality *airqualityModule        = nullptr;

// digital class module
digital *digitalModule              = nullptr;

// display class module
display *displayModule              = nullptr;


// time server related
bool cbtime_set = false;
bool _cbtime_call = false;          // used to display things about time sync
time_t cbtime_cur, cbtime_prev;     // time set in callback


/*
 * [aug.21] shared JSON document to allow modules to exchange data.
 * ... mainly used by display module ;)
 */
#define MODULES_SHARED_JSON_SIZE  512
//StaticJsonDocument<MODULES_SHARED_JSON_SIZE> sharedRoot;    // stack allocation
DynamicJsonDocument sharedRoot(MODULES_SHARED_JSON_SIZE);   // heap allocation


// --- Functions ---------------------------------------------------------------
void setupSerial( void ) {
#ifdef SERIAL_BAUDRATE
  delay(3000);  // time for USB serial link to come up anew
  Serial.begin(SERIAL_BAUDRATE); // Start serial for output

  // Arduino libs v2.4.1, to enable printf and debug messages output
  Serial.setDebugOutput( true );
  
  char tmp[96];
  snprintf(tmp,sizeof(tmp),"\n\n\n\n# %s firmware rev %d for neOCampus is starting ... ",getBoardName(),getFirmwareRev());
  log_info(tmp);
  log_info(F("\n#\tMac address is ")); log_info(getMacAddress());
  log_info(F("\n#\tlog level is ")); log_info(LOG_LEVEL,DEC);
  log_info(F("\n"));
  log_flush();
#endif
#ifndef DEBUG_WIFI_MANAGER
  wifiManager.setDebugOutput(false);
#endif
}



// ---
// Setup system led (mostly led2 on ESP8266) i.e blue led on ESP12E/F near antenna
// we then use serial link1 used to either core debugging or just to blink led2
void setupSysLed( void ) {
#if defined(ESP8266) && defined(SYS_LED)
  if( SYS_LED != INVALID_GPIO ) {
    // ESP8266's GPIO2 is tied to Serial1's TX
    Serial1.begin( 115200, (SerialConfig)SERIAL_8N1, (SerialMode)UART_TX_ONLY, SYS_LED);
  
    // Arduino libs v2.4.1, to enable printf and debug messages output
    // Serial1.setDebugOutput( true ); // default
    return;
  }
#elif defined(ESP32) && defined(SYS_LED)
  if( SYS_LED != INVALID_GPIO ) {  
    pinMode(SYS_LED, OUTPUT);
    return;
  }
#endif
  log_info(F("\nNO SYS_LED defined.")); log_flush();
}


// ---
// clear Sensors WiFi, EEPROM, config files (SPIFFS) ...
// ... called upon clear SWITCH button activated at startup
void clearSensor( void ) {
  log_debug(F("\n[CLEAR] starting CLEAR procedure ..."));

  // clear WiFi credentials
  WiFi.disconnect();

  // clear JSON config files (SPIFFS)
  formatSPIFFS();
}


// ---
// I2C setup pins and frequency
bool setupI2C( void ) {

  if( _need2reboot ) return false;

  // if not defined SDA and SCL ... give up
  if( SDA==INVALID_GPIO or SCL==INVALID_GPIO ) {
    log_info(F("\nI2C bus disabled.")); log_flush();
    return false;
  }
  
  log_info(F("\n[i2c] start setup ..."));
  Wire.begin(SDA,SCL);
  #ifdef I2C_FREQ
    log_debug(F("\nI2C frequency set to "));log_debug(I2C_FREQ/1000,DEC); log_debug("kHz"); log_flush();
    Wire.setClock(I2C_FREQ);
  #else
    log_debug(F("\nI2C frequency set to default 100kHz")); log_flush();
  #endif
  return true;
}


// ---
// Blink system led
inline void blinkSysLed( void ) {
#ifdef ESP8266
  /* ESP8266's onboard blue led (GPIO2) is tied to Serial1:
    * send zero(s) to blink the led
    */
  byte _msg[] = {0x00, 0x00, 0x00, 0x00 };
  Serial1.write(_msg, sizeof(_msg));Serial1.flush();
#elif defined(ESP32)
  /* ESP32 does not feature the blue led like in ESP12E/F modules
   * ==> SYS_LED not defined ==> use LED (if defined and if no NoiseModule)
   */
  uint8_t _led = SYS_LED;
  if( _led==INVALID_GPIO && LED!=INVALID_GPIO && noiseModule==nullptr ) _led = LED;
  if( _led != INVALID_GPIO ) {
    digitalWrite( _led, HIGH);
    delay(50);
    digitalWrite( _led, LOW);
  }
#endif
}


// ---
// process end of main loop: specific functions executed every seconds
void endLoop( void ) {
  static unsigned long _lastCheck = 0;    // elapsed ms since last check

#if 0
  // ONY FOR DEBUGGING
  static unsigned long _lastJSONdisplay = 0;    // elapsed ms since last displying shared JSON
  // 90s second elapsed ?
  if( ((millis() - _lastJSONdisplay) >= (unsigned long)90*1000UL) == true ) {
    _lastJSONdisplay = millis();
    log_debug(F("\nGlobal sharedJSON:\n")); log_flush();
    serializeJsonPretty( sharedRoot, Serial );
  }
#endif /* 0 */

  // check if a reboot has been requested ...
  if( _need2reboot ) {
    log_info(F("\n[REBOOT] a reboot has been asked ..."));log_flush();
    #ifndef DISABLE_MODULES
    modulesList.stopAll();
    #endif
    neOSensor_reboot();
    delay(5000); // to avoid an infinite loop
  }

  // a second elapsed ?
  if( ((millis() - _lastCheck) >= (unsigned long)1000UL) == true ) {
    _lastCheck = millis();

    /* blink SYS_LED */
    blinkSysLed();

    // check Heap available
    if( ESP.getFreeHeap() < 4096 ) {
      log_error(F("\n[SYS] CRTICAL free heap very low!!!")); log_flush();
    }

    // display NTP servers used for time sync
    if( _cbtime_call ) {
      _cbtime_call = false;
      
      // list active ntp servers
      for( uint8_t i=0; i<SNTP_MAX_SERVERS; i++ ) {
        IPAddress sntp = *(IPAddress *)sntp_getserver(i);
        const char* sntp_name = sntp_getservername(i);
        // check if address is valid
#ifdef ESP8266
        if( sntp.isSet() ) {
#elif defined(ESP32)
        if( sntp != IPADDR_ANY ) {
#endif
          log_debugF("\n[NTP][%d]:     ", i);
          if( sntp_name ) {
            log_debugF("%s (%s) ", sntp_name, sntp.toString().c_str());
          } else {
            log_debugF("%s ", sntp.toString().c_str());
          }
        }
      }      
    }

    // serial link activity marker ...
    log_debug(F("."));
  }
}


// --- LED MODES while in setup()
// ledWiFiMode, i.e fade-in / fade-out constantly
// input is either the led_pin (esp8266)
// or the channel_id (esp32)
void _ledWiFiMode( uint8_t id ) {

#ifdef ESP8266

  const uint8_t _val_steps = 50;
  static int16_t _val=0;
  
  if( _val < ESP8266_PWMRANGE ) _val+=_val_steps;
  else _val=-ESP8266_PWMRANGE;

  analogWrite( id, (abs(_val)<ESP8266_PWMRANGE) ? abs(_val) : ESP8266_PWMRANGE );

#elif defined(ESP32)
  /* Ok, we'll get called every 50ms ... 
   *  we want fade_in across 1s and fade_out across another 1s
   *  ==> 20 steps for each fade_
   *  Channel resolution being 10bits ==> 1024 / 20 = #50
   *  ... thus each steo will increase/decrese duty by this step
   */
  const uint8_t _val_steps = (pow(2,LED_RESOLUTION)-1) / 20;
  static int32_t _duty = 0;

  if( _duty < pow(2,LED_RESOLUTION) ) _duty+=_val_steps;
  else _duty = -pow(2,LED_RESOLUTION);

  ledcWrite( id, (abs(_duty)<(pow(2,LED_RESOLUTION)-1)) ? abs(_duty) : pow(2,LED_RESOLUTION)-1 );
#endif
}


// various led modes to express neOSensor status while in setup()
void setupLed( uint8_t led, enum_ledmode_t led_mode ) {

  if( led == INVALID_GPIO ) return;

  log_debug(F("\n[setupLed] starting to setup led '"));log_debug(led,DEC);
  log_debug(F("' with mode: "));log_debug(led_mode,DEC);
  log_flush();
  
  static Ticker timer_led;

  switch( led_mode ) {
    case WIFI:
      // set led to show we're waiting for WiFi connect
#ifdef ESP8266
      pinMode(led,OUTPUT);
      analogWriteRange(ESP8266_PWMRANGE);
      timer_led.attach_ms(50, _ledWiFiMode, led);
#elif defined(ESP32)
      ledcSetup(LED_CHANNEL, LED_BASE_FREQ, LED_RESOLUTION);
      ledcAttachPin( led, LED_CHANNEL);
      timer_led.attach_ms(50, _ledWiFiMode, LED_CHANNEL);
#endif
      break;

    case DISABLE:
      // switch back to INPUT mode (default after reset)
      timer_led.detach();
#ifdef ESP8266
      analogWrite( led, 0 );  // [Dec.17] workaround for resilient analog value written to led
#elif defined(ESP32)
      ledcDetachPin( led );
#endif
      pinMode(led,INPUT);
      break;

    default:
      log_error(F("\n[setupLed] unknwown led_mode ?!?!"));
  }

  return;
}


// ---
/* sync ntp callback:
 *  This function gets called whenever a NTP sync occurs
 */
#ifdef ESP32
void _syncNTP_cb(struct timeval *tv) {
  // tv arg is time from NTP server
  syncNTP_cb();
}
#endif
void syncNTP_cb( void ) {
  char _tmpbuf[64];
  struct tm *_tm;

  /* WARNING
   *  - gettimeofday --> TZ settings does not apply
   *  - time() while incorrect will get masked by localtime behaviour :s
   */
   
  // retrieve when time sync occured
  // gettimeofday( &cbtime_cur, NULL ); // WARNING: timezone does not apply ... :(
  // log_debug(F("\n[NTP] time sync occured ")); log_debug( ctime(&cbtime_cur.tv_sec) );
  time( &cbtime_cur );
  _tm = localtime( &cbtime_cur );   // Weird part ... localtime function *corrects* time to match timezone ... :s

  
  log_info(F("\n[NTP] sync CALLBACK ... ")); log_flush();

  /* Note: while in this callback, the ntp server
   *  is not yet written in the list of NTP servers!
  */

  // display time synchro message
  strftime( _tmpbuf, sizeof(_tmpbuf), "%Y-%m-%d %H:%M:%S %z", _tm);
  log_debug(F("\n[NTP] time sync occured ")); log_debug(_tmpbuf);

  // first call ?
  if( not cbtime_set ) {
    cbtime_set = true;
  }
  else {
    int16_t _sync_diff = cbtime_cur - ( cbtime_prev + (SNTP_UPDATE_DELAY/1000) ); // seconds
    if( abs(_sync_diff) <= 2 ) {
      log_info(F(" local clock well syncronized :)"));
    }
    else if( abs(_sync_diff) <= 60 ) {
      log_warning(F(" local clock shift(seconds)= "));log_info(_sync_diff,DEC);
    }
    else {
      log_debug(F(" local clock shift > 60s ... probably a WiFi resync ..."));
    }
  }
  log_flush();

  cbtime_prev = cbtime_cur;

  // callback called
  _cbtime_call = true;
}


// ---
// NTP setup
bool setupNTP( void ) {
  
  if( _need2reboot ) return false;

  log_debug(F("\n[NTP] start setup of (S)NTP ..."));

#ifdef ESP8266
  // register ntp sync callback
  settimeofday_cb( syncNTP_cb );
  // [may.20] as default, NTP server is provided by the DHCP server
  configTime( MYTZ, NTP_DEFAULT_SERVER1, NTP_DEFAULT_SERVER2, NTP_DEFAULT_SERVER3 );
#elif ESP32
  // register ntp sync callback
  sntp_set_time_sync_notification_cb( _syncNTP_cb );
  // [nov.21] now using timezone definition :)
  configTzTime( MYTZ, NTP_DEFAULT_SERVER1, NTP_DEFAULT_SERVER2, NTP_DEFAULT_SERVER3 );
  // [may.20] as default, NTP server is provided by the DHCP server
  // configTime(gmtOffset_sec, daylightOffset_sec, NTP_DEFAULT_SERVER1, NTP_DEFAULT_SERVER2, NTP_DEFAULT_SERVER3 );
#endif    

  log_flush();
  // the end ...
  return true;
}


// ---
// process WiFi Parameters options
void processWIFIparameters( wifiParametersMgt *wp=nullptr ) {
  
  if( !wp ) return;

  log_debug(F("\n[wifiParams] start to process options ... ")); log_flush();

  //
  // check for ERASE ALL OPTION
  if( wp->isEnabledEraseALL() ) {
    log_info(F("\n[wifiParams] a WHOLE ERASE has been requested ...")); log_flush();
    clearSensor();
    log_debug(F("\n\t... and asking for reboot ..."));
    delay(250);
    _need2reboot = true;  // set flag for reboot
    // and no need to go further ...
    return;
  }

  //
  // TM1637 display
  if( wp->isEnabled7segTM1637() and !clockModule ) {
    log_info(F("\n[wifiParams] start 7 segments neoclock (TM1637 driven) ...")); log_flush();
    
    // allocate display with TM1637 7 segment driver
    clockModule = new neoclock();
    clockModule->add_display( NEOCLOCK_TM1637 );    
  }
  else if( !wp->isEnabled7segTM1637() and clockModule ) {
    log_info(F("\n[wifiParams] removing 7 segments neoclock (TM1637 driven) ...")); log_flush();

    // stop and de-allocate object
    clockModule->stop();
    free( clockModule );
    clockModule = nullptr;
  }
 
  //
  // PIR sensor
  if( wp->isEnabledPIR() and !digitalModule ) {
    log_info(F("\n[wifiParams] start PIR sensor ...")); log_flush();
    
    // allocate digital module
    digitalModule = new digital();
    digitalModule->add_gpio( "IRsensor", PIR_SENSOR, digitalInputType_t::presence, digitalFrontDetect_t::rising, 60 );  // 60s cooldown
  }


  /*
   * Add processing for additional options here !
   */
}


// ---
// earlySetup: called at the very begining of setup()
void earlySetup( void ) {

  // note: no log_xxx msg since they will get defined later
  // log_info(F("\n[Early] disable autoConnect and clear saved IP ...")); log_flush();

#if defined(MAX_TCP_CONNECTIONS) && defined(ESP8266)
  // set maximum number of simultaneous TCP connexions 
  // log_info(F("\n[Early] set max TCP concurrent sockets to ")); log_info(MAX_TCP_CONNECTIONS, DEC); log_flush();
  espconn_tcp_set_max_con( MAX_TCP_CONNECTIONS );
#endif /* MAX_TCP_CONNECTIONS */

  // WiFi.disconnect(true); // to erase default credentials
  WiFi.setAutoReconnect(false);

  // Disable sleep modes
#ifdef ESP8266
  WiFi.setSleepMode(WIFI_NONE_SLEEP);
#elif defined(ESP32)
  esp_sleep_pd_config(ESP_PD_DOMAIN_MAX,ESP_PD_OPTION_ON);
#endif

#if defined(ESP32) && !defined(DISABLE_ADC_CAL)
  // retrieve esp32 adc calibration
  adc1_config_width( ADC_RESOLUTION );
  // adc1_config_channel_atten(ADC1_CHANNEL_5,ADC_ATTEN_DB_11); sensor dependent !
  esp_adc_cal_src = 
           esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_RESOLUTION, DEFL_ESP32_ADC_VREF, adc_chars);
#endif /* ESP32 advanced ADC */
}


// ---
// lateSetup: called at the very end of setup()
void lateSetup( void ) {

  if( _need2reboot ) {
    log_warning(F("\nA reboot has been asked while in setup() ... probably a faulty WiFi connect ...")); log_flush();
    delay(3000);
    #ifndef DISABLE_MODULES
    modulesList.stopAll();
    #endif /* DISABLE_MODULES */
    neOSensor_reboot();
    delay(5000); // to avoid an infinite loop
  }
  
  char tmp[64];
  snprintf(tmp,sizeof(tmp),"\n\n\n#\n# %s firmware rev %d for neOCampus started :)",getBoardName(),getFirmwareRev());
  log_info(tmp);

  if( not cbtime_set ) {
    log_warning(F("\n#\t TIME NOT (YET) SYNCED !!"));
  }
  else {
    log_info(F("\n# current time is ")); log_info(getCurTime());
  }

  WiFi.setAutoReconnect(true);

#if ESP32
  // switch back POWER MODES to auto
  if( esp_sleep_pd_config(ESP_PD_DOMAIN_MAX,ESP_PD_OPTION_AUTO) != ESP_OK ) {
    log_warning(F("\nerror while restoring ESP32 power modes ?!?! ... continuing")); log_flush();
  }
#endif /* ESP32 */

#if defined(ESP32) && !defined(DISABLE_ADC_CAL)
  // display ADC calibration method
  if( esp_adc_cal_src != (esp_adc_cal_value_t)(-1) ) {
    log_info(F("\n# ESP32 ADC calibration source: "));
    switch(esp_adc_cal_src) {
      case ESP_ADC_CAL_VAL_EFUSE_VREF:
        log_info(F("eFuse vRef")); break;
      case ESP_ADC_CAL_VAL_EFUSE_TP:
        log_info(F("eFuse two points")); break;
      case ESP_ADC_CAL_VAL_DEFAULT_VREF:
        log_info(F("default Vref :(")); break;
      default:
        log_error(F("unknown ?!?!"));
    }
    log_flush();
  }
#endif /* ESP32 adcanced ADC */

#if defined(MAX_TCP_CONNECTIONS) && defined(ESP8266)
  log_info(F("\n# max TCP concurrent sockets = ")); log_info(MAX_TCP_CONNECTIONS, DEC); log_flush();
#elif defined(ESP32)
  log_info(F("\n# max TCP concurrent sockets = ")); log_info(CONFIG_LWIP_MAX_ACTIVE_TCP, DEC); log_flush();
  if( CONFIG_LWIP_MAX_ACTIVE_TCP < MAX_TCP_CONNECTIONS ) {
    log_warning(F("\nNOT ENOUGHT TCP CONNEXIONS ...")); log_flush();
    delay(1000);
  }
#endif /* MAX_TCP_CONNECTIONS */

  // display loop() delay
  log_info(F("\n# lopp() delay(ms): ")); log_info(MAIN_LOOP_DELAY,DEC); log_flush();

  log_info(F("\n# --- --- ---")); log_flush();
}


// --- SETUP -------------------------------------------------------------------
void setup() {

  /*
   * Very first steps of setup :)
   */
  earlySetup();

  // obtain and store MAC address ...
  getMacAddress();

  /*
   * Serial link for debug ...
   */
  setupSerial();

  /*
   * Setup system led (mostly ESP8266 with blue led on GPIO2)
   */
  setupSysLed();

  /*
   * Check for CLEAR switch status:
   * if micro-switch active at startup for a specified delay, then clear all configurations.
   */
#ifdef CLEAR_SW

  if( checkCLEARswitch(CLEAR_SW) == true ) {
    // CLEAR sensor parameters (WiFi, modules, EEPROM etc)
    clearSensor();
  }
#endif


  /*
   * Setup SPIFFS
   */
  setupSPIFFS();


  /* 
   * Led blinking for specific WiFi setup mode
   * - either with NOISE_LED (neOSensor)
   * - either with 7 segments (neOClock option)
   */
  wifiParameters.loadConfigFile();
  processWIFIparameters( &wifiParameters );
  if( clockModule ) {
    clockModule->animate( displayAnimate_t::network_connect );
  }
  else {
#ifdef LED
    setupLed( LED, (enum_ledmode_t)WIFI );
#endif
  }


  /*
   * WiFiManager to activate the network :)
   * - we added a 'sensOCampus' check box to enable/disable sensOCampus sandbox mode
   */
  setupWiFi( &wifiParameters );


  /*
   * setupNTP
   * Configure Timezone & DST
   * note: real ntp server may get sent from dhcp server :)
   */
  setupNTP();


  /*
   * Disable led blinking for WiFI setup mode since we're already connected
   */
  if( clockModule ) {
    clockModule->animate( displayAnimate_t::stop );
  }
  else {
#ifdef LED
    setupLed( LED, (enum_ledmode_t)DISABLE );
#endif
  }

  
  /*
   * Allocate time for ESP's firmware ...
   */
  yield();



  /*
   * Check for updates ...
   * - following example read a file containing config SSID and psk, it then compare to actual WiFi.SSID() and WiFi.psk()
   * to check if it needed to rerun a setup
   * https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266mDNS/examples/OTA-mDNS-SPIFFS/OTA-mDNS-SPIFFS.ino
   * - ArduinoOTA callback may disable interrupts
   */
  neOCampusOTA();


  /*
   * Allocate time for ESP's firmware ...
   */
  yield();



  /*
   * SENSOCAMPUS client starts to retrieve
   * [jun.18] WiFiParameters passed within constructor
   * - MQTT server url
   * - MQTT server port
   * - MQTT username
   * - MQTT password (first time only)
   */
  uint8_t _retry=SENSO_MAX_RETRIES;
  // now FS is initialized, load our config file (may contain the previsously saved mqtt_passwd)
  sensocampus.loadConfigFile();

  while( not _need2reboot and sensocampus.begin(getMacAddress()) != true ) {
    log_info(F("\n[senso] WARNING unable to achieve sensOCampus credentials and/or config :|"));
    if( _retry-- ) {
      log_debug(F("\n\t... sleeping a bit before retrying ..."));
      delay((SENSO_MAX_RETRIES-_retry)*10*1000);
    }
    else {
      log_error(F("\n[senso] ERROR no more retries with sensOCampus ... reboot"));
      _need2reboot = true;
    }
  }


  /*
   * Allocate time for ESP's firmware ...
   */
  yield();



  /*
   * Manage WiFi parameters options
   * WARNING: second call to process WiFi parameters
   */
  processWIFIparameters( &wifiParameters );
  delay(250);  // to avoid race condition between I2C clock and TM1637 CLOCK (shared pin)


  /*
   * Allocate time for ESP's firmware ...
   */
  yield();



  /*
   * neOCampus' modules instanciation
   */
  if( not _need2reboot ) {
    temperatureModule   = new temperature();
    humidityModule      = new humidity();
    luminosityModule    = new luminosity();
    #ifndef NOISE_DETECT
      noiseModule       = nullptr; // means no noise sensor
    #elif NOISE_LED    // with or without led signaling noise
      noiseModule       = new noise( NOISE_DETECT, &noiseDetectISR, NOISE_LED );
    #else
      noiseModule       = new noise( NOISE_DETECT, &noiseDetectISR );
    #endif
    airqualityModule    = new airquality();

    // [aug.21] digitalModule may get already instantiated through WiFiParameters management
    if( !digitalModule ) digitalModule = new digital();
    // add switches
    #ifdef INCR_SW
    digitalModule->add_gpio( "SW+", INCR_SW, digitalInputType_t::on_off, digitalFrontDetect_t::none );  // none ==> no MQTT sending
    #endif
    #ifdef DECR_SW
    digitalModule->add_gpio( "SW-", DECR_SW, digitalInputType_t::on_off, digitalFrontDetect_t::none );  // none ==> no MQTT sending
    #endif
    #ifdef CLEAR_SW
    // WARNING: CLEAR_SW & LED share the same pin (setupLED called with disable earlier, ok)
    digitalModule->add_gpio( "OK", CLEAR_SW, digitalInputType_t::on_off, digitalFrontDetect_t::none );  // none ==> no MQTT sending
    #endif

    displayModule       = new display();

    // add additional modules initialization here
  }



  /*
   * I2C bus initialization, set pins & frequency
   */
  setupI2C();


  /* 
   *  neOCampus modules instanciation
   *  - scan i2c bus
   *  - instantite components :)
   */
  uint8_t i2c_addr,res;
  
  i2c_addr=I2C_ADDR_START;
  log_debug(F("\nStart I2C scanning ..."));

  /* i2c scanner loop ...
   * [may.20] since some sensors are of both kinds (e.g SHTXX --> temperature AND hygro)
   * ==> each i2c addr ought to get tested against all kinds of modules 
   */
  do {
    if( _need2reboot ) break;
    
    res = i2c_scan(i2c_addr);
    if( res==uint8_t(-1) ) break;
    log_debug(F("\n\t... detected device at i2c_addr=0x"));log_debug(res,HEX);log_flush();


/* DEBUG single sensor ... 
if( res!=uint8_t(0x44) ) {
  log_debug(F("\n\t[DEBUG] discarding device at i2c_addr=0x"));log_debug(res,HEX);log_flush();
  i2c_addr=res+1;
  continue;
}
*/

    bool _known = false;

    // is chip a temperature sensor ?
    if( temperatureModule and temperatureModule->add_sensor(res) == true ) {
      log_debug(F("\n\t\tadded temperature sensor at i2c addr = 0x"));log_debug(res,HEX); log_flush();
      _known = true;
    }
    // is chip a luminosity sensor ?
    if( luminosityModule and luminosityModule->add_sensor(res) == true ) {
      log_debug(F("\n\t\tadded luminosity sensor at i2c addr = 0x"));log_debug(res,HEX); log_flush();
      _known = true;
    }
    // is chip a DAC (part of a noise detection subsystem) ?
    if( noiseModule and noiseModule->add_dac(res) == true ) {
      log_debug(F("\n\t\tadded DAC to noise module whose i2c addr = 0x"));log_debug(res,HEX); log_flush();      
      _known = true;
    }
    // is chip a humidity sensor ?
    if( humidityModule and humidityModule->add_sensor(res) == true ) {
      log_debug(F("\n\t\tadded humidity sensor at i2c addr = 0x"));log_debug(res,HEX); log_flush();
      _known = true;
    }
    // is chip an airquality sensor ?
    if( airqualityModule and airqualityModule->add_sensor(res) == true ) {
      log_debug(F("\n\t\tadded airquality sensor at i2c addr = 0x"));log_debug(res,HEX); log_flush();
      _known = true;
    }
    // is chip a display ?
    if( displayModule and displayModule->add_display(res) == true ) {
      log_debug(F("\n\t\tadded display at i2c addr = 0x"));log_debug(res,HEX); log_flush();
      _known = true;
    }


    // add test for others modules ...

    // did the i2c device has been identified ?
    if( not _known ) {
      log_warning(F("\n[WARNING] unknwown i2c device with i2c addr = 0x"));log_debug(res,HEX); log_flush();
    }
    
    // next iteration
    i2c_addr=res+1;

  } while( not _need2reboot and i2c_addr<=I2C_ADDR_STOP );

  // end of scanning
  log_debug(F("\n... END OF I2C scan ... "));log_flush();


  // add device module
  if( not modulesList.add(&deviceModule) ) {
    log_error(F("\n# unable to add deviceModule to the list ?!?! something weird is on way ... reboot asked"));log_flush();
    delay(5000);
    _need2reboot = true;
  }

  // check devices count in temperature module
  if( temperatureModule ) {
    // [aug.20] load an eventual sensOCampus configuration
    temperatureModule->loadSensoConfig( &sensocampus );

    if( temperatureModule->is_empty()==true or not modulesList.add(temperatureModule) ) {
      log_debug(F("\n# either temperature module does not have any sensor or we've not been able to add it to the list of modules ... removing instance ..."));log_flush();
      free(temperatureModule);
      temperatureModule = NULL;
    }
  }
#ifdef TEMPERATURE_CORRECTION_LASTI2C
  if( temperatureModule ) {
    log_warning(F("\n\n[temperature] >>>WARNING<<< measures correction of last i2c sensor activated!\n")); log_flush();
    delay(1000);
  }
#endif
    

  // check devices count in humidity module
  if( humidityModule ) {
    // [aug.20] load an eventual sensOCampus configuration
    humidityModule->loadSensoConfig( &sensocampus );

    if( humidityModule->is_empty()==true or not modulesList.add(humidityModule) ) {
      log_debug(F("\n# either humidity module does not have any sensor or we've not been able to add it to the list of modules ... removing instance ..."));log_flush();
      free(humidityModule);
      humidityModule = NULL;
    }
  }

  
  // check devices count in luminosity module
  if( luminosityModule ) {
    // [aug.20] load an eventual sensOCampus configuration
    luminosityModule->loadSensoConfig( &sensocampus );

    if( luminosityModule->is_empty()==true or not modulesList.add(luminosityModule) ) {
      log_debug(F("\n# either luminosity module does not have any sensor or we've not been able to add it to the list of modules ... removing instance ..."));log_flush();
      free(luminosityModule);
      luminosityModule = NULL;
    }
  }


  // check if noise module is ok
  if( noiseModule ) {
    // [aug.20] load an eventual sensOCampus configuration
    noiseModule->loadSensoConfig( &sensocampus );

    if( noiseModule->is_empty()==true or not modulesList.add(noiseModule) ) {
      log_debug(F("\n# either noise module didn't detected any dac or we've not been able to add it to the list of modules ... removing instance ..."));log_flush();
      free(noiseModule);
      noiseModule = NULL;
    }
  }


  // check if neoclock module is ok
  if( clockModule ) {
    // [aug.20] load an eventual sensOCampus configuration
    clockModule->loadSensoConfig( &sensocampus );

    if( clockModule->is_empty()==true or not modulesList.add(clockModule) ) {
      log_debug(F("\n# either neoclock module is empty or we've not been able to add it to the list of modules ... removing instance ..."));log_flush();
      free(clockModule);
      clockModule = NULL;
    }
  }

  // check if airquality module is ok
  if( airqualityModule ) {
    // [aug.20] load an eventual sensOCampus configuration
    airqualityModule->loadSensoConfig( &sensocampus );


    if( airqualityModule->is_empty()==true or not modulesList.add(airqualityModule) ) {
      log_debug(F("\n# either airquality module is empty or we've not been able to add it to the list of modules ... removing instance ..."));log_flush();
      free(airqualityModule);
      airqualityModule = NULL;
    }
  }

  // check if digital module is ok
  if( digitalModule ) {
    // [aug.21] load an eventual sensOCampus configuration
    digitalModule->loadSensoConfig( &sensocampus );

    if( digitalModule->is_empty()==true or not modulesList.add(digitalModule) ) {
      log_debug(F("\n# either digital module is empty or we've not been able to add it to the list of modules ... removing instance ..."));log_flush();
      free(digitalModule);
      digitalModule = nullptr;
    }
  }

  // check if display module is ok
  if( displayModule ) {
    // [aug.21] load an eventual sensOCampus configuration
    displayModule->loadSensoConfig( &sensocampus );

    if( displayModule->is_empty()==true or not modulesList.add(displayModule) ) {
      log_debug(F("\n# either display module is empty or we've not been able to add it to the list of modules ... removing instance ..."));log_flush();
      free(displayModule);
      displayModule = nullptr;
    }
  }

  // add check for additional neOCampus sensors/actuators modules


  /*
   * Allocate time for ESP's firmware ...
   */
  yield();


  /*
   * start all modules ...
   */
  modulesList.startAll( &sensocampus, sharedRoot );


  /*
   * Very end of setup() :)
   */
  lateSetup();
}


// --- LOOP --------------------------------------------------------------------
void loop() {

  /*
   * Process device, module sensors ...
   */
  modulesList.processAll();


  /* 
   * end of main loop
   */
  // call endLoop system management level
  endLoop();
  
  // waiting a bit
  delay( MAIN_LOOP_DELAY );
}
