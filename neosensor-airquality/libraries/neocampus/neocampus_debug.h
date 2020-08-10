/*
 * neOCampus operation
 * 
 * Debug definitions
 * 
 * Thiebolt F. June 17
 * 
 */


#ifndef _NEOCAMPUS_DEBUG_H_
#define _NEOCAMPUS_DEBUG_H_

/*
 * Includes
 */
#include <Arduino.h>

#include "log_levels.h"



/*
 * Definitions
 */

// define minimum messages
#if !defined(LOG_LEVEL)
  #define LOG_LEVEL       DEFL_LOG_LVL
#endif


// define macros related to various log levels
#define log_flush()                 { Serial.flush(); }


#if (LOG_LEVEL >= LOG_LVL_DEBUG)
  #define log_debug(X, ...)         { Serial.print(X, ## __VA_ARGS__); }
  #define log_debugF(X, ...)        { Serial.printf(X, ## __VA_ARGS__); }
#else
  #define log_debug(X, ...)         {}
  #define log_debugF(X, ...)        {}
#endif

#if (LOG_LEVEL >= LOG_LVL_INFO)
  #define log_info(X, ...)          { Serial.print(X, ## __VA_ARGS__); }
  #define log_infoF(X, ...)         { Serial.printf(X, ## __VA_ARGS__); }
#else
  #define log_info(X, ...)          {}
  #define log_infoF(X, ...)         {}
#endif

#if (LOG_LEVEL >= LOG_LVL_ERROR)
  #define log_error(X, ...)         { Serial.print(X, ## __VA_ARGS__); }
  #define log_errorF(X, ...)        { Serial.printf(X, ## __VA_ARGS__); }
#else
  #define log_error(X, ...)         {}
  #define log_errorF(X, ...)        {}
#endif

#if (LOG_LEVEL >= LOG_LVL_WARNING)
  #define log_warning(X, ...)       { Serial.print(X, ## __VA_ARGS__); }
  #define log_warningF(X, ...)      { Serial.printf(X, ## __VA_ARGS__); }
#else
  #define log_warning(X, ...)       {}
  #define log_warningF(X, ...)      {}
#endif


#endif /* _NEOCAMPUS_DEBUG_H */
