
#include "motor_board.h"

#include <Arduino.h>
#include <Wire.h>

#include "config/config.h"
#include "sd_card/sd_card.h"
#include "utils/logging.h"
#include "motor_driver/motor_driver.h"
#include "motor_driver/motor_utils.h"
#include "motor_driver/path_calculator/path_calculator.h"

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

static void _get_next_position_from_wire(location_msg_t *location)
{
    uint8_t *received_buffer = (uint8_t *) location;
    
    int index = 0;
    
    // log_debug("Requesting next position from wire");
    
    location->x_location_steps = 0;
    location->y_location_steps = 0;
    
    log_debug("Requesting instruct after 5 sec");
    delay(8000);
        
    Wire.requestFrom(SD_CARD_BOARD_I2C_ADDR, 4);
    
    while (Wire.available()) { // peripheral may send less than requested
        char c = Wire.read();
        // Discard any extra bytes
        if (index >= sizeof(received_buffer)) {
            continue;
        }
        received_buffer[index] = (uint8_t) c;
        index++;
    }
    
    // log_debug_value("Got next position from wire", location->x_location_steps);
    Serial.print("Got instruction: ");
    Serial.print(location->x_location_steps, DEC);
    Serial.print(", ");
    Serial.print(location->y_location_steps, DEC);
    Serial.println(";");
}

void motor_board_loop()
{
    // Read next instruction if necesssary
    if (ready_for_next_instruction()) {
        if (!_next_instruction_ready()) {
            log_debug("Next instruction not ready");
            return;
        }
        location_msg_t target_position;
        _get_next_position_from_wire(&target_position);
        set_target_pos_steps(target_position.x_location_steps, target_position.y_location_steps);
    } else {
        // log_debug("Not ready for next instruction");
    }
    
    service_motors();
}
