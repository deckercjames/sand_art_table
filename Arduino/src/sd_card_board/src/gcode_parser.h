
#ifndef GCODE_PARSER_H__
#define GCODE_PARSER_H__

#include "utils.h"

typedef struct gcode_instruction {
    uint16_t x_location_100um; // units are 0.1 millimeters (100 micrometers)
    uint16_t y_location_100um;
} gcode_instruction_t;

void parse_gcode_line(const char *instr_buf, gcode_instruction_t *new_target);

#endif