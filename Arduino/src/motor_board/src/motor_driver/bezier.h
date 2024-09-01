
#ifndef BEZIER__
#define BEZIER__

#include <stdint.h>

#include "utils.h"

void init_bezier(const uint16_t *current_pos_x, const uint16_t *current_pos_y);

void start_bezier_path();

void get_next_dest_point(location_msg_t *location);

#endif //BEZIER__
