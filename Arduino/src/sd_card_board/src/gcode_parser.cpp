
#include "gcode_parser.h"

#include <stdint.h>
#include <stdlib.h>

#include "utils.h"
#include "config.h"

enum gcode_parse_state  {
    GCODE_STATE_PENDING_CMD,
    GCODE_STATE_PARSE_INSTR_NUMBER,
    GCODE_STATE_PARSE_INSTR_COMPLETE,
    GCODE_STATE_PENDING_ARG,
    GCODE_STATE_PARSE_ARG,
    GCODE_STATE_PROCESS_ARG,
    GCODE_STATE_SKIP_ARG,
};

static void process_pending_arg(char *arg_buf, int arg_buf_idx, char *current_arg_id, gcode_instruction_t *new_target)
{
    if (*current_arg_id == '\0') {
        return;
    }
    arg_buf[arg_buf_idx] = '\0';
    float mm = atof(arg_buf);
    mm = max(mm, 0.0f);
    unsigned int val_100um = (unsigned int) (mm * 10);
    if (*current_arg_id == 'X') {
        new_target->x_location_100um = val_100um;
    } else if (*current_arg_id == 'Y') {
        new_target->y_location_100um = val_100um;
    }
    *current_arg_id = '\0';
}

void parse_gcode_line(const char *instr_buf, gcode_instruction_t *new_target)
{
    int idx = 0;
    char c;
    char current_arg_id; // The arg that is currently being parsed. Either 'X' or 'Y'
    char current_arg_val[10];
    int current_arg_val_idx = 0;

    enum gcode_parse_state state = GCODE_STATE_PENDING_CMD;
    
    while (1)
    {
        c = instr_buf[idx];
        if (c == ';') {
            process_pending_arg(current_arg_val, current_arg_val_idx, &current_arg_id, new_target);
            break;
        }
        
        switch (state)
        {
            case GCODE_STATE_PENDING_CMD:
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
                process_pending_arg(current_arg_val, current_arg_val_idx, &current_arg_id, new_target);
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
