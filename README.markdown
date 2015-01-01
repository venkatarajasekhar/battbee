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
tool. Under Linux

### Modem XB24-Z7WIT-004

| Modem | XB24-ZB |
| Function Set | e.g. Zigbee Coordinator **API** |
| Networking -> PAN ID | e.g. 2001 |
| Serial Interfacing -> API Enable | 1 |

## References

### Links

### Books

Robert Faludi: Building Wireless Sensor Networks, [O'Reilly Media](http://shop.oreilly.com/product/9780596807740.do)


