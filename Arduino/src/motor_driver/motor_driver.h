
#ifndef MOTOR_DRIVER_H__
#define MOTOR_DRIVER_H__

bool init_motors();

void set_state_register_carriage();

void release_motors();

bool ready_for_next_instruction();

void service_motors();

#endif //MOTOR_DRIVER__
