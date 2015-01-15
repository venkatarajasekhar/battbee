#include "serial.h"



void serial_init()
{
	uint16_t baud_prescaler = ((F_CPU / (BAUDRATE * 16UL))) - 1;
	UBRR0H = (uint8_t)((baud_prescaler >> 8) & 0xff);
	UBRR0L = (uint8_t)(baud_prescaler & 0xff);
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	UCSR0C = (3 << UCSZ00);
}



uint8_t serial_receive()
{
	while (!(UCSR0A & (1 << RXC0)));
	return UDR0;
}


 
void serial_send(uint8_t value)
{ 
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = value;
}


 
void serial_sendstr(char* str)
{
	while (1)
	{
		switch (*str)
		{
			case '\0':
				return;
			case '\n':
				serial_send('\r');
			default:
				serial_send(*str);
		}
		str++;
	}
}
