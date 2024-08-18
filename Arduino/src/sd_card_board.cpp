
#include "sd_card_board.h"

#include <Wire.h>

#include "utils/logging.h"
#include "config/config.h"
#include "sd_card/sd_card.h"
#include "sd_card/gcode_parser.h"

#define HALT do {} while(1)

typedef enum {
    SD_STATE_IDLE,
    SD_STATE_OPEN_FILE,
    SD_STATE_READ_INSTR,
    SD_STATE_SEND_INSTR_PENDING,
    SD_STATE_INSTR_SENT,
    SD_STATE_CLOSE_FILE,
    SD_STATE_HALT,
} sd_state_t;

volatile sd_state_t sd_state;

location_msg_t next_location;

void wire_request_provide_next_pos()
{
    digitalWrite(INSTRUCTION_READY_PIN_OUT, INSTRUCTION_NOT_READY);
    const char * buffer = (const char *) & next_location;
    Wire.write(buffer, sizeof(location_msg_t));
    sd_state = SD_STATE_INSTR_SENT;
}


void sd_card_board_setup()
{
    // Setup I2C to send instruction to motor board
    Wire.begin(SD_CARD_BOARD_I2C_ADDR);
    Wire.onRequest(wire_request_provide_next_pos);
    
    // Setup pin to indicate a new instruction is ready
    pinMode(INSTRUCTION_READY_PIN_OUT, OUTPUT);

    // Setup SD card
    if (!init_sd_card()) {
        log_fatal("Could not init sd card");
        HALT;
    }
    log_info("SD Card Initilized");
    
    // Clear next instruction
    memset(&next_location, 0, sizeof(location_msg_t));
    
    sd_state = SD_STATE_OPEN_FILE;
}


void sd_card_board_loop()
{
    switch(sd_state)
    {
        case SD_STATE_OPEN_FILE:
            bool rc = open_next_file();
            if (!rc) {
                sd_state = SD_STATE_HALT;
                break;
            }
            sd_state = SD_STATE_READ_INSTR;
            break;
        case SD_STATE_READ_INSTR:
        {
            if (file_completed()) {
                sd_state = SD_STATE_CLOSE_FILE;
            }
            char buf[128] = { 0 };
            get_next_line(buf, sizeof(buf));
            location_msg_t parsed_location;
            parsed_location.x_location_steps = next_location.x_location_steps;
            parsed_location.y_location_steps = next_location.y_location_steps;
            parse_gcode_line(buf, &parsed_location);
            // No change. read another instruction
            if ((parsed_location.x_location_steps == next_location.x_location_steps) &&
                (parsed_location.y_location_steps == next_location.y_location_steps)) {
                break;
            }
            sd_state = SD_STATE_SEND_INSTR_PENDING;
            break;
        }
        case SD_STATE_SEND_INSTR_PENDING:
            break;
        case SD_STATE_INSTR_SENT:
            sd_state = SD_STATE_READ_INSTR;
            break;
        case SD_STATE_HALT:
            break;
        case SD_STATE_CLOSE_FILE:
            close_current_file();
            sd_state = SD_STATE_OPEN_FILE;
            break;
        
    }
}
