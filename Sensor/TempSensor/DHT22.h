#ifndef _DHT22_H_
#define _DHT22_H_



#include "Arduino.h"



class DHT22
{
	public:

		DHT22(uint8_t powerPin, uint8_t ioPin);
		bool Read(int16_t &temperature, int16_t &humidity);

	private:

		uint8_t powerPin;
		uint8_t ioPin;
};



#endif	// _DHT22_H_
