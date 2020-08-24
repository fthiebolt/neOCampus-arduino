/**************************************************************************/
/*! 
    @file     lcc_sensor.h
    @author   F. Thiebolt
	  @license
	
    This is part of a the neOCampus drivers library.
    This driver is intended to the CNRS' LCC air quality monitoring board
    
    (c) Copyright 2020 Thiebolt F. <thiebolt@irit.fr>

	@section  HISTORY

    2020-Aug    - First release, F.Thiebolt
    
*/
/**************************************************************************/

#ifndef _LCC_SENSOR_H_
#define _LCC_SENSOR_H_


#include <Arduino.h>
#include <ArduinoJson.h>

#include "sensocampus.h"

// generic sensor driver
#include "generic_driver.h"



/*
 * Definitions
 */
// define GPIO INPUTS' roles associated with _inputs[]
enum {
  LCC_SENSOR_10K = 0,
  LCC_SENSOR_100K,
  LCC_SENSOR_1M,
  LCC_SENSOR_10M,
  LCC_SENSOR_ANALOG,
  LCC_SENSOR_LAST_INPUT
};

// output field corresponds to GPIO activating the heater



/*
 * Class
 */
class lcc_sensor : public generic_driver {
  public:

    // constructor
    lcc_sensor( void );
    
    /* Power Modes
     * [aug.20] the board does feature any way to switch power
    void powerON( void );       // switch ON
    void powerOFF( void );      // switch OFF
     */

    // sensor creation via sensOCampus JSON array 'params'
    boolean begin( JsonVariant );

    // send back sensor's value, units and subID
    float acquire( void );
    const char *sensorUnits( void ) { return units; };
    String subID( void ) { return _subID; };


  // --- protected methods / attributes ---------------------
  // --- i.e subclass have direct access to
  protected:
    // -- private/protected methods

    // --- private/protected attributes
    char _subID[SENSO_SUBID_MAXSIZE];
    uint8_t _inputs[LCC_SENSOR_LAST_INPUT];
    uint8_t _heater;      // GPIO PIN to start heating the sensor

    static const char *units;
    // uint8_t _integrationTime; // ms time to integrate a measure
};

#endif /* _LCC_SENSOR_H_ */