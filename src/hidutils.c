
#include <stdio.h>
#include "hidutils.h"

#define INTEL_ARC_A770_VID  0x2516  // Vendor ID
#define INTEL_ARC_A770_PID  0x01B5  // Produce ID
#define INTEL_ARC_A770_INT  0x01    // RGB Interface
#define HID_TIMEOUT         250     // ms

// Look for an Intel Arc A770 Limited Edition RGB controller HID
// and return a hid_device handle if we find one
hid_device* get_intel_a770_le_hid(void) {
    struct hid_device_info *devs, *cur_dev;
    hid_device *handle = NULL;
    int res = 0;

    // find the A770 HID
    devs = hid_enumerate(INTEL_ARC_A770_VID, INTEL_ARC_A770_PID);
    cur_dev = devs;
    while (cur_dev) {
        printf("path: %s, interface: %d\n", cur_dev->path, cur_dev->interface_number);
        if (cur_dev->interface_number == INTEL_ARC_A770_INT) {
            printf("Found target device: %s (Interface: %d)\n", 
                cur_dev->path, cur_dev->interface_number);

            handle = hid_open_path(cur_dev->path);
            if (handle) {
//debug:steve - probably don't need this second check in the release code
                // make sure we return interface 1
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

    // query the version to check communication
    res = get_firmware_version(handle);

    // set enable mode
    if (res >= 0) {
        res = set_enable_mode(handle);
    }

    // set apply mode
    if (res >= 0) {
        set_apply_mode(handle);
    }

    return handle;
}

// Write to the HID and read the response
int hid_write_and_read(hid_device *dev, unsigned char *buf, size_t len) {
    if (hid_write(dev, buf, len) < 0) {
        fprintf(stderr, "hid_write failed\n");
        return -1;
    }

    int res = hid_read_timeout(dev, buf, len, HID_TIMEOUT);

    if (res > 0) {
        printf("DEBUG: Read %d bytes: ", res);
        for (int i = 0; i < res; i++) {
            printf("%02X ", buf[i]);
        }
        printf("\n");
    } else if (res < 0) {
        fprintf(stderr, "hid_read_timeout failed\n");
    }

    return res;
}

// Get A770/Coolermaster Firmware version
int get_firmware_version(hid_device *dev) {
    unsigned char buf[65] = {0};
    buf[1] = 0x12;
    buf[2] = 0x20;

    int res = hid_write_and_read(dev, buf, 65);

    if (res > 0) {
        // Version starts at index 8
        // Format: V n . n n . n n (with 0x00 padding)
        printf("Firmware Version: %c%c%c%c%c%c%c%c\n", 
               buf[8],  buf[10], buf[12], buf[14], 
               buf[16], buf[18], buf[20], buf[22]);
    }

    return res;
}

// Set enable mode
// Notes: OpenRGB calls this "enable" but other Cooler Master RGB code
// refers to 0x41 as the FLOW_CONTROL op code, so who knows...
int set_enable_mode(hid_device *dev) {
    unsigned char buf[65] = {0};
    buf[1] = 0x41;
    buf[2] = 0x03;

    int res = hid_write_and_read(dev, buf, 65);
}

// Set apply mode
// Notes: OpenRGB calls this "apply" but other Cooler Master RGB code
// refers to 0x51 as the WRITE op code, which sounds likely here.
int set_apply_mode(hid_device *dev) {
    unsigned char buf[65] = {0};
    buf[1] = 0x51;
    buf[2] = 0x28;

    int res = hid_write_and_read(dev, buf, 65);
}


// Send direct LED command
// coming soon...

