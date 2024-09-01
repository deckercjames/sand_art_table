
#include "bezier.h"

#include <Arduino.h>

#include "config.h"
#include "logging.h"

typedef struct point {
    uint16_t x; // measured in steps
    uint16_t y;
} point_t;

// Global Variables
static const uint16_t *current_pos_x_ptr = 0; // measured in steps
static const uint16_t *current_pos_y_ptr = 0;

static point_t p0, p1, p2, p3, p4;

static float t;

void init_bezier(const uint16_t *current_pos_x, const uint16_t *current_pos_y)
{
    current_pos_x_ptr = current_pos_x;
    current_pos_y_ptr = current_pos_y;
}

static void randomize_point(point_t *point)
{
    point->x = random(MM_TO_STEPS(TABLE_DIM_X_MM));
    point->y = random(MM_TO_STEPS(TABLE_DIM_Y_MM));
}

static void randomize_segment_ending()
{
    randomize_point(&p2);
    randomize_point(&p4);
    
    // Set p3 to the midpoint of p2 and p4
    if (p2.x > p4.x) {
        p3.x = ((p2.x - p4.x) / 2) + p4.x;
    } else {
        p3.x = ((p4.x - p2.x) / 2) + p2.x;
    }
    
    if (p2.y > p4.y) {
        p3.y = ((p2.y - p4.y) / 2) + p4.y;
    } else {
        p3.y = ((p4.y - p2.y) / 2) + p2.y;
    }
}

void start_bezier_path()
{
    p0.x = *current_pos_x_ptr;
    p0.y = *current_pos_y_ptr;
    
    randomize_point(&p1);

    randomize_segment_ending();
    
    t = 0.0f;
}

void get_next_dest_point(location_msg_t *location)
{
    if (*current_pos_x_ptr == p3.x && *current_pos_y_ptr == p3.y)
    {
        log_debug("Bezier segment completed, planning next segment");

        p0.x = p3.x;
        p0.y = p3.y;
        
        p1.x = p4.x;
        p1.y = p4.y;
        
        randomize_segment_ending();
        t = 0.0f;
    }
    
    t += BEZIER_DELTA_T;
    
    if (t >= 0.999f) {
        location->x_location_steps = p3.x;
        location->y_location_steps = p3.y;
        return;
    }
    
    const float a = CUBED(1.0f - t);
    const float b = 3 * SQUARED(1.0f - t) * t;
    const float c = 3 * (1.0f - t) * SQUARED(t);
    const float d = CUBED(t);
    
    const float x = (a * p0.x) + (b * p1.x) + (c * p2.x) + (d * p3.x);
    const float y = (a * p0.y) + (b * p1.y) + (c * p2.y) + (d * p3.y);
    
    location->x_location_steps = (uint16_t) x;
    location->y_location_steps = (uint16_t) y;
}

