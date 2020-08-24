/**************************************************************************/
/*! 
  @file     lcc_sensor.cpp
  @author   F.Thiebolt (neOCampus / Univ.Tlse3)
  @license  
	
	LCC's Airquality sensors driver for neOCampus

	@section  HISTORY

    aug.20  F.Thiebolt  neOCampus integration
                        adapted for neOCampus
    2020  Aymen Sendhi  sensor low-level functions
*/
/**************************************************************************/


#include <Arduino.h>

#include "neocampus.h"
#include "neocampus_debug.h"

#include "lcc_sensor.h"



/* declare kind of units (value_units) */
const char *lcc_sensor::units = "ppm";


/**************************************************************************/
/*! 
    @brief  Instantiates a new lcc_sensor class
*/
/**************************************************************************/
lcc_sensor::lcc_sensor( void ) : generic_driver() {
  _subID[0] = '\0';
}

/**************************************************************************/
/*! 
    @brief  Setups the HW
*/
/**************************************************************************/
boolean lcc_sensor::begin( JsonVariant root ) {

  // check input(s)
  if( root.isNull() or not root.is<JsonArray>() ) {
    log_error(F("\n[lcc_sensor] either empty JSON or wrong JSON type (array expected)!")); log_flush();
    return false;
  }

  boolean _param_subID = false;
  boolean _param_input = false;
  boolean _param_output = false;
  /* parse all parameters of  our sensor:
  [
    {
      "param": "subIDs",
      "value": "NO2"
    },
    {
      "param": "inputs",
      "value": [
        [
          16,
          17,
          5,
          18,
          35
        ]
      ]
    },
    {
      "param": "outputs",
      "value": -1
    }
  ]
  */
  for( JsonVariant item : root.to<JsonArray>() ) {

    if( item.isNull() or not item.is<JsonObject>() ) {
      log_warning(F("\n[lcc_sensor] format error while parsing parameters !")); log_flush();
      continue;
    }

    // SUBID
    const char *_param = PSTR("subID");
    if( strncmp_P(item[F("param")], _param, strlen_P(_param))==0 ) {
      snprintf( _subID, sizeof(_subID), item[F("value")]);
    }


    // TO BE CONTINUED


    // INPUT(S)


    // OUTPUT(S)

  }



  // TO BE CONTINUED



  /* set config:
   * - disable all alerts, hysteresis, critical threshold and so on
   * - set continuous conversions
   */

  // define defaults parameters like resolution

  return (_param_subID and _param_input and _param_output);
}



/**************************************************************************/
/*! 
    @brief  return sensor value read from pins
*/
/**************************************************************************/
float lcc_sensor::acquire( void )
{
  float val = 0.0;

  return val;
}



/* ------------------------------------------------------------------------------
 * Private methods 
 */

