
#ifndef BEZIER__
#define BEZIER__

#include <stdint.h>

#include "utils.h"

void start_bezier_path(uint16_t current_pos_x, uint16_t current_pos_y);

void get_next_bezier_point(location_msg_t *location);

#endif //BEZIER__
