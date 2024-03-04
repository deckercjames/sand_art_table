
#include <Arduino.h>
#include "logger.h"

typedef enum log_severity {
    LOG_DEBUG,
    LOG_INFO,
    LOG_ERROR,
    LOG_FATAL,
    LOG_NONE,
} log_severity_t;

const log_severity_t log_severity = LOG_DEBUG;

char *severity_prefixes[] = {
    [LOG_DEBUG] = "[DEBUG] ",
    [LOG_INFO]  = "[INFO]  ",
    [LOG_ERROR] = "[ERROR] ",
    [LOG_FATAL] = "[FATAL] ",
};

void init_logger()
{
    Serial.begin(9600);
}

static void log_message(log_severity_t msg_severity, char *msg)
{
    if (msg_severity < log_severity) {
        return;
    }
    
    Serial.print(severity_prefixes[msg_severity]);
    Serial.println(msg);
}

void log_fatal(char *msg)
{
    log_message(LOG_FATAL, msg);
}

void log_error(char *msg)
{
    log_message(LOG_ERROR, msg);
}

void log_info(char *msg)
{
    log_message(LOG_INFO, msg);
}

void log_debug(char *msg)
{
    log_message(LOG_DEBUG, msg);
}