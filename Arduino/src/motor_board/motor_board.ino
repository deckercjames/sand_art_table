
#include <Arduino.h>
#include <Wire.h>

#include "config.h"
#include "utils.h"
#include "logging.h"
#include "src/motor_driver/motor_driver.h"
#include "src/motor_driver/motor_utils.h"
#include "src/heat_manager/heat_manager.h"

#define HALT do { release_motors(); while(1); } while(0)

void setup()
{
    Wire.begin();
    
    init_heat_manager();

    if (!init_motors()) {
        log_fatal("Could not initilize motors");
        HALT;
    }
    log_info("Motors Initilized");
    
    set_state_register_carriage();
    
    pinMode(INSTRUCTION_READY_PIN_IN, INPUT_PULLUP);
}

static void _get_next_position_from_wire(location_msg_t *location)
{
    uint8_t *received_buffer = (uint8_t *) location;
    
    int index = 0;
    
    log_debug("Requesting next position from wire");
    
    location->x_location_steps = 0;
    location->y_location_steps = 0;
    
    Wire.requestFrom(SD_CARD_BOARD_I2C_ADDR, 4);
    
    while (Wire.available()) { // peripheral may send less than requested
        char c = Wire.read();
        // Discard any extra bytes
        if (index >= sizeof(location_msg_t)) {
            continue;
        }
        received_buffer[index] = (uint8_t) c;
        index++;
    }
    
    if (index < sizeof(location_msg_t)) {
        log_error("Received too few bytes");
    }

    log_debug_value("Received Position [x steps]", location->x_location_steps);
    log_debug_value("Received Position [y steps]", location->y_location_steps);
}

void loop()
{
    manage_heat();
    if (!motors_operable()) {
        HALT;
    }
    
    // Read next instruction if necesssary
    if (at_target()) {
        if (digitalRead(INSTRUCTION_READY_PIN_IN) != INSTRUCTION_READY) {
            log_debug("Next instruction not ready");
            return;
        }
        location_msg_t target_position;
        _get_next_position_from_wire(&target_position);
        set_target_pos_steps(&target_position);
    }
    
    service_motors();
}
