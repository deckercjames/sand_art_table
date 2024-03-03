
#ifndef LOGGER_H__
#define LOGGER_H__

void logger_init();

void log_fatal_halt(char *str);
void log_error(char *str);
void log_info(char *str);
void log_debug(char *str);

#endif //ADD_H__
