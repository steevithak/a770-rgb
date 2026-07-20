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
