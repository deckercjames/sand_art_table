
#ifndef MOTOR_UTILS_H__
#define MOTOR_UTILS_H__


#include <stdint.h>

typedef uint8_t move_instr_t;


#define CLOCKWISE (0x0)
#define COUNTER_CLOCKWISE (0x8)

#define MOVE_NONE 0

#define ONE_STEP   (0x1)
#define TWO_STEPS  (0x1 | 0x2)

#define MOVEMENT(left_dir, left_steps, right_dir, right_steps) ( ((left_dir | left_steps) << 4) | (right_dir | right_steps) )

#define MOVE_UP         MOVEMENT(COUNTER_CLOCKWISE, ONE_STEP, CLOCKWISE,         ONE_STEP)
#define MOVE_DOWN       MOVEMENT(CLOCKWISE,         ONE_STEP, COUNTER_CLOCKWISE, ONE_STEP)
#define MOVE_RIGHT      MOVEMENT(CLOCKWISE,         ONE_STEP, CLOCKWISE,         ONE_STEP)
#define MOVE_LEFT       MOVEMENT(COUNTER_CLOCKWISE, ONE_STEP, COUNTER_CLOCKWISE, ONE_STEP)

#define MOVE_UP_LEFT    MOVEMENT(COUNTER_CLOCKWISE, TWO_STEPS, NA,                NA)
#define MOVE_UP_RIGHT   MOVEMENT(NA,                NA,        CLOCKWISE,         TWO_STEPS)
#define MOVE_DOWN_LEFT  MOVEMENT(NA,                NA,        COUNTER_CLOCKWISE, TWO_STEPS)
#define MOVE_DOWN_RIGHT MOVEMENT(CLOCKWISE,         TWO_STEPS, NA,                NA)


#endif //MOTOR_UTILS__
