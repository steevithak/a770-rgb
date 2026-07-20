#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <hidapi.h>

#include "ledzones.h"

#define VERSION "1.0.0"
#define INTEL_ARC_A770_VID  0x2516  // Vendor ID
#define INTEL_ARC_A770_PID  0x01B5  // Produce ID
#define INTEL_ARC_A770_INT  0x01    // RGB Interface

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

hid_device* get_intel_a770_le_hid() {
    struct hid_device_info *devs, *cur_dev;
    hid_device *handle = NULL;

    devs = hid_enumerate(INTEL_ARC_A770_VID, INTEL_ARC_A770_PID);
    cur_dev = devs;

    while (cur_dev) {
        printf("path: %s, interface: %d\n", cur_dev->path, cur_dev->interface_number);
        if (cur_dev->interface_number == INTEL_ARC_A770_INT) {
            printf("Found target device: %s (Interface: %d)\n", 
                cur_dev->path, cur_dev->interface_number);

            handle = hid_open_path(cur_dev->path);
            if (handle) {
                // Double-check: confirm interface 1
                if (cur_dev->interface_number == 1) {
                    break;
                } else {
                    hid_close(handle);
                    handle = NULL;
                }
            }
        }
        cur_dev = cur_dev->next;
    }

    hid_free_enumeration(devs);
    return handle;
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
//                printf("Intel ARC A770 Limited Edition LED Utility v%s\n", VERSION);
//                printf("Copyright (C) 2026 Steve Rainwater.\n");
//                printf("This is free software released under then GPLv3+\n");
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

    // Now proceed with your handshake and commands as before...

    hid_close(dev);
    hid_exit();
    return 0;
}
