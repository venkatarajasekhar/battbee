#include <Arduino.h>



#define StartDelimiter 0x7e

// MSB..LSB addressed by index 0..n; modify for big/small endian systems
#define GET_BYTE(X, I)		(((X) >> ((sizeof(X) - (I) - 1) << 3)) & 0xff)
#define SET_BYTE(X, I, B)	((X) |= ((B) << ((sizeof(X) - (I) - 1) << 3)))

uint8_t frameID = 1;
#define UPDATE_FRAMEID()	{frameID = (frameID == 0xff) ? 1 : (frameID + 1);}

#define AT_COMMAND	0x08
#define AT_COMMAND_RESPONSE	0x88
#define ZIGBEE_TRANSMIT_REQUEST	0x10
#define ZIGBEE_TRANSMIT_STATUS	0x8B



void HexDump(uint8_t *buf, uint16_t len)
{
	for (uint16_t i = 0; i < len; i++)
	{
		Serial.print(buf[i], HEX);
		Serial.print(" ");
	}
	Serial.print(" -> ");
	Serial.print(len);
	Serial.println(" bytes in total");
}

void SendByte(uint8_t value)
{
	Serial1.write(value);
}

void SendFrame(uint8_t *buf, uint16_t len)
{
	// Send frame delimiter
	SendByte(StartDelimiter);
	// Send frame size
	SendByte(GET_BYTE(len, 0));
	SendByte(GET_BYTE(len, 1));
	// Send frame data
	uint8_t checksum = 0;
	for (uint16_t i = 0; i < len; i++)
	{
		SendByte(buf[i]);
		checksum += buf[i];
	}
	// Send checksum
	SendByte(((uint8_t)0xff) - checksum);
}

uint8_t ReceiveByte()
{
	while (true)
	{
		int value = Serial1.read();
		if (value == -1)
			delay(10);
		else
			return (uint8_t)value;
	}
}

bool ReceiveFrame(uint8_t *buf, uint16_t *len)
{
	// Omit all bytes before frame delimiter
	*len = 0;
	while (ReceiveByte() != StartDelimiter);
	// Receive frame size
	SET_BYTE(*len, 0, (uint16_t)ReceiveByte());
	SET_BYTE(*len, 1, (uint16_t)ReceiveByte());
	// Receive frame data
	uint8_t checksum = 0;
	for (uint16_t i = 0; i < *len; i++)
	{
		buf[i] = ReceiveByte();
		checksum += buf[i];
	}
	// Receive and check checksum
	checksum += ReceiveByte();
	if (checksum != 0xff)
	{
		*len = 0;
		return false;
	}
	return true;
}

bool AtCommand(const char *cmd, const uint32_t *param=NULL, uint32_t *result=NULL)
{
	uint8_t buf[100];
	uint16_t len = 0;
	buf[0] = AT_COMMAND;
	buf[1] = frameID;
	buf[2] = cmd[0];
	buf[3] = cmd[1];
	len = 4;
	if (param != NULL)
	{
		for (uint16_t i = 0; i < sizeof(uint32_t); i++)
			buf[len++] = GET_BYTE(*param, i);
	}
	HexDump(buf, len);
	SendFrame(buf, len);

	if (!ReceiveFrame(buf, &len))
		return false;
	HexDump(buf, len);
	if (len < 5)
		return false;
	if ((buf[0] != AT_COMMAND_RESPONSE) ||
		(buf[1] != frameID) ||
		(buf[2] != cmd[0]) ||
		(buf[3] != cmd[1]) ||
		(buf[4] != 0))	// OK
		return false;
	if (result != NULL)
	{
		*result = 0;
		// Just use lower 32 bits; 64 bit support in Arduino seems incomplete
		for (uint16_t i = 0; i < min(sizeof(uint32_t), len - 5); i++)
			SET_BYTE(*result, i, (uint32_t)buf[len - (5-1) + i]);
	}

	UPDATE_FRAMEID();
	return true;
}

bool Transmit(uint8_t *buf, uint16_t len, uint64_t address=0)
{
	uint8_t xferBuf[100];
	xferBuf[0] = ZIGBEE_TRANSMIT_REQUEST;
	xferBuf[1] = frameID;
	for (uint16_t i = 0; i < 8; i++)
		xferBuf[2 + i] = GET_BYTE(address, i);
	xferBuf[10] = 0xff; // 16 bit destination address MSB, if known
	xferBuf[11] = 0xfe; // 16 bit destination address LSB, if known
	xferBuf[12] = 0x00; // Broadcast radius
	xferBuf[13] = 0x00; // Options
	for (uint16_t i = 0; i < len; i++)
		xferBuf[14 + i] = buf[i];
	HexDump(xferBuf, 14 + len);
	SendFrame(xferBuf, 14 + len);

	if (!ReceiveFrame(xferBuf, &len))
		return false;
	HexDump(xferBuf, len);
	if (len != 7)
		return false;
	if ((xferBuf[0] != ZIGBEE_TRANSMIT_STATUS) ||
		(xferBuf[1] != frameID) ||
		(xferBuf[5] != 0))	// OK
		return false;

	UPDATE_FRAMEID();
	return true;
}

bool TransmitStr(char *str, uint64_t destAddress=0)
{
	return Transmit((uint8_t *)str, strlen(str) + 1, destAddress);
}

void setup()
{
	Serial.begin(9600);

	Serial1.begin(9600);
	uint32_t id=0x2001;
	AtCommand("ID", &id);
}

void loop()
{
	uint32_t sl;
	Serial.println(AtCommand("SL", NULL, &sl));
	delay(3000);
	Serial.println(TransmitStr("AAA"));
	delay(3000);
}
