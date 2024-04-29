# DIY light wand

This repo contains source code for a teensy 4.1 powered DIY light painting wand. I am not a professional nor do I really know what I'm doing. I hope that this repository has value to those who are attempting similar projects. This project works as is, but I would suggest doing some improvements and changes on your own wand based on your own prefences and requirements.

Also C sucks and I suck at C. Code is likely to have major bugs and contain bad practices. It works, it doesn't crash and so on, but I can't confidently say I'd know why it works. Memory leaks and so on are a possibility, but they just don't crash the controller because it has enough memory.

### Components required
- Teensy 4.1 or some other arduino compatible board with SD card slot.
- High density 5V addressable RGB strip, apa102, sk9822 or something else. Check fastled support before buying. 144 per meter or 300 per meter strips seem to be the current best thing.
- Aluminium led profile with frosted/milky diffuser.
- Various resistors for buttons
- OLED display(SSD1306 or similar)
- 4 buttons
- SD card
- 5*7cm prototype board
- 3 Pre-soldered usb c ports with 4 pins.
- 2x USB C to USB C cables
- powerbank(with USB C PD)
- Wire, solder

### Optional
- Smaller proto boards
- 2.54mm male/female header pins
- USB C 5V 3A pd trigger board
- USB C 12V PD trigger board
- 12V to 5V DC converter

### Tools
- Soldering iron
- Solder sucker
- Multimeter
- 3D printer


## Controller unit
The controller unit consists of the teensy board, OLED display, 1 USB port for power input, 1 USB port for power and signal output, voltage transformer(optional) and buttons. A 5*7 cm proto board is enough to fit all of this and it will result in a really neat and small package, but debugging could be difficult. Another option is to go with a multi layer design which leaves enough room for a DC converter.

Breadboards are OK for early prototyping, but signal issues due to multiple poor connections were a common issue. Would suggest soldering a button + display unit on another proto board just for testing purposes and connecting multiple proto boards with soldered wires instead of using breadboards.

Online stores sell USB C ports with 4 pre soldered pinds. USB C is love, USB C is life and these can be used for all kinds of purposes as long as they are wired correctly. 

## LED wand
The wand is just a 1m long s9822 strip inside a 125cm long aluminium profile with a USB C port soldered to the 4 pins of the led strip. USB port is held in place by strong epoxy.

## Improvements on the "reference design"
The reference design is good but by placing components more optimally, some space could be saved and the case size could be smaller. Current size is not a problem for a separate control unit, but the controller could be integrated with the light painting wand. Perhaps the control unit could work as a handle that could be moved along the led profile? A coiled USB cable for controller to wand power/signal transfer could make this a really neat package. This would be easier if the controller was smaller.

Software does not contain support for drawing direction adjustments. Thus all images on the SD card have to be oriented properly. This could be implemented in software.

White balance adjustments are a real hack as fastled doesn't have WB adjustments, this could certainly be improved on. 


## Power input and stability
USB power banks hate unstable power draw. Only a small fraction of all tested powerbanks worked on the original hardware revision without USB C PD boards. Going from high power draw to near zero cuts power off, this is most likely caused by the power bank thinking that it has fully charged the device. Similarly most power banks don't charge low power devices, meaning that when LEDs are off, the power bank may turn off.

Managed to solve stability issues by using a 12V 3A PD board and a high current 9-16V to 5V regulated voltage converter. These are fairly cheap and I would encourage either using one of these or going with a 5V 3A trigger board instead.

If a strip is rated at 40W/m at 100% power, drawing images is unlikely to draw nearly as much and so matching power supply with strip ratings is not necessary. Even a 5V 3A or 15W power delivery system is likely to work well.

With 1m wands:
5V 1A is enough for working in dark spaces but will most likely not be enough to use the full brightness range of the led strip.
5V 3A is much better and should work well with most images and brightness levels.
5V 7A or the 12V PD trigger + 12V to 5V converter setup should work for 2m wands at high brightness settings.

## Photography tips
Iso 100, f1.8, 8 sec exposure and a 3 stop ND filter has proven to be a good starting point when taking light painting shots. Without nd filter, ambient light overexposes shots at wide apertures and this is fairly limiting. Variable nd filters are worth considering, but anything darker than nd8(3 stop) is unlikely to be useful.
![]([https://myoctocat.com/assets/images/base-octocat.svg](https://github.com/TimoSalola/lightwand/blob/main/DSC00034_01.jpg))


You can remove the led diffuser for extra brightness whenever needed. Depending on pixel density, type and so on this might even look good. 



