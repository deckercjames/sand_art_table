
#ifndef MOTOR_DRIVER_H__
#define MOTOR_DRIVER_H__

#include "utils.h"

bool init_motors();

void set_state_register_carriage();

void release_motors();

bool at_target();

void set_target_pos_steps(const location_msg_t *target_location);

void service_motors();

#endif //MOTOR_DRIVER__
