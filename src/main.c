#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <hidapi.h>

#include "hidutils.h"
#include "ledzones.h"

#define VERSION "1.0.0"

void print_copyright() {
    printf("Intel ARC A770 Limited Edition LED Utility v%s\n", VERSION);
    printf("Copyright (C) 2026 Steve Rainwater.\n");
    printf("This is free software released under then GPLv3+\n");
}

void print_help(const char *prog_name) {
    printf("Usage: %s [options]\n", prog_name);
    printf("Options:\n");
    printf("  -h, --help        Show this help message\n");
    printf("  -v, --version     Show version information\n");
    printf("\n");
    print_copyright();
}

int main(int argc, char *argv[]) {
    static struct option long_options[] = {
        {"help",    no_argument, 0, 'h'},
        {"version", no_argument, 0, 'v'},
        {0, 0, 0, 0}
    };

    int opt;
    while ((opt = getopt_long(argc, argv, "hv", long_options, NULL)) != -1) {
        switch (opt) {
            case 'h':
                print_help(argv[0]);
                return 0;
            case 'v':
                print_copyright();
                return 0;
            default:
                print_help(argv[0]);
                return 1;
        }
    }

    hid_init();
    hid_device *dev = get_intel_a770_le_hid();

    if (!dev) {
        fprintf(stderr, "Device not found!\n");
        return 1;
    }

    // do stuff here

    hid_close(dev);
    hid_exit();
    return 0;
}
