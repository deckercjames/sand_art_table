
#include "motor_driver.h"

#include <Arduino.h>

#include "motor_utils.h"
#include "motor_register.h"
#include "path_calculator.h"
#include "config.h"
#include "logging.h"
#include "bezier.h"

#define MOTOR_DISABLED (HIGH)
#define MOTOR_ENABLED (LOW)

uint16_t current_pos_x_steps = 0; // measured in steps
uint16_t current_pos_y_steps = 0;

typedef enum {
    MOTOR_STATE_IDLE,
    MOTOR_STATE_REGISTER_CARRIAGE,
    MOTOR_STATE_DRAWING_IMAGE,
    MOTOR_STATE_DRAWING_BEZIER,
    MOTOR_STATE_THERMO_THROTTLE,
    MOTOR_STATE_HALT,
} motor_state_t;

motor_state_t motor_state;

bool init_motors()
{
    // Initilize Motors
    pinMode(LEFT_MOTOR_STEP_PIN, OUTPUT);
    pinMode(LEFT_MOTOR_DIR_PIN, OUTPUT);
    pinMode(LEFT_MOTOR_EN_PIN, OUTPUT);

    pinMode(RIGHT_MOTOR_STEP_PIN, OUTPUT);
    pinMode(RIGHT_MOTOR_DIR_PIN, OUTPUT);
    pinMode(RIGHT_MOTOR_EN_PIN, OUTPUT);

    digitalWrite(LEFT_MOTOR_EN_PIN, MOTOR_ENABLED);
    digitalWrite(RIGHT_MOTOR_EN_PIN, MOTOR_ENABLED);

    motor_state = MOTOR_STATE_REGISTER_CARRIAGE;

    init_registration();
    init_path_calculator(&current_pos_x_steps, &current_pos_y_steps);

    return true;
}

static void _make_movement(move_instr_t move, bool update_current_pos)
{
    // Set motor directions
    digitalWrite(LEFT_MOTOR_DIR_PIN,  ((move >> 7) & 0x01));
    digitalWrite(RIGHT_MOTOR_DIR_PIN, ((move >> 3) & 0x01));

    unsigned int motor_speed_delay = MOTOR_SPEED;
    // If moving diagonally, slow down by a factor of root 2
    if (move == MOVE_UP_LEFT || move == MOVE_UP_RIGHT ||
        move == MOVE_DOWN_LEFT || move == MOVE_DOWN_RIGHT) {
        motor_speed_delay = MOTOR_SPEED_DIAG;
    }

    // One or two steps for each motor
    for (int i = 0; i < 2; i++)
    {
        digitalWrite(LEFT_MOTOR_STEP_PIN,  (move >> (4 + i)) & 0x1);
        digitalWrite(RIGHT_MOTOR_STEP_PIN, (move >> (0 + i)) & 0x1);

        delayMicroseconds(motor_speed_delay);

        digitalWrite(LEFT_MOTOR_STEP_PIN,  LOW);
        digitalWrite(RIGHT_MOTOR_STEP_PIN, LOW);

        delayMicroseconds(motor_speed_delay);
    }

    if (!update_current_pos) {
        return;
    }

    // Update current position
    if (move == MOVE_UP || move == MOVE_UP_LEFT || move == MOVE_UP_RIGHT) {
        current_pos_y_steps++;
    } else if (move == MOVE_DOWN || move == MOVE_DOWN_LEFT || move == MOVE_DOWN_RIGHT) {
        current_pos_y_steps--;
    }

    if (move == MOVE_UP_LEFT || move == MOVE_LEFT || move == MOVE_DOWN_LEFT) {
        current_pos_x_steps--;
    } else if (move == MOVE_UP_RIGHT || move == MOVE_RIGHT || move == MOVE_DOWN_RIGHT) {
        current_pos_x_steps++;
    }
}

void release_motors()
{
    digitalWrite(LEFT_MOTOR_EN_PIN, MOTOR_DISABLED);
    digitalWrite(RIGHT_MOTOR_EN_PIN, MOTOR_DISABLED);
}

void service_motors()
{
    switch (motor_state)
    {
        case MOTOR_STATE_IDLE:
            break;
        case MOTOR_STATE_REGISTER_CARRIAGE:
        {
            if (registration_complete()) {
                log_info("Motor registration complete");
                motor_state = MOTOR_STATE_IDLE;
                randomSeed(micros() >> 2);
                break;
            }

            move_instr_t movement;
            bool register_rc;
            register_rc = service_register_carriage(&movement);

            if (!register_rc) {
                log_error("Motor registration failed");
                release_motors();
                motor_state = MOTOR_STATE_HALT;
                break;
            }

            _make_movement(movement, false);
            break;
        }
        case MOTOR_STATE_DRAWING_IMAGE:
        {
            if (path_calculator_at_target()) {
                break;
            }

            if (current_pos_y_steps % 50 == 0) {
        // log_debug_value("Current position [x steps]", current_pos_x_steps);
        // log_debug_value("Current position [y steps]", current_pos_y_steps);
            }

            move_instr_t move = path_calculator_get_motor_movement_instruction();
            _make_movement(move, true);
            break;
        }
        case MOTOR_STATE_DRAWING_BEZIER:
            break;
        case MOTOR_STATE_HALT:
        case MOTOR_STATE_THERMO_THROTTLE:
            break;
    }
}

void set_drawing_type(drawing_type_t type)
{
    if (motor_state != MOTOR_STATE_IDLE && motor_state != MOTOR_STATE_DRAWING_IMAGE && motor_state != MOTOR_STATE_DRAWING_BEZIER) {
        return;
    }

    if (type == DRAWING_TYPE_IMAGE && motor_state != MOTOR_STATE_DRAWING_IMAGE) {
        log_info("Transitioning to drawing image");
        path_calculator_reset_target();
        motor_state = MOTOR_STATE_DRAWING_IMAGE;
    } else if (type == DRAWING_TYPE_BEZIER && motor_state != MOTOR_STATE_DRAWING_BEZIER) {
        log_info("Transitioning to drawing bezier");
        start_bezier_path();
        motor_state = MOTOR_STATE_DRAWING_BEZIER;
    }
}

void set_target_pos_steps(const location_msg_t *target_location)
{
    uint16_t clamped_x = min(target_location->x_location_steps, MM_TO_STEPS(TABLE_DIM_X_MM));
    uint16_t clamped_y = min(target_location->y_location_steps, MM_TO_STEPS(TABLE_DIM_Y_MM));

    log_debug_value("Setting clamped position [x steps]", clamped_x);
    log_debug_value("Setting clamped position [y steps]", clamped_y);

    path_calculator_set_target_pos_steps(
        clamped_x,
        clamped_y
    );
}

bool is_in_image_mode()
{
    return motor_state == MOTOR_STATE_DRAWING_IMAGE;
}

bool ready_for_next_instr()
{
    return path_calculator_at_target();
}
