#include <Arduino.h>

#include "Port.h"

void ShowBuffer(uint8 *buffer, uint16 size)
{
	for (unsigned int i = 0; i < size; i++)
	{
		Serial.print(buffer[i], HEX);
		Serial.print(" ");
	}
	Serial.print(" -> ");
	Serial.print(size);
	Serial.println(" bytes in total");
}

// --------- Basic sending ---------

void SendByte(uint8 value)
{
	Serial1.write(value);
}

uint8 sndBuffer[100];
uint16 sndBufferSize;

void SendFrame()
{
	// Send frame delimiter
	SendByte(0x7e);
	// Send frame size
	SendByte(((uint8 *)&sndBufferSize)[1]);
	SendByte(((uint8 *)&sndBufferSize)[0]);
	// Send frame data
	uint8 checksum = 0;
	for (uint16 i = 0; i < sndBufferSize; i++)
	{
		SendByte(sndBuffer[i]);
		checksum += sndBuffer[i];
	}
	// Send checksum
	SendByte(0xff - checksum);
}

// --------- Basic receiving ---------

uint8 ReceiveByte()
{
	while (true)
	{
		int value = Serial1.read();
		if (value == -1)
			delay(10);
		else
			return (uint8)value;
	}
}

uint8 rcvBuffer[100];
uint16 rcvBufferSize;

uint8 frameID = 1;

void ReceiveFrame()
{
	// Omit all bytes before frame delimiter
	while (ReceiveByte() != 0x7e);
	// Receive frame size
	((uint8 *)&rcvBufferSize)[1] = ReceiveByte();
	((uint8 *)&rcvBufferSize)[0] = ReceiveByte();
	// Receive frame data
	uint8 checksum = 0;
	for (uint16 i = 0; i < rcvBufferSize; i++)
	{
		rcvBuffer[i] = ReceiveByte();
		checksum += rcvBuffer[i];
	}
	// Receive and check checksum
	checksum += ReceiveByte();
	if (checksum != 0xff)
		rcvBufferSize = 0;
}

// --------- Service commands ---------

int32 AtCommand(const char *cmd, int32 param=-1)
{
	sndBuffer[0] = 0x08;	// AT command
	sndBuffer[1] = frameID;
	sndBuffer[2] = cmd[0];
	sndBuffer[3] = cmd[1];
	sndBufferSize = 4;
	if (param != -1)
	{
		if ((((uint8 *)&param)[3] != 0) || (((uint8 *)&param)[2] != 0))
		{
			// Send parameter as 32-bit
			sndBuffer[4] = ((uint8 *)&param)[3];
			sndBuffer[5] = ((uint8 *)&param)[2];
			sndBuffer[6] = ((uint8 *)&param)[1];
			sndBuffer[7] = ((uint8 *)&param)[0];
			sndBufferSize += 4;
		}
		else if (((uint8 *)&param)[1] != 0)
		{
			// Send parameter as 16-bit
			sndBuffer[4] = ((uint8 *)&param)[1];
			sndBuffer[5] = ((uint8 *)&param)[0];
			sndBufferSize += 2;
		}
		else
		{
			// Send parameter as 8-bit
			sndBuffer[4] = ((uint8 *)&param)[0];
			sndBufferSize += 1;
		}
	}
	ShowBuffer(sndBuffer, sndBufferSize);
	SendFrame();

	ReceiveFrame();
	ShowBuffer(rcvBuffer, rcvBufferSize);
	if (rcvBufferSize < 5)
		return -1;
	if ((rcvBuffer[0] != 0x88) || // AT command response
		(rcvBuffer[1] != frameID) ||
		(rcvBuffer[2] != cmd[0]) ||
		(rcvBuffer[3] != cmd[1]) ||
		(rcvBuffer[4] != 0))	// OK
		return -1;
	frameID = (frameID == 0xff) ? 1 : (frameID + 1);

	int32 result = 0;
	for (uint16 i = 0; i < rcvBufferSize - 5; i++)
		((uint8 *)&result)[i] = rcvBuffer[rcvBufferSize - i - 1];
	return result;
}

bool Transmit(uint8 *buf, uint16 bufSize, uint64 destAddress=0)
{
	sndBuffer[0] = 0x10;	// ZigBee transmit request
	sndBuffer[1] = frameID;
	for (uint16 i = 0; i < 8; i++)
		sndBuffer[2 + i] = ((uint8 *)&destAddress)[8 - i - 1];
	sndBuffer[10] = 0xff; // 16 bit destination address MSB, if known
	sndBuffer[11] = 0xfe; // 16 bit destination address LSB, if known
	sndBuffer[12] = 0x00; // Broadcast radius
	sndBuffer[13] = 0x00; // Options
	for (uint16 i = 0; i < bufSize; i++)
		sndBuffer[14 + i] = buf[i];
	sndBufferSize = 14 + bufSize;
	ShowBuffer(sndBuffer, sndBufferSize);
	SendFrame();

	ReceiveFrame();
	ShowBuffer(rcvBuffer, rcvBufferSize);

	frameID = (frameID == 0xff) ? 1 : (frameID + 1);

	return true;
}

bool TransmitStr(char *str, uint64 destAddress=0)
{
	return Transmit((uint8 *)str, strlen(str) + 1, destAddress);
}

void setup()
{
	Serial.begin(9600);

	Serial1.begin(9600);
	AtCommand("ID", 0x2001);
}

void loop()
{
	TransmitStr("AAA");
	delay(3000);
}

