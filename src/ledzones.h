#ifndef LEDZONES_H
#define LEDZONES_H

#include <stdint.h>
#include <stddef.h>

typedef struct {
    const char* name;
    const char* desc;
    const uint8_t* leds;
    const size_t count;
} led_zone;

// Accessor for the zones array
extern const led_zone zones[];
extern const size_t num_zones;

void print_zones();

extern int debug;

#endif
