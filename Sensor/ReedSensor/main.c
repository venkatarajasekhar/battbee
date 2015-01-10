#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "serial.h"

#define BIT_SET(X, I)	((X) |= (1 << (I)))
#define BIT_CLEAR(X, I)	((X) &= ~(1 << (I)))
#define BIT_ISSET(X, I)	(((X) & (1 << (I))) != 0)

ISR (INT0_vect)
{
    serial_sendstr("Button\n");
}

int main()
{
	serial_init();

	BIT_CLEAR(DDRD, PD2);	/* Digital input */
	BIT_SET(PORTD, PD2); /* Activate Pullup resistor */

	BIT_SET(EICRA, ISC00); /* Set INT0 to trigger on ANY logic change */
	BIT_SET(EIMSK, INT0); /* Turn on INT0 */
	sei();
	
	while(1)
	{
		_delay_ms(1000);
	}
	return 0;
}
