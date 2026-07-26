#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include <hidapi.h>

#include "hidutils.h"
#include "ledzones.h"

#define VERSION "1.0.0"

// print the copyright
void print_copyright() {
    printf("Intel Arc A770 Limited Edition LED Utility v%s\n", VERSION);
    printf("Copyright (C) 2026 Steve Rainwater.\n");
    printf("This is free software released under then GPLv3+\n\n");
}

// print some help for the user
void print_help(const char *prog_name) {
    printf("Usage: %s [options] [color]\n", prog_name);
    printf("Options:\n");
    printf("  -h, --help           Show this help message\n");
    printf("  -l, --list           List zones and LEDs\n");
    printf("  -v, --version        Show version information\n");
    printf("  --zone [zone] [rgb-hex] Set zone to color");
    printf("  --led [address] [rgb-hex] Set LED to color");
    printf("\n");
}

// set all LEDs in a named zone to a color
int set_zone_color(const char *zone_name, unsigned char *color, void *dev) {

    const led_zone *target_zone = NULL;

    // find the requested zone
    for (int i = 0; i < num_zones; i++) {
        if(strcasecmp(zones[i].name, zone_name) == 0) {
            target_zone = &zones[i];
            break;
        }
    }
    // if we couldn't find the zone return
    if (!target_zone) {
        printf("Zone %s not found\n",zone_name);
        return -1;
    }

//debug:steve
//printf("set_zone_color() color: 0x%02x%02x%02x",color[0], color[1], color[3]);

    return set_color(target_zone->count, target_zone->leds, color, dev);
}

int main(int argc, char *argv[]) {

    int opt;
    char *zone = NULL;
    unsigned char *led_str = NULL;
    char *color_str = NULL;
    unsigned char color[3] = {0};

    enum {
        OPT_ZONE = 1000,
        OPT_LED
    };

    static struct option long_options[] = {
        {"help",    no_argument,       0, 'h'},
        {"list",    no_argument,       0, 'l'},
        {"version", no_argument,       0, 'v'},
        {"zone",    required_argument, 0, OPT_ZONE},
        {"led",     required_argument, 0, OPT_LED},
        {0, 0, 0, 0}
    };

    while ((opt = getopt_long(argc, argv, "hlv", long_options, NULL)) != -1) {
        switch (opt) {
            case 'h':
                print_copyright();
                print_help(argv[0]);
                return 0;
            case 'l':
                print_copyright();
                print_zones();
                return 0;
            case 'v':
                print_copyright();
                return 0;
            case OPT_ZONE:
                zone = optarg;
                break;
            case OPT_LED:
                led_str = optarg;
                break;
            default:
                print_help(argv[0]);
                return 1;
        }
    }

    int num_args = argc - optind;

    if (num_args > 0) {
        char hextest[7];
        color_str = argv[optind];

        // ensure color is a valid 6 digit hex number
        unsigned char r, g, b;
        int rc = sscanf(color_str, "%2hhx%2hhx%2hhx", &r, &g, &b);
        if(rc == 1) {
            g = 0x00;
            b = 0x00;
        } else if (rc != 3) {
            printf("color must be valid 6-digit hexadecimal value");
            return 1;
        }
        color[0] = r;
        color[1] = g;
        color[2] = b;

//debug:steve
//printf("  red: 0x%02x\n", color[0]);
//printf("green: 0x%02x\n", color[1]);
//printf(" blue: 0x%02x\n", color[2]);

    }

    if (hid_init() != 0) {
        fprintf(stderr,"Failed to initialize HIDAPI");
        return 1;
    }

    hid_device *dev = get_intel_a770_le_hid();

    if (!dev) {
        fprintf(stderr, "Device not found!\n");
        return 1;
    }

    // Set a color, either all, zone, or led

    if (zone != NULL) {
        // Set a zone to a color
        // debug:steve
        printf("Target zone: %s, color: 0x%02x%02x%02x\n", zone, color[0], color[1], color[2]);
//debug:steve: probably need to check the return value here
        set_zone_color(zone, color, dev);
    } else if (led_str != NULL) {
printf("setting LED: led_str\n",led_str);
        // Set one LED to a color
        unsigned char led;
        if( sscanf(led_str, "%x", &led) != 1 ) {
            printf("LED address must be valid 2-digit hexadecimal value");
        } else {
            // debug:steve
            printf("Target led: 0x%02x, color: 0x%02x%02x%02x\n", led, color[0], color[1], color[2]);
            set_color(1, &led, color, dev);
        }
    } else {
        // Set all LEDs to a color
        set_zone_color("ring",color,dev);
        set_zone_color("fan1",color,dev);
        set_zone_color("fan2",color,dev);
        set_zone_color("back",color,dev);
    }


    hid_close(dev);
    hid_exit();
    return 0;
}
