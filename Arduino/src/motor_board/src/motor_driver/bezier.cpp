
#include "bezier.h"

#include <Arduino.h>

#include "config.h"
#include "utils.h"

typedef struct point {
    uint16_t x;
    uint16_t y;
} point_t;

// Global Variables
const uint16_t *current_pos_x_ptr_bezier = 0; // measured in steps
const uint16_t *current_pos_y_ptr_bezier = 0;

point_t p0, p1, p2, p3;

void init_bezier(const uint16_t *current_pos_x, const uint16_t *current_pos_y)
{
    current_pos_x_ptr_bezier = current_pos_x;
    current_pos_y_ptr_bezier = current_pos_y;
}

static void randomize_point(point_t *point)
{
    point->x = random(MM_TO_STEPS(TABLE_DIM_X_MM));
    point->y = random(MM_TO_STEPS(TABLE_DIM_Y_MM));
}

void start_bezier_path()
{
    p0.x = *current_pos_x_ptr_bezier;
    p0.y = *current_pos_y_ptr_bezier;
    
    randomize_point(&p1);
    randomize_point(&p2);
    randomize_point(&p3);
}

