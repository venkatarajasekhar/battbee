# Battery-powered Zigbee sensor mesh network

**Project objective**: Use ZigBee network modules and microcontroller hardware
to build a battery-powered wireless sensor mesh network.

* ZigBee hardware: XBee2 and XBee2 Pro modules from
	[Digi](http://www.digi.com/)
* Microcontrollers: [Arduino](http://www.arduino.cc/) hardware, later maybe
	something from the [ATtiny](http://www.atmel.com/devices/attiny85.aspx) series
* Sensors: temperature, humidity, binary state change (e.g. door opening)
* Programming: C/C++ on Microcontrollers, Python on the server side (front end)
* Operating System: Developed under Linux (Ubuntu 14.04), but should run under windows too

## Hardware

## Configure XBee modules

Use the [X-CTU](http://www.digi.com/products/wireless-wired-embedded-solutions/zigbee-rf-modules/xctu)
tool to configure the XBee modules. Under Linux install *X-CTU* in version
5.2.8.6 (the Next Generation XCTU does not support user added COM ports by
now) using *wine* with

	wine 40003002_C.exe

create a link to XBee USB device

	ln -s /dev/ttyUSB0 ~/.wine/dosdevices/com5

and start X-CTU with

	wine ~/.wine/drive_c/Program\ Files\ \(x86\)/Digi/XCTU/X-CTU.exe

Under *User Com Ports* add COM5 and you are ready to use the X-CTU tool.

### Modem XB24-Z7WIT-004

#### Coordinator

* Modem: XB24-ZB
* Function Set: Zigbee Coordinator **API**
* Networking: PAN ID = 2001
* Serial Interfacing: API Enable = 1
* Sleep Modes: SP - Cyclic Sleep Period = AF0;
	SN - Number of Cyclic Sleep Periods = FFFF;
	Gives a maximum "Child Poll Timeout" of 63.7 days (see below)

	Set/read the number of cyclic sleep periods used to calculate end device poll
	timeout.  If an end device does not send a poll request to its parent coordinator
	or router within the poll timeout, the end device is removed from the child table.
	The poll timeout is calculated in milliseconds as (3 * SN * (SP * 10ms)), minimum
	of 5 seconds.  i.e. if SN=15, SP=0x64, the timeout is 45 seconds.

#### End Device

* Modem: XB24-ZB
* Function Set: Zigbee End Device **API**
* Networking: PAN ID = 2001
* Serial Interfacing: API Enable = 1
* Sleep Modes: SM - Sleep Mode = Pin Hibernate (1)

## References

### Links

* [How to Run an Arduino for Years on a Battery](http://www.openhomeautomation.net/arduino-battery/)
* [Hush little microprocessor… AVR and Arduino sleep mode basics](http://www.engblaze.com/hush-little-microprocessor-avr-and-arduino-sleep-mode-basics/)
* [Power saving techniques for microprocessors](http://www.gammon.com.au/forum/?id=11497)
* [JeeNode](http://jeelabs.org/category/software/)
* [Serving Raspberry Pi with Flask](http://mattrichardson.com/Raspberry-Pi-Flask/)

### Books

Robert Faludi: Building Wireless Sensor Networks, [O'Reilly Media](http://shop.oreilly.com/product/9780596807740.do)


