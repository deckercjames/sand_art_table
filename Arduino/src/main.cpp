
#include <Arduino.h>

#include "sd_card/sd_card.h"
#include "motor_driver/motor_driver.h"
#include "led_strip/led_strip.h"
#include "config/config.h"

#define HALT do { release_motors(); while(1); } while(0)

void main_setup()
{
    Serial.begin(9600);
    Serial.println(F("Executing Setup **************************************"));

    if (!init_motors()) {
        Serial.println(F("Could not initilize motors"));
        HALT;
    }
    Serial.println(F("Motors Initilized"));
    
    if (!init_sd_card()) {
        Serial.println(F("Could not init sd card"));
        HALT;
    }
    
    if (!register_carriage()) {
        Serial.println(F("Could not register motors"));
        HALT;
    }
    
    set_target_position_gcode("G01 X100 Y100");
}


void main_loop()
{
    char line_buffer[LINE_BUFFER_SIZE];

    // Read next instruction if necesssary
    if (at_target()) {
        // set next target
        get_next_line(line_buffer, sizeof(line_buffer));
        set_target_position_gcode(line_buffer);
    }
    
    // Move to target
    if (!at_target()) { // guard again incase the next instruction was a noop (e.g. a gcode comment)
        move_one_instruction_t move_instr;
        get_motor_movement_instructions(&move_instr);
        move_toward_target(&move_instr);
    }
    
}

