#include "XBee.h"



void setup()
{
	Serial.begin(9600);
}

void loop()
{
	XBeeTransmit((uint8_t *)"AAA", 4);
	delay(3000);
}
