
#ifndef MOTOR_DRIVER_H__
#define MOTOR_DRIVER_H__

#include "utils.h"

void init_motors();

void release_motors();

void service_motors(move_instr_t move);

#endif //MOTOR_DRIVER__
