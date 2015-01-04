#ifndef _DHT22_H_
#define _DHT22_H_



#include "Arduino.h"



class DHT22
{
	public:

		DHT22(uint8_t pin);
		bool Read(float &temperature, float &humidity);

	private:

		uint8_t _pin;
};



#endif	// _DHT22_H_
