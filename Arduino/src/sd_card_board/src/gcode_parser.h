
#ifndef GCODE_PARSER_H__
#define GCODE_PARSER_H__

#include "utils.h"

void parse_gcode_line(const char *instr_buf, location_msg_t *new_target);

#endif