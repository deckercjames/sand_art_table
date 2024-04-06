
#ifndef LOGGING_H__
#define LOGGING_H__

#include <Arduino.h>

#define log_fatal(msg) do { \
    Serial.print("[FATAL] "); \
    Serial.println(F(msg)); \
} while (0);

#define log_error(msg) do { \
    Serial.print("[ERROR] "); \
    Serial.println(F(msg)); \
} while (0);

#define log_info(msg) do { \
    Serial.print("[INFO]  "); \
    Serial.println(F(msg)); \
} while (0);

#define log_debug(msg) do { \
    Serial.print("[DEBUG] "); \
    Serial.println(F(msg)); \
} while (0);

#endif //LOGGING_H__
