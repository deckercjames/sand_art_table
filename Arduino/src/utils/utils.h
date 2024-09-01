
#ifndef UTILS_H__
#define UTILS_H__

#include <stdint.h>

#define UNUSED(x) (void) (x)

#define PI 3.14159265f

#define SECS_TO_MILLIS (1000)

#define POT_MAX_VAL 1023

#define MM_TO_STEPS(millis) ((millis) * STEPS_PER_MM)
#define UM100_TO_STEPS(um100) ((um100) * STEPS_PER_100UM)

#define CUBED(x) ((x) * (x) * (x))
#define SQUARED(x) ((x) * (x))

#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })
     

typedef struct location_msg {
    uint16_t x_location_steps;
    uint16_t y_location_steps;
} __attribute__((__packed__)) location_msg_t;

#endif //UTILS_H__
