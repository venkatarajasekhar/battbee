#ifndef _SERIAL_H_
#define _SERIAL_H_



#include <avr/io.h>



extern void serial_init();
extern uint8_t serial_receive();
extern void serial_send(uint8_t value);
extern void serial_sendstr(char* str);



#endif	// _SERIAL_H_
