
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

toggle_switch_position_t check_toggle_switch()
{
    if (millis() < debounce_until_time_millis) {
        return current_toggle_position;
    }

    toggle_switch_position_t new_toggle_position = TOGGLE_POSITION_OFF;

    if (digitalRead(TOGGLE_UP_IN_PIN) == LOW) {
        new_toggle_position = TOGGLE_POSITION_UP;
    } else if (digitalRead(TOGGLE_DOWN_IN_PIN) == LOW) {
        new_toggle_position = TOGGLE_POSITION_DOWN;
    }

    if (current_toggle_position != new_toggle_position) {
        log_debug_value("Toggle Switch thrown", new_toggle_position);
        current_toggle_position = new_toggle_position;
        debounce_until_time_millis = millis() + DEBOUNCE_TIME_MILLIS;
    }

    return current_toggle_position;
}
