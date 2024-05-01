# DIY light wand

This repo contains source code for a teensy 4.1 powered DIY light painting wand. I am not a professional nor do I really know what I'm doing. I hope that this repository has value to those who are attempting similar projects. This project works as is, but I would suggest doing some improvements and changes on your own wand based on your own prefences and requirements.

Also C sucks and I suck at C. Code is likely to have major bugs and contain bad practices. It works, it doesn't crash and so on, but I can't confidently say I'd know why it works. Memory leaks and so on are a possibility, but they just don't crash the controller because it has enough memory.

# Components and tools:

### Components used

- Teensy 4.1(chosen for on board sd slot and cpu speed).
- High density(144/m) 5V sk9822 addressable RGB strip(any Fast led compatible rgb strip should work).
- Aluminium led profile with frosted/milky diffuser.
- Various resistors for buttons.
- OLED display(SSD1306).
- 4 buttons.
- SD card.
- 5\*7cm prototype board.
- Smaller proto boards.
- 2.54mm male/female header pins.
- 2 Pre-soldered usb c ports with 4 pins.
- 12V USB C PD trigger board.
- 12V to 5V regulated DC transformer.
- 2x USB C to USB C cables.
- powerbank(with USB C PD).
- Wire, solder.

### Tools

- Soldering iron
- Solder sucker
- Multimeter
- 3D printer
- Clear epoxy

Simplified wiring diagrams and pictures of the controller are in docu_pics folder.

# Issues encountered during development

## Power input and stability

First prototypes had a lot of issues due to connectivity issues with a breadboard. Would advide against using them for anything more complex.

USB power banks hate unstable power draw. Only a small fraction of all tested powerbanks worked on the original hardware revision without USB C PD boards. Going from high power draw to near zero cuts power off, this is most likely caused by the power bank thinking that it has fully charged the device. Similarly most power banks don't charge low power devices, meaning that when LEDs are off, the power bank may turn off.

Managed to solve stability issues by using a 12V 3A PD board and a high current 9-16V to 5V regulated voltage converter. These are fairly cheap and I would encourage either using one of these or going with a 5V 3A trigger board instead.

If a strip is rated at 40W/m at 100% power, this means 100% brightness RGB white. Drawing images is unlikely to draw nearly as much and so matching power supply with strip ratings is not necessary. Even a 5V 3A or 15W power delivery system is likely to work well.

With 1m wands:
5V 1A is enough for working in dark spaces but will most likely not be enough to use the full brightness range of the led strip.
5V 3A is much better and should work well with most images and brightness levels.
5V 7A or the 12V PD trigger + 12V to 5V converter setup should work for 2m wands at high brightness settings.

## Code issues

Memory handling in c++ is a complete mystery to me. Original code did not clear images from memory after using them and this caused rapid teensy crashes. The memory of a variable can overflow over the memory of some other variable, causing odd behavior. C++ must have ways for handling issues like this, but I'm not familiar enough with the language to be aware of them yet.

# Photography tips

Iso 100, f1.8, 8 sec exposure and a 3 stop ND filter has proven to be a good starting point when taking light painting shots. Without nd filter, ambient light overexposes shots at wide apertures and this is fairly limiting. Variable nd filters are worth considering, but anything darker than nd8(3 stop) is unlikely to be useful.

The following shot of Ronja Lauton(@avaruusronja) was taken at f5, 8s, 100 iso, 16mm.

![Wand test with Avaruusronja(Ronja Lauton)](https://github.com/TimoSalola/lightwand/blob/main/lightwandtest.png)

You can remove the led diffuser for extra brightness whenever needed. Depending on pixel density, type and so on this might even look good.
