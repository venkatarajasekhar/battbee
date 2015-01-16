#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <util/delay.h>

#include "xbee.h"

#define BIT_MASK(I)		(1 << (I))
#define BIT_SET(X, I)	((X) |= BIT_MASK(I))
#define BIT_CLEAR(X, I)	((X) &= ~BIT_MASK(I))
#define BIT_ISSET(X, I)	(((X) & BIT_MASK(I)) != 0)

ISR (INT0_vect)
{
}

void sleep_until_keypressed()
{
	// Setup wakeup condition
	BIT_SET(EICRA, ISC00); // Set INT0/Pin2 to trigger on ANY logic change
	BIT_SET(EIMSK, INT0); // Turn on INT0

	// Shut down as much as possible and sleep
	BIT_SET(PORTD, PD3);	// XBee: Set HIGH to sleep
	BIT_CLEAR(ADCSRA, ADEN);	// Disable ADC
 	power_all_disable();	// Disable Timer/Counter, USART, I2C, SPI, ...
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);  
	cli();
	sleep_enable();
	sleep_bod_disable();	// Brown-out disable
	sei();
	sleep_cpu();	// Goto Sleep
	
	// Wake up and enable just what we need
	sleep_disable();	// Wake up
	power_usart0_enable();	// Enable USART0
	BIT_CLEAR(PORTD, PD3);	// Set LOW to wake up
}

int main()
{
	xbee_init();

	DDRD &= 0b00000011;	// Set pin 2,4..7 as input with pullup
	PORTD |= 0b11110100;
	DDRB = 0b00000000;	// Set pin 8..13 as input  with pullup
	PORTB |= 0b11111111;

	BIT_SET(DDRD, PD3);

	while(1)
	{
		sleep_until_keypressed();
		xbee_sendstr("Someone pressed the button!\n", 0);
	}
	return 0;
}
