#include "DHT22.h"



DHT22::DHT22(uint8_t pin)
{
	_pin = pin;
	pinMode(_pin, INPUT);
	digitalWrite(_pin, HIGH);
}



bool DHT22::Read(float &temperature, float &humidity)
{
	uint8_t laststate = HIGH;
	uint8_t counter = 0;
	uint8_t j = 0;

	// Receive buffer
	uint8_t data[6];
	data[0] = data[1] = data[2] = data[3] = data[4] = 0;

	// Pull the pin high and wait 250 milliseconds
	digitalWrite(_pin, HIGH);
	delay(250);

	// Now pull it low for ~20 milliseconds
	pinMode(_pin, OUTPUT);
	digitalWrite(_pin, LOW);
	delay(20);
	noInterrupts();
	digitalWrite(_pin, HIGH);
	delayMicroseconds(40);
	pinMode(_pin, INPUT);

	// Read in timings: how many timing transitions we need to keep track of. 2 * number bits + extra
	for (uint8_t i = 0; i < 85; i++)
	{
		counter = 0;
		while (digitalRead(_pin) == laststate)
		{
			counter++;
			delayMicroseconds(1);
			if (counter == 255)
				break;
		}
		laststate = digitalRead(_pin);

		if (counter == 255) break;

		// ignore first 3 transitions
		if ((i >= 4) && (i%2 == 0))
		{
			// shove each bit into the storage bytes
			data[j/8] <<= 1;
			if (counter > 6)
				data[j/8] |= 1;
			j++;
		}
	}
	interrupts();

	if (j != 40)	// 40 bits = 5 bytes read?
		return false;

	if (data[4] != ((data[0] + data[1] + data[2] + data[3]) & 0xFF)) // CRC
		return false;

	humidity = (data[0] << 8) | data[1];
	humidity /= 10;

	temperature = ((data[2] & 0x7F) << 8) |  data[3];
	temperature /= 10;
	if (data[2] & 0x80)
		temperature *= -1;

	return true;
}
