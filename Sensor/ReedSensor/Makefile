CC=avr-gcc
CFLAGS=-c -Os -mmcu=atmega328p -W -Wall
DEFINES=-DF_CPU=16000000UL -DBAUDRATE=9600
LFLAGS=-mmcu=atmega328p
OBJCOPY=avr-objcopy
AVRDUDE=avrdude

SOURCES=main.c serial.c
OBJECTS=$(SOURCES:.c=.o)
PROG=main


.PHONY: flash
flash: $(PROG).hex
	$(AVRDUDE) -F -V -c arduino -p ATMEGA328P -P /dev/ttyACM0 -b 115200 -U flash:w:$(PROG).hex

$(PROG).hex: $(PROG)
	$(OBJCOPY) -O ihex -R .eeprom $(PROG) $@

$(PROG): $(OBJECTS)
	$(CC) $(LFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $(DEFINES) $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJECTS) $(PROG) $(PROG).hex
