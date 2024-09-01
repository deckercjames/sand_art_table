
#ifndef LOGGING_H__
#define LOGGING_H__

#include <Arduino.h>

#ifdef RELEASE

#define LOG_INIT(baud) do { } while (0)

#define log_fatal(msg) do { } while (0)

#define log_error(msg) do { } while (0)

#define log_info(msg) do { } while (0)

#define log_debug(msg) do { } while (0)

#define log_debug_value(msg, value) do { } while (0)

#else

#define LOG_INIT(baud) \
    do { \
        delay(200); \
        Serial.begin(baud); \
        Serial.println(F("\n\n**************************************")); \
    } while (0)

#define log_fatal(msg) \
    do { \
        Serial.print(F("[FATAL] ")); \
        Serial.println(F(msg)); \
    } while (0)

#define log_error(msg) \
    do { \
        Serial.print(F("[ERROR] ")); \
        Serial.println(F(msg)); \
    } while (0)

#define log_info(msg) \
    do { \
        Serial.print(F("[INFO]  ")); \
        Serial.println(F(msg)); \
    } while (0)

#define log_debug(msg) \
    do { \
        Serial.print(F("[DEBUG] ")); \
        Serial.println(F(msg)); \
    } while (0)

#define log_debug_value(msg, value) \
    do { \
        Serial.print(F("[DEBUG] ")); \
        Serial.print(F(msg)); \
        Serial.print(F(": ")); \
        Serial.println(value); \
    } while (0)

#endif //RELEASE

#endif //LOGGING_H__
