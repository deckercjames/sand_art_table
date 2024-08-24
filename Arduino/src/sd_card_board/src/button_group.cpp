
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
unsigned long bg_state_start_time_millis;

#define TIME_IN_STATE_MILLIS(start_time) (millis() - start_time)

const uint8_t button_list[] = {
    INPUT_BUTTON_1_PIN,
    INPUT_BUTTON_2_PIN,
    INPUT_BUTTON_3_PIN,
    INPUT_BUTTON_4_PIN,
};

void init_button_group()
{
    for (int i = 0; i < sizeof(button_list); i++) {
        pinMode(button_list[i], INPUT_PULLUP);
    }
    
    button_group_state = BUTTON_GROUP_STATE_IDLE;
}

int check_button_pressed()
{
    switch (button_group_state)
    {
        case BUTTON_GROUP_STATE_IDLE:
        {
            int pressed_button = 0;
            for (int i = 0; i < sizeof(button_list); i++) {
                if (digitalRead(button_list[i])) {
                    continue;
                }
                int pressed_button = i + 1;
                log_debug_value("Button pressed", pressed_button);
                button_group_state = BUTTON_GROUP_STATE_DEBOUNCE_PRESSED;
                bg_state_start_time_millis = millis();
                return pressed_button;
            }
            return 0;
        }
        case BUTTON_GROUP_STATE_DEBOUNCE_PRESSED:
            if (TIME_IN_STATE_MILLIS(bg_state_start_time_millis) > DEBOUNCE_TIME_MILLIS) {
                log_debug("Debounce pressed delay complete");
                button_group_state = BUTTON_GROUP_STATE_WAIT_RELEASE;
            }
            return 0;
        case BUTTON_GROUP_STATE_WAIT_RELEASE:
        {
            for (int i = 0; i < sizeof(button_list); i++) {
                if (digitalRead(button_list[i]) == LOW) {
                    return 0;
                }
            }
            log_debug("All buttons released");
            button_group_state = BUTTON_GROUP_STATE_DEBOUNCE_RELEASE;
            bg_state_start_time_millis = millis();
            return 0;
        }
        case BUTTON_GROUP_STATE_DEBOUNCE_RELEASE:
            if (TIME_IN_STATE_MILLIS(bg_state_start_time_millis) > BUTTON_GROUP_STATE_IDLE) {
                log_debug("Debounce released delay complete");
                button_group_state = BUTTON_GROUP_STATE_IDLE;
            }
            return 0;
    }
}
