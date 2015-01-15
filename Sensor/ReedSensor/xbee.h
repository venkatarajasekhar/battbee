#ifndef _XBEE_H_
#define _XBEE_H_



#include <avr/io.h>
#include <stdbool.h>



void xbee_init();
bool xbee_send(const uint8_t *buf, uint16_t len, uint64_t address);
bool xbee_sendstr(const char *str, uint64_t address);



#endif	// _XBEE_H_
