/*
 * SD card read/write
 */

#include <SPI.h>
#include <SD.h>
#include <Wire.h>

#include "src/gcode_parser.h"
#include "src/button_group.h"
#include "src/sd_card_io.h"

#include "logging.h"
#include "utils.h"
#include "config.h"

typedef enum {
    SD_STATE_IDLE,
    SD_STATE_LOAD_FIRST_INSTR,
    SD_STATE_LOAD_NEXT_INSTR,
    SD_STATE_SEND_FIRST_INSTR_PENDING,
    SD_STATE_SEND_INSTR_PENDING,
    SD_STATE_CLOSE_FILE,
    SD_STATE_
} sd_state_t;

volatile sd_state_t sd_state;
unsigned long instr_ready_time_millis;

volatile gcode_instruction_t next_location;

void wire_request_provide_next_pos()
{
    log_info("Received wire request for instruction");
    digitalWrite(INSTRUCTION_READY_PIN_OUT, INSTRUCTION_NOT_READY);
    digitalWrite(SIG_INT_PIN_OUT, LOW);
    log_debug_value("Sending Position [x 100um]", next_location.x_location_100um);
    log_debug_value("Sending Position [y 100um]", next_location.y_location_100um);
    const location_msg_t outgoing_msg = {
        .x_location_steps = UM100_TO_STEPS(next_location.x_location_100um),
        .y_location_steps = UM100_TO_STEPS(next_location.y_location_100um),
    };
    Wire.write((const uint8_t *) &outgoing_msg, sizeof(location_msg_t));
    if (sd_state == SD_STATE_SEND_INSTR_PENDING || sd_state == SD_STATE_SEND_FIRST_INSTR_PENDING) {
        sd_state = SD_STATE_LOAD_NEXT_INSTR;
    }
}


void setup()
{
    LOG_INIT(SERIAL_BAUD);

    // Setup pin to indicate a new instruction is ready
    pinMode(INSTRUCTION_READY_PIN_OUT, OUTPUT);
    pinMode(SIG_INT_PIN_OUT, OUTPUT);

    // Clear next instruction
    memset(&next_location, 0, sizeof(location_msg_t));

    sd_state = SD_STATE_IDLE;

    // Setup SD card
    if (!init_sd_card()) {
        log_fatal("Could not init sd card");
        HALT;
    }
    log_info("SD Card Initilized");

    // Init buttons
    init_button_group();

    // Setup I2C last so we can not receive premature requests
    Wire.begin(SD_CARD_BOARD_I2C_ADDR);
    Wire.onRequest(wire_request_provide_next_pos);
}

void loop()
{
    int button_pressed = check_button_pressed();

    if (button_pressed) {
        log_info("main: button pressed");
        close_current_file();
        digitalWrite(INSTRUCTION_READY_PIN_OUT, INSTRUCTION_NOT_READY);
        digitalWrite(SIG_INT_PIN_OUT, HIGH);
        open_file_idx(button_pressed - 1);
        sd_state = SD_STATE_LOAD_FIRST_INSTR;
    }

    switch(sd_state)
    {
        case SD_STATE_IDLE:
            break;
        case SD_STATE_LOAD_FIRST_INSTR:
        case SD_STATE_LOAD_NEXT_INSTR:
        {
            if (file_completed()) {
                log_info("File read complete");
                sd_state = SD_STATE_CLOSE_FILE;
            }
            log_debug("Reading line...");
            char buf[128] = { 0 };
            get_next_line(buf, sizeof(buf));
            log_debug_value("Read instruction", buf);
            gcode_instruction_t old_location = {
                .x_location_100um = next_location.x_location_100um,
                .y_location_100um = next_location.y_location_100um,
            };
            parse_gcode_line(buf, &next_location);
            // No change. read another instruction next frame
            if ((old_location.x_location_100um == next_location.x_location_100um) &&
                (old_location.y_location_100um == next_location.y_location_100um)) {
                log_debug("Instruction does not change location");
                break;
            }
            digitalWrite(INSTRUCTION_READY_PIN_OUT, INSTRUCTION_READY);
            instr_ready_time_millis = millis();
            sd_state = (sd_state == SD_STATE_LOAD_FIRST_INSTR) ? SD_STATE_SEND_FIRST_INSTR_PENDING : SD_STATE_SEND_INSTR_PENDING;
            break;
        }
        case SD_STATE_SEND_FIRST_INSTR_PENDING:
            if (millis() - instr_ready_time_millis > FIRST_INSTR_TIMEOUT_MILLIS) {
                log_info("Motor board timed out");
                log_debug_value("Intr ready time", instr_ready_time_millis);
                log_debug_value("Current time", millis());
                digitalWrite(INSTRUCTION_READY_PIN_OUT, INSTRUCTION_NOT_READY);
                digitalWrite(SIG_INT_PIN_OUT, LOW);
                sd_state = SD_STATE_CLOSE_FILE;
            }
            break;
        case SD_STATE_SEND_INSTR_PENDING:
            break;
        case SD_STATE_CLOSE_FILE:
            close_current_file();
            sd_state = SD_STATE_IDLE;
            break;
    }
}


