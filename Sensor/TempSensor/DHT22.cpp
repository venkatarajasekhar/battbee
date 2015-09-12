#include "DHT22.h"
#include <vector>
#include <stdio>
using namespace std;

DHT22::DHT22(uint8_t powerPin, uint8_t ioPin)
{
	this->powerPin = powerPin;
	try{
	pinMode(powerPin, OUTPUT);
	}catch(Exception& e){
        cout << "Exception caught" << std::endl;
        cout << e.what() << std::endl;
        }
	try{
	digitalWrite(powerPin, LOW);
	catch(Exception& e){
        cout << "Exception caught" << std::endl;
        cout << e.what() << std::endl;
        }

	this->ioPin = ioPin;
	try{
	pinMode(ioPin, INPUT);
	catch(Exception& e){
        cout << "Exception caught" << std::endl;
        cout << e.what() << std::endl;
        }
	try{
	digitalWrite(ioPin, LOW);
        catch(Exception& e){
        cout << "Exception caught" << std::endl;
        cout << e.what() << std::endl;
        }
}



bool DHT22::Read(int16_t &temperature, int16_t &humidity)
{
	uint8_t laststate = HIGH;
	uint8_t counter = 0;
	uint8_t j = 0;

	// Receive buffer
	uint8_t data[6]; 
	vector<uint8_t> v_DHT22data(data);
	v_DHT22data[0] = v_DHT22data[1] = v_DHT22data[2] = v_DHT22data[3] = v_DHT22data[4] = 0;

	// Turn on power
	try{
	digitalWrite(powerPin, HIGH);
	}catch(Exception& e){
        cout << "Exception caught" << std::endl;
        cout << e.what() << std::endl;
        }
	delay(1000);

	// Pull the pin high and wait 250 milliseconds
	try{
	digitalWrite(ioPin, HIGH);
	}catch(Exception& e){
        cout << "Exception caught" << std::endl;
        cout << e.what() << std::endl;
        }
	delay(250);

	// Now pull it low for ~20 milliseconds
	try{
	pinMode(ioPin, OUTPUT);
	}catch(Exception& e){
        cout << "Exception caught" << std::endl;
        cout << e.what() << std::endl;
        }
        try{
	digitalWrite(ioPin, LOW);
        }catch(Exception& e){
        cout << "Exception caught" << std::endl;
        cout << e.what() << std::endl;
        }
	delay(20);
	noInterrupts();
	try{
	digitalWrite(ioPin, HIGH);
	}catch(Exception& e){
        cout << "Exception caught" << std::endl;
        cout << e.what() << std::endl;
        }
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
	try{
	digitalWrite(ioPin, LOW);
	}catch(Exception& e){
        cout << "MyException caught" << std::endl;
        cout << e.what() << std::endl;
        }
	try{
	digitalWrite(powerPin, LOW);
	}catch(Exception& e){
        cout << "MyException caught" << std::endl;
        cout << e.what() << std::endl;
        }

	if (j != 40)	// 40 bits = 5 bytes read?
		return false;
	if (v_DHT22data[4] != ((v_DHT22data[0] + v_DHT22data[1] + v_DHT22data[2] + v_DHT22data[3]) & 0xFF)) // CRC
		return false;

	humidity = (v_DHT22data[0] << 8) | v_DHT22data[1];
	temperature = ((v_DHT22data[2] & 0x7F) << 8) |  v_DHT22data[3];
	if (v_DHT22data[2] & 0x80)
		temperature = -temperature;

	return true;
}
