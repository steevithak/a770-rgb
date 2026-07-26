# a770-rgb

A simple command line utility for configuring Intel Arc A770 LEDs.

I have an Intel A770 GPU on a headless NUC 12 Extreme running Fedora GNU/Linux
that I use to run large language models. I wanted to configure the LEDs on the
GPU and found there were no small command line utilities available to do it.
What I did find were these two options:

- Intel Arc RGB Controller, a proprietary, Windows-only program.
https://www.intel.com/content/www/us/en/download/743675/intel-arc-rgb-controller.html

- OpenRGB, an excellent GPL FLOSS application, however it's massive and
requires the QT libraries and a GUI to use all of its features. It is possible
to run on a headless server but seemed like overkill for my needs.
https://openrgb.org/

I decided the best plan was to write a simple program that met my needs. Maybe
it will be useful for others too. In developing it, I borrowed code from
OpenRGB, primarily their LED and zone map for the A770 that provides the hex
address of each LED. I also used Wireshark to capture examples of the protocol
being used.

## Building From Source

- Ensure the hidapi and hidapi-devel packages installed. This was tested
with the hidapi 0.15.0 release but should work with 0.14.0 as well. To
install, the command is usually `sudo dnf install hidapi hidapi-devel`.


- After cloning the repo cd to a770-rgb, then
   - `mkdir cmake`
   - `cd cmake`
   -  `cmake ..`
   -  `make`
- You should have an a770-rgb binary in the cmake directory now. Copy it a bin
directory in your path and it's ready to use.

## How to use it
A couple of notes.
- The Intel A770 LE RGB controller does not have any flash memory to store the
LED configurations. If you want the settings to stick you'll have to add the
commands to your start-up process by putting them in a systemd startup service
or an rc.local scipt on older systems. On Windows the Intel drivers loaded
saved color settings at boot time but I haven't found a similar feature in
the Linux drivers. The downside to this is that you'll have to keep the USB
cable connected permanently to maintain your color settings.

- By default, most systems will restrict the A770 USB port access to the root
user. So you'll need to use sudo to execute this program. Or you can use the
provided udev rules file to allow this user-level access to the A770 USB port.
To do this, follow these steps
  - copy 99-intel-a770.rules to /etc/udev/rules.d/
  - sudo udevadm control --reload
  - sudo udevadm trigger
  - this rules applies 0666 permissions to the port, if you want to tighter
  permissions, you'll need to edit the file to your likely.

The rest is fairly simple. All it does is basic set color commands. No
animations, blinking, breathing, just colors. If RGB hex values don't come
naturally to you, there are many online CSS hex color pickers that will let you
select color/brightness on a color wheel and give you the hex value.

```
$ a770-rgb --help
Intel Arc A770 Limited Edition LED Utility v1.0.0
Copyright (C) 2026 Steve Rainwater.
This is free software released under then GPLv3+

Usage: a770-rgb [options] [color]
Options:
  -h, --help                Show this help message
  -l, --list                List zones and LEDs
  -v, --version             Show a770-rgb version
  -d, --debug               Show verbose protocol info
  --zone [zone] [rgb-hex]   Set zone to color
  --led [address] [rgb-hex] Set LED to color

Examples:
  a770-rgb --list                  # List zone names and LED addresses
  a770-rgb --zone fan1 ff0000      # Set fan 1 to bright red
  a770-rgb --zone logo 22          # Set logo to dim white
  a770-rgb --zone led 0x84 00ff00  # Set LED 0x84 to green
  a770-rgb 550055                  # Set all LEDs (except logo) to purple
  a770-rgb off                     # Turn off all LEDs (including logo)
```
