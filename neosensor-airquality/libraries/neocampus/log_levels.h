/*
 * neOCampus operation
 * 
 * Debug levels definitions
 * 
 * Thiebolt F. July 17
 * 
 */


#ifndef _LOG_LEVELS_H_
#define _LOG_LEVELS_H_

/*
 * Includes
 */



/*
 * Definitions
 */
/*
 * Logging levels:
 *  1: ERROR    errors
 *  3: WARN     errors and warnings
 *  5: INFO     errors, warnings and informational (default)
 *  4: DEBUG    errors, warnings, informational and debug
 */
#define LOG_LVL_ERROR     1
#define LOG_LVL_WARNING   3
#define LOG_LVL_INFO      5
#define LOG_LVL_DEBUG     10

#define DEFL_LOG_LVL      LOG_LVL_INFO


#endif /* _LOG_LEVELS_H_ */


