
#ifndef MOTOR_REGISTER_H__
#define MOTOR_REGISTER_H__

#include <stdint.h>

void init_registration();

bool registration_complete();

bool service_register_carriage(uint8_t *movement);

#endif //MOTOR_REGISTER__
