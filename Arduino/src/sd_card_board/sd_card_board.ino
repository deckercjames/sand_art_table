/*
  SD card read/write

  This example shows how to read and write data to and from an SD card file
  The circuit:
   SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4 (for MKRZero SD: SDCARD_SS_PIN)

  created   Nov 2010
  by David A. Mellis
  modified 9 Apr 2012
  by Tom Igoe

  This example code is in the public domain.

*/

#include <SPI.h>
#include <SD.h>

#include <Wire.h>

#include "utils/logging.h"
#include "utils/utils.h"
#include "config/config.h"

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

bool init_sd_card()
{
    return SD.begin(CARD_SELECT_PIN);
}

enum gcode_parse_state  {
    GCODE_STATE_PENDING_CMD,
    GCODE_STATE_PARSE_INSTR_NUMBER,
    GCODE_STATE_PARSE_INSTR_COMPLETE,
    GCODE_STATE_PENDING_ARG,
    GCODE_STATE_PARSE_ARG,
    GCODE_STATE_PROCESS_ARG,
    GCODE_STATE_SKIP_ARG,
};

void parse_gcode_line(const char *instr_buf, location_msg_t *new_target)
{
    int idx = 0;
    char c;
    char current_arg_id; // The arg that is currently being parsed. Either 'X' or 'Y'
    char current_arg_val[10];
    int current_arg_val_idx;

    enum gcode_parse_state state = GCODE_STATE_PENDING_CMD;
    
    while (1)
    {
        c = instr_buf[idx];
        if (c == ';') {
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
                current_arg_val[current_arg_val_idx] = '\0';
                float mm = atof(current_arg_val);
                mm = max(mm, 0.0f);
                if (current_arg_id == 'X') {
                    new_target->x_location_steps = (unsigned int) MM_TO_STEPS(mm);
                } else if (current_arg_id == 'Y') {
                    new_target->y_location_steps = (unsigned int) MM_TO_STEPS(mm);
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

bool open_next_file()
{
    if (!SD.exists("test.gcd")) {
        log_error("File does not exist");
        return false;
    }
    log_debug("File exists");
    log_debug("opening next file");
    current_file = SD.open("test.gcd");
    if (!current_file) {
        log_error("File opened Failed");
        return false;
    }
    
    log_info("File successfully opened");
    return true;
}

void close_current_file()
{
    log_debug("Closing current file");
    current_file.close();
}

void get_next_line(char *buf, unsigned int buf_size)
{
    unsigned int i = 0;
    int val;
    
    if (!current_file) {
        log_debug("no current file");
        buf[0] = 0;
        return;
    }
    
    while (1)
    {
        val = current_file.read();
        
        log_debug_value("reading value", val);
        
        if (val == -1) {
            break;
        }
        
        char c = (char) val;
        
        if (c == '\n') {
            break;
        }
        
        buf[i] = c;
        
        i++;
    }
    
    log_debug("buffer read complete");
    buf[i] = 0;
}


bool file_completed()
{
    return !current_file.available();
}

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


void setup() {
  
  Serial.begin(9600);
  Serial.println("\n\n**************************************");
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

void loop() {
  
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


