#include "XBee.h"



// MSB..LSB addressed by index 0..n; modify for big/small endian systems
#define GET_BYTE(X, I)		(((X) >> ((sizeof(X) - (I) - 1) << 3)) & 0xff)
#define SET_BYTE(X, I, B)	((X) |= ((B) << ((sizeof(X) - (I) - 1) << 3)))



static void XBeeSend(uint8_t value)
{
	Serial1.write(value);
}



static uint8_t XBeeRecv()
{
	while (true)
	{
		int value = Serial1.read();
		if (value == -1)
			delay(1);
		else
			return (uint8_t)value;
	}
}



bool XBeeTransmit(const uint8_t *buf, uint16_t len, uint64_t address)
{
	// Send transmit request
	XBeeSend(0x7e);	// Start delimiter
	uint16_t totalLen = len + 14;
	XBeeSend(GET_BYTE(totalLen, 0));	// Length MSB
	XBeeSend(GET_BYTE(totalLen, 1));	// Length LSB
	static uint8_t frameID = 1;
	uint8_t checksum = 0x10 + frameID + 0xff + 0xfe + 0x00 + 0x00;
	XBeeSend(0x10);	// Frame type: ZigBee Transmit Request
	XBeeSend(frameID);	// Frame ID
	for (uint16_t i = 0; i < 8; i++)
	{
		uint8_t value = GET_BYTE(address, i); // 64 bit destination address: MSB..LSB
		XBeeSend(value);
		checksum += value;
	}
	XBeeSend(0xff);	// 16-bit destination address MSB: Unknown
	XBeeSend(0xfe);	// 16-bit destination address LSB
	XBeeSend(0x00);	// Broadcast radius
	XBeeSend(0x00);	// Options
	for (uint16_t i = 0; i < len; i++)
	{
		XBeeSend(buf[i]);	// RF data
		checksum += buf[i];
	}
	XBeeSend(0xff - checksum);	// Checksum

	// Receive frames util we get the transmit status 
	bool exitLoop, success;
	while (true)
	{
		// Omit all bytes before frame delimiter
		while (XBeeRecv() != 0x7e);
		exitLoop = true; success = true;	// Be optimistic ...
		totalLen = 0;
		SET_BYTE(totalLen, 0, (uint16_t)XBeeRecv());	// Length MSB
		SET_BYTE(totalLen, 1, (uint16_t)XBeeRecv());	// Length LSB
		if (totalLen != 7)	// Size of answer
			exitLoop = false;
		for (uint16_t i = 0; i < totalLen; i++)
		{
			uint8_t value = XBeeRecv();
			if ((i == 0) && (value != 0x8b))	// Frame type: ZigBee Transmit Status
				exitLoop = false;
			if ((i == 1) && (value != frameID))	// Correct Frame ID
				exitLoop = false;
			if (i == 5)
				success = (value == 0); // Delivery Status: OK
		}
		XBeeRecv();	// Checksum
		if (exitLoop)
			break;
	}
	frameID = (frameID == 0xff) ? 1 : (frameID + 1);
	return success;
}

