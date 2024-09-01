
#ifndef I2C_COMM__
#define I2C_COMM__

#include "utils.h"

void init_i2c_communcation();

void get_next_position_from_wire(location_msg_t *location);

#endif //I2C_COMM__
