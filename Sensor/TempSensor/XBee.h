#ifndef _XBEE_H_
#define _XBEE_H_


#include <Arduino.h>


bool XBeeTransmit(const uint8_t *buf, uint16_t len, uint64_t address=0);



#endif	// _XBEE_H_
