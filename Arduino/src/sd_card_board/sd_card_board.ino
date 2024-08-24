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

File current_file;

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

#define HALT do {} while(1)

void wire_request_provide_next_pos()
{
    log_info("Received wire request for instruction");
    digitalWrite(INSTRUCTION_READY_PIN_OUT, INSTRUCTION_NOT_READY);
    const char * buffer = (const char *) & next_location;
    Serial.print("Sending instruction: ");
    Serial.print(next_location.x_location_steps, DEC);
    Serial.print(", ");
    Serial.print(next_location.y_location_steps, DEC);
    Serial.println(";");
    Wire.write(buffer, sizeof(location_msg_t));
    sd_state = SD_STATE_INSTR_SENT;
}


void setup()
{
    LOG_INIT(SERIAL_BAUD);
  
    // Setup pin to indicate a new instruction is ready
    pinMode(INSTRUCTION_READY_PIN_OUT, OUTPUT);
    digitalWrite(INSTRUCTION_READY_PIN_OUT, INSTRUCTION_NOT_READY);

    // Setup I2C to send instruction to motor board
    Wire.begin(SD_CARD_BOARD_I2C_ADDR);
    Wire.onRequest(wire_request_provide_next_pos);
    
    // Setup SD card
    if (!init_sd_card()) {
        log_fatal("Could not init sd card");
        HALT;
    }
    log_info("SD Card Initilized");
    
    // Init buttons
    init_button_group();
    
    // Clear next instruction
    memset(&next_location, 0, sizeof(location_msg_t));
    
    sd_state = SD_STATE_OPEN_FILE;
}

void loop()
{
    check_button_pressed();
  
    switch(sd_state)
    {
        case SD_STATE_OPEN_FILE:
        {
            log_debug("SD state OEPN_FILE");
            bool rc = open_next_file();
            if (!rc) {
                sd_state = SD_STATE_HALT;
                break;
            }
            sd_state = SD_STATE_READ_INSTR;
            break;
        }
        case SD_STATE_READ_INSTR:
        {
            log_debug("SD state READ_INSTR");
            if (file_completed()) {
                log_info("File read complete");
                sd_state = SD_STATE_CLOSE_FILE;
            }
            log_debug("Reading line...");
            char buf[128] = { 0 };
            get_next_line(buf, sizeof(buf));
            log_debug_value("Read instruction", buf);
            location_msg_t old_location;
            old_location.x_location_steps = next_location.x_location_steps;
            old_location.y_location_steps = next_location.y_location_steps;
            parse_gcode_line(buf, &next_location);
            // No change. read another instruction
            if ((old_location.x_location_steps == next_location.x_location_steps) &&
                (old_location.y_location_steps == next_location.y_location_steps)) {
                log_debug("Instruction does not change location");
                break;
            }
            log_debug("Pending send instruction");
            digitalWrite(INSTRUCTION_READY_PIN_OUT, INSTRUCTION_READY);
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


