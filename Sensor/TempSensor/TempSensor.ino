//#include "XBee.h"
#include "DHT22.h"

DHT22 dht(5, 4);

void setup()
{
	Serial.begin(9600);
}

void loop()
{
	//XBeeTransmit((uint8_t *)"AAA", 4);
	//delay(3000);

	int16_t h;
	int16_t t;
	if (!dht.Read(t, h))
	{
		Serial.println("Failed to read from DHT");
	}
	else
	{
		Serial.print("Humidity: "); 
		Serial.print(h);
		Serial.print(" %\t");
		Serial.print("Temperature: "); 
		Serial.print(t);
		Serial.println(" *C");
	}
	delay(2000);
}
