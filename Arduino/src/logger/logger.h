
#ifndef LOGGER_H__
#define LOGGER_H__

void init_logger();

void log_fatal(char *str);
void log_error(char *str);
void log_info(char *str);
void log_debug(char *str);

#endif //ADD_H__
