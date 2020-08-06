#include "neoscheduler.hpp"

// constructor
neoscheduler::neoscheduler( void ) {
    // neOScheduler init
}

// destructor
neoscheduler::~neoscheduler( void ) {
  // free dynamically allocated memory (if any)
}

void neoscheduler::setup(){
    log_debug("--- beg of neoscheduler::setup ---")
    _mem.begin(true);
    _mem.makedir("/");
    memcpy(_mac_addr, _wm.setup(),6);
    //byte b[] = {0x43, 0x55,0x87};
    //bc1.on_message(b, 0);
    //bc1.add();
    //bc2.add();
    //client.subscribe();
    //client.connect();
    //client.serialize();
    
    log_debug("Attempting to read conf from mem");
    //if(_mem.exists(CRED_FILE)){
    //    _jso = _mem.read(CRED_FILE);
    //}else{
        StaticJsonDocument<CRED_JSON_SIZE> json_doc;
        json_doc = _https.get_credentials(_mac_addr);
        _mem.write(CRED_FILE,json_doc);
    //}
    if(_mem.exists(MQTT_FILE)){
        _jso = _mem.read(MQTT_FILE);
    }else{
        log_error("this file wasn't found in memory");
    }
    log_debug("--- end of neoscheduler::setup ---")
}

void neoscheduler::loop(){
    // main loop processing sensors algorithms
}