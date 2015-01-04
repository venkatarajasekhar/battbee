#include "DHT22.h"



DHT22::DHT22(uint8_t powerPin, uint8_t ioPin)
{
	this->powerPin = powerPin;
	pinMode(powerPin, OUTPUT);
	digitalWrite(powerPin, LOW);

	this->ioPin = ioPin;
	pinMode(ioPin, INPUT);
	digitalWrite(ioPin, LOW);
}



bool DHT22::Read(int16_t &temperature, int16_t &humidity)
{
	uint8_t laststate = HIGH;
	uint8_t counter = 0;
	uint8_t j = 0;

	// Receive buffer
	uint8_t data[6];
	data[0] = data[1] = data[2] = data[3] = data[4] = 0;

	// Turn on power
	digitalWrite(powerPin, HIGH);
	delay(1000);

	// Pull the pin high and wait 250 milliseconds
	digitalWrite(ioPin, HIGH);
	delay(250);

	// Now pull it low for ~20 milliseconds
	pinMode(ioPin, OUTPUT);
	digitalWrite(ioPin, LOW);
	delay(20);
	noInterrupts();
	digitalWrite(ioPin, HIGH);
	delayMicroseconds(40);
	pinMode(ioPin, INPUT);

	// Read in timings: how many timing transitions we need to keep track of. 2 * number bits + extra
	for (uint8_t i = 0; i < 85; i++)
	{
		counter = 0;
		while (digitalRead(ioPin) == laststate)
		{
			counter++;
			delayMicroseconds(1);
			if (counter == 255)
				break;
		}
		laststate = digitalRead(ioPin);

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

	// Turn off power
	digitalWrite(ioPin, LOW);
	digitalWrite(powerPin, LOW);

	if (j != 40)	// 40 bits = 5 bytes read?
		return false;
	if (data[4] != ((data[0] + data[1] + data[2] + data[3]) & 0xFF)) // CRC
		return false;

	humidity = (data[0] << 8) | data[1];
	temperature = ((data[2] & 0x7F) << 8) |  data[3];
	if (data[2] & 0x80)
		temperature = -temperature;

	return true;
}
