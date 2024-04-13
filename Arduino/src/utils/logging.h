
#ifndef LOGGING_H__
#define LOGGING_H__

#include <Arduino.h>

typedef enum logging_level {
    LOG_LEVEL_FATAL = 0,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_INFO,
    LOG_LEVEL_DEBUG,
} logging_level_t;

extern logging_level_t current_level;

#define log_fatal(msg) \
    if (current_level >= LOG_LEVEL_FATAL) { \
        Serial.print(F("[FATAL] ")); \
        Serial.println(F(msg)); \
    }

#define log_error(msg) \
    if (current_level >= LOG_LEVEL_ERROR) { \
        Serial.print(F("[ERROR] ")); \
        Serial.println(F(msg)); \
    }

#define log_info(msg) \
    if (current_level >= LOG_LEVEL_INFO) { \
        Serial.print(F("[INFO]  ")); \
        Serial.println(F(msg)); \
    }

#define log_debug(msg) \
    if (current_level >= LOG_LEVEL_DEBUG) { \
        Serial.print(F("[DEBUG] ")); \
        Serial.println(F(msg)); \
    }

#define log_debug_value(msg, value) \
    if (current_level >= LOG_LEVEL_DEBUG) { \
        Serial.print(F("[DEBUG] ")); \
        Serial.print(F(msg)); \
        Serial.print(F(": ")); \
        Serial.println(value); \
    }

#endif //LOGGING_H__
