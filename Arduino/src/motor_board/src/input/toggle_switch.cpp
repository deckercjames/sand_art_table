
#include "toggle_switch.h"

#include <Arduino.h>

#include "logging.h"

toggle_switch_position_t current_toggle_position;
unsigned long debounce_until_time_millis;

void init_toggle_switch()
{
    pinMode(TOGGLE_UP_IN_PIN, INPUT_PULLUP);
    pinMode(TOGGLE_DOWN_IN_PIN, INPUT_PULLUP);

    debounce_until_time_millis = 0;
    current_toggle_position = TOGGLE_POSITION_OFF;
}

/**
 * Check the current toggle switch position and if it was thrown
 * 
 * @param ret_position [out] set to the current position of the toggle switch
 * @return true if the toggle swich was changed since the last time it was checked
*/
bool check_toggle_switch(toggle_switch_position_t *ret_position)
{
    if (millis() < debounce_until_time_millis) {
        *ret_position = current_toggle_position;
        return false;
    }

    toggle_switch_position_t new_toggle_position = TOGGLE_POSITION_OFF;

    if (digitalRead(TOGGLE_UP_IN_PIN) == LOW) {
        new_toggle_position = TOGGLE_POSITION_UP;
    } else if (digitalRead(TOGGLE_DOWN_IN_PIN) == LOW) {
        new_toggle_position = TOGGLE_POSITION_DOWN;
    }
    
    *ret_position = new_toggle_position;

    if (current_toggle_position != new_toggle_position) {
        log_debug_value("Toggle Switch thrown", new_toggle_position);
        current_toggle_position = new_toggle_position;
        debounce_until_time_millis = millis() + DEBOUNCE_TIME_MILLIS;
        return true;
    }

    return false;
}
