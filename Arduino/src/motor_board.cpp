
#include "motor_board.h"

#include <Arduino.h>
#include <Wire.h>

#include "config/config.h"
#include "sd_card/sd_card.h"
#include "utils/logging.h"
#include "motor_driver/motor_driver.h"

#define HALT do { release_motors(); while(1); } while(0)

void motor_board_setup()
{
    Wire.begin();

    if (!init_motors()) {
        log_fatal("Could not initilize motors");
        HALT;
    }
    log_info("Motors Initilized");
    
    set_state_register_carriage();
    
    pinMode(INSTRUCTION_READY_PIN_IN, INPUT_PULLUP);
}

static bool _next_instruction_ready()
{
    return digitalRead(INSTRUCTION_READY_PIN_IN) == INSTRUCTION_READY;
}

static uint32_t _get_next_position_from_wire()
{
    Wire.requestFrom(SD_CARD_BOARD_I2C_ADDR, 6);
    
    uint32_t target_pos = 0;
    
    while (Wire.available()) { // peripheral may send less than requested
        char c = Wire.read(); // receive a byte as character
    }
}

void motor_board_loop()
{
    // Read next instruction if necesssary
    if (ready_for_next_instruction()) {
        if (!_next_instruction_ready()) {
            return;
        }
        uint32_t target_position = _get_next_position_from_wire();
    }
    
    service_motors();
}
