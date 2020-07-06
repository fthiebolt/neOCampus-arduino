#ifndef NEOLOGGER_HPP
#define NEOLOGGER_HPP

/*
 * Attempt at custom logger for the neOScheduler
 * Each strata of debug provides insight of what's going on 
 */

#include <Arduino.h>

#define LOG_LVL DBUG 1
#define LOG_LVL_INFO 2
#define LOG_LVL_WARN 3
#define LOG_LVL_ERR 4
#define LOG_LVL_CRIT 5

#define LOG_LVL LOG_LVL_DEBUG

/* Output debug level messages */
#if (LOG_LVL >= LOG_LVL_DEBUG)
    #define log_debug(X,...) {Serial.print(X, ## __VA_ARGS_);}
#else 
    #define log_debug(X,...) {}
#endif

/* Outputs info level messages*/
#if (LOG_LVL >= LOG_LVL_INFO)
    #define log_info(X,...) {Serial.print(X, ## __VA_ARGS_);}
#else 
    #define log_info(X,...) {}
#endif

/* Outputs warning level messages*/
#if (LOG_LVL >= LOG_LVL_WARN)
    #define log_warning(X,...) {Serial.print(X, ## __VA_ARGS_);}
#else 
    #define log_warning(X,...) {}
#endif

/* Outputs error level messages*/
#if (LOG_LVL >= LOG_LVL_ERR)
    #define log_error(X,...) {Serial.print(X, ## __VA_ARGS_);}
#else 
    #define log_error(X,...) {}
#endif

/* Outputs critical level messages*/
#if (LOG_LVL >= LOG_LVL_CRIT)
    #define log_critical(X,...) {Serial.print(X, ## __VA_ARGS_);}
#else 
    #define log_critical(X,...) {}
#endif

#endif