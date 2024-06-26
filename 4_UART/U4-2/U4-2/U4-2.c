/*
 * U4_2.c
 *
 * Created: 02.05.2024 13:33:12
 *  Author: ref41518
 */ 

#define F_CPU 16000000UL
#define BAUDRATE 9600

#include "uart.h"
#include <avr/io.h>

void clearScreen(){
	// Clear terminal
	uart_send(27); // ESC
	uart_send('[');
	uart_send('2');
	uart_send('J');

	// Move cursor to start
	uart_send(27);
	uart_send('[');
	uart_send('H');
}

int main(void)
{	
	uart_init();
	sei();
	
	const char meldung[]="Hit me:";
	clearScreen();
	for(int f = 0; meldung[f]!='\0'; f++) uart_send(meldung[f]);
	
	uint8_t t;
	while (1){
		t = uart_receive();
		if (t == '\0') {
			continue;
		}
		uart_send(t);
		_delay_ms(100);
	}
}
