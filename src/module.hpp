#ifndef MODULE_HPP
#define MODULE_HPP
/*
 * Main base for the board
 */

#include <Arduino.h>
#include "base_class.hpp"
#include "cred.hpp"
#include "neologger.hpp"

#define MAC_SIZE 16
/* I'm not sure if 256 sensors or a single ESP8266 / ESP32 is overkill, but... who knows? */
#define MAX_SENSORS 256
class module{
public:
    /* generic constructor */
    module();
    /* Add a detected sensor into the sensor list */
    bool add();
    /* more likely to be the init() */
    bool start();
    /* maybe when need to reboot? */
    bool stop();
    /* calls the processing function of each sensor */
    int process_all();
    /* Getting the MAC address */
    char * getMAC();
    /* detecting all sensors by I2C scan, GPIO readings, ...  */
    void detect();
    /* usefullness of a destructor to be discussed */
    ~module();
private:
    /* the MAC is the primary identifier of the board*/
    static char _mac[MAC_SIZE];
    /* array of sensors */
    base_class *_sensorList[MAX_SENSORS];
};


#endif