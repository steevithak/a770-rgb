#ifndef LEDZONES_H
#define LEDZONES_H

#include <stdint.h>
#include <stddef.h>

typedef struct {
    const char* name;
    const unsigned int* leds;
    const size_t count;
} led_zone;

// Accessor for the zones array
extern const led_zone zones[];
extern const size_t num_zones;

#endif
