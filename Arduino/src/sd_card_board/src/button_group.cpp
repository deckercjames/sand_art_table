
#include "button_group.h"

#include <Arduino.h>

#include "config.h"
#include "logging.h"


typedef enum button_group_state {
    BUTTON_GROUP_STATE_IDLE,
    BUTTON_GROUP_STATE_DEBOUNCE_PRESSED,
    BUTTON_GROUP_STATE_WAIT_RELEASE,
    BUTTON_GROUP_STATE_DEBOUNCE_RELEASE,
} button_group_state_t;

button_group_state_t button_group_state;
unsigned long state_start_time_millis;

#define TIME_IN_STATE_MILLIS(start_time) (millis() - start_time)

void init_button_group()
{
    pinMode(INPUT_BUTTON_1_PIN, INPUT_PULLUP);
    pinMode(INPUT_BUTTON_2_PIN, INPUT_PULLUP);
    pinMode(INPUT_BUTTON_3_PIN, INPUT_PULLUP);
    pinMode(INPUT_BUTTON_4_PIN, INPUT_PULLUP);
    
    button_group_state = BUTTON_GROUP_STATE_IDLE;
}


int check_button_pressed()
{
    switch (button_group_state)
    {
        case BUTTON_GROUP_STATE_IDLE:
        {
            int pressed_button = 0;
            if (digitalRead(INPUT_BUTTON_1_PIN) == LOW) {
                pressed_button = 1;
            }
            if (digitalRead(INPUT_BUTTON_2_PIN) == LOW) {
                pressed_button = 2;
            }
            if (digitalRead(INPUT_BUTTON_3_PIN) == LOW) {
                pressed_button = 3;
            }
            if (digitalRead(INPUT_BUTTON_4_PIN) == LOW) {
                pressed_button = 4;
            }
            // log_debug_value("Checking", pressed_button);
            if (pressed_button) {
                log_debug_value("Button pressed", pressed_button);
                button_group_state = BUTTON_GROUP_STATE_DEBOUNCE_PRESSED;
                state_start_time_millis = millis();
            }
            return pressed_button;
        }
        case BUTTON_GROUP_STATE_DEBOUNCE_PRESSED:
            if (TIME_IN_STATE_MILLIS(state_start_time_millis) > DEBOUNCE_TIME_MILLIS) {
                log_debug("Debounce pressed delay complete");
                button_group_state = BUTTON_GROUP_STATE_WAIT_RELEASE;
            }
            return 0;
        case BUTTON_GROUP_STATE_WAIT_RELEASE:
        {
            if (!digitalRead(INPUT_BUTTON_1_PIN) ||!digitalRead(INPUT_BUTTON_2_PIN) || !digitalRead(INPUT_BUTTON_3_PIN) || !digitalRead(INPUT_BUTTON_4_PIN)) {
                return 0;
            }
            log_debug("All buttons released");
            button_group_state = BUTTON_GROUP_STATE_DEBOUNCE_RELEASE;
            state_start_time_millis = millis();
            return 0;
        }
        case BUTTON_GROUP_STATE_DEBOUNCE_RELEASE:
            if (TIME_IN_STATE_MILLIS(state_start_time_millis) > BUTTON_GROUP_STATE_IDLE) {
                log_debug("Debounce released delay complete");
                button_group_state = BUTTON_GROUP_STATE_IDLE;
            }
            return 0;
    }
}
