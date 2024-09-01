
#include "motor_driver.h"

#include <Arduino.h>

#include "motor_register.h"
#include "config.h"
#include "logging.h"

#define MOTOR_DISABLED (HIGH)
#define MOTOR_ENABLED (LOW)

void init_motors()
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

    init_registration();
}

#define NA (0x0)
#define CLOCKWISE (0x0)
#define COUNTER_CLOCKWISE (0x1)

typedef struct motor_commands {
    unsigned long delay_micros;
    int left_motor_dir;
    int left_motor_steps;
    int right_motor_dir;
    int right_motor_steps;
} motor_commands_t;

static const motor_commands_t move_instr_to_motor_cmds[] = {
    [MOVE_INSTR_NONE]       = { 0 },
    [MOVE_INSTR_UP]         = { MOTOR_SPEED_ORTH, COUNTER_CLOCKWISE, 1,  CLOCKWISE,         1  },
    [MOVE_INSTR_DOWN]       = { MOTOR_SPEED_ORTH, CLOCKWISE,         1,  COUNTER_CLOCKWISE, 1  },
    [MOVE_INSTR_LEFT]       = { MOTOR_SPEED_ORTH, COUNTER_CLOCKWISE, 1,  COUNTER_CLOCKWISE, 1  },
    [MOVE_INSTR_RIGHT]      = { MOTOR_SPEED_ORTH, CLOCKWISE,         1,  CLOCKWISE,         1  },
    [MOVE_INSTR_UP_LEFT]    = { MOTOR_SPEED_DIAG, COUNTER_CLOCKWISE, 2,  NA,                NA },
    [MOVE_INSTR_UP_RIGHT]   = { MOTOR_SPEED_DIAG, NA,                NA, CLOCKWISE,         2  },
    [MOVE_INSTR_DOWN_LEFT]  = { MOTOR_SPEED_DIAG, NA,                NA, COUNTER_CLOCKWISE, 2  },
    [MOVE_INSTR_DOWN_RIGHT] = { MOTOR_SPEED_DIAG, CLOCKWISE,         2,  NA,                NA },
};

static void service_motors(move_instr_t move)
{
    if (move == MOVE_INSTR_NONE) {
        return;
    }
    
    const motor_commands_t *motor_command = &move_instr_to_motor_cmds[move];

    // Set motor directions
    digitalWrite(LEFT_MOTOR_DIR_PIN,  motor_command->left_motor_dir);
    digitalWrite(RIGHT_MOTOR_DIR_PIN, motor_command->right_motor_dir);

    // One or two steps for each motor
    for (int i = 0; i < 2; i++)
    {
        digitalWrite(LEFT_MOTOR_STEP_PIN,  i < motor_command->left_motor_steps);
        digitalWrite(RIGHT_MOTOR_STEP_PIN, i < motor_command->right_motor_steps);

        delayMicroseconds(motor_command->delay_micros);

        digitalWrite(LEFT_MOTOR_STEP_PIN,  LOW);
        digitalWrite(RIGHT_MOTOR_STEP_PIN, LOW);

        delayMicroseconds(motor_command->delay_micros);
    }
}

void release_motors()
{
    digitalWrite(LEFT_MOTOR_EN_PIN, MOTOR_DISABLED);
    digitalWrite(RIGHT_MOTOR_EN_PIN, MOTOR_DISABLED);
}
