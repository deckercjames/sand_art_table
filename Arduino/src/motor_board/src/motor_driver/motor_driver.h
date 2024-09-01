
#ifndef MOTOR_DRIVER_H__
#define MOTOR_DRIVER_H__

#include "utils.h"

typedef enum drawing_type {
    DRAWING_TYPE_IMAGE,
    DRAWING_TYPE_BEZIER,
} drawing_type_t;

bool init_motors();

void release_motors();

void set_drawing_type(drawing_type_t type);

bool is_in_image_mode();
bool is_in_bezier_mode();
bool ready_for_next_instr();

void set_target_pos_steps(const location_msg_t *target_location);

void service_motors();

#endif //MOTOR_DRIVER__
