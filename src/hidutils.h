#ifndef HIDUTILS_H
#define HIDUTILS_H

#include <hidapi.h>

hid_device* get_intel_a770_le_hid(void);
int hid_write_and_read(hid_device *dev, unsigned char *buf, size_t len);
int get_firmware_version(hid_device *dev);
int set_enable_mode(hid_device *dev);
int set_apply_mode(hid_device *dev);
int set_color(unsigned int size, const unsigned char *leds, const unsigned char *color, hid_device *dev);
void dump_hid_packet(unsigned char *buf, size_t len);

extern int debug;

#endif
