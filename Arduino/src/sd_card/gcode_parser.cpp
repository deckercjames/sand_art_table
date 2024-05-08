
#include "gcode_parser.h"

#include <stdlib.h>

#include "../utils/utils.h"
#include "../config/config.h"

enum gcode_parse_state  {
    GCODE_STATE_PENDING_INSTR,
    GCODE_STATE_PARSE_INSTR_NUMBER,
    GCODE_STATE_PARSE_INSTR_COMPLETE,
    GCODE_STATE_PENDING_ARG,
    GCODE_STATE_PARSE_ARG,
    GCODE_STATE_PROCESS_ARG,
    GCODE_STATE_SKIP_ARG,
};

void set_target_position_gcode(const char *instr, int *new_target_x, int *new_target_y)
{
    int idx = 0;
    char c;
    char current_arg_id; // The arg that is currently being parsed. Either 'X' or 'Y'
    char current_arg_val[10];
    int current_arg_val_idx;

    enum gcode_parse_state state = GCODE_STATE_PENDING_INSTR;
    
    while (1)
    {
        c = instr[idx];
        if (c == ';') {
            break;
        }
        
        switch (state)
        {
            case GCODE_STATE_PENDING_INSTR:
                if (c == ' ') {
                    break;
                }
                if (c != 'G') {
                    return;
                }
                state = GCODE_STATE_PARSE_INSTR_NUMBER;
                break;
            case GCODE_STATE_PARSE_INSTR_NUMBER:
                if (c == '0') {
                    break;
                }
                if (c == '1') {
                    state = GCODE_STATE_PARSE_INSTR_COMPLETE;
                    break;
                }
                return;
            case GCODE_STATE_PARSE_INSTR_COMPLETE:
                if (c != ' ') {
                    return;
                }
                state = GCODE_STATE_PENDING_ARG;
                break;
            case GCODE_STATE_PENDING_ARG:
                if (c == ' ') {
                    break;
                }
                if (c == 'X' || c == 'Y') {
                    state = GCODE_STATE_PARSE_ARG;
                    current_arg_id = c;
                    current_arg_val_idx = 0;
                    break;
                }
                state = GCODE_STATE_SKIP_ARG;
                break;
            case GCODE_STATE_PARSE_ARG:
                if (c == ' ' || c == '\0') {
                    state = GCODE_STATE_PROCESS_ARG;
                    continue;
                }
                if (current_arg_val_idx == sizeof(current_arg_val) - 1) {
                    state = GCODE_STATE_SKIP_ARG;
                    break;
                }
                current_arg_val[current_arg_val_idx++] = c;
                break;
            case GCODE_STATE_PROCESS_ARG:
            {
                current_arg_val[current_arg_val_idx] = '\0';
                float mm = atof(current_arg_val);
                mm = max(mm, 0.0f);
                if (current_arg_id == 'X') {
                    new_target_x = (unsigned int) MM_TO_STEPS(mm);
                } else if (current_arg_id == 'Y') {
                    new_target_y = (unsigned int) MM_TO_STEPS(mm);
                }
                state = GCODE_STATE_PENDING_ARG;
                break;
            }
            case GCODE_STATE_SKIP_ARG:
                if (c != ' ') {
                    break;
                }
                state = GCODE_STATE_PENDING_ARG;
                break;
        }
        
        if (c == '\0') {
            break;
        }

        idx++;
    }
}
