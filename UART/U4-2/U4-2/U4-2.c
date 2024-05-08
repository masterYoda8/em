/*
 * U4_2.c
 *
 * Created: 02.05.2024 13:33:12
 *  Author: ref41518
 */ 

#define F_CPU 16000000UL
#define BAUDRATE 9600

#define DEBUG_LED_MODE DDB0
#define DEBUG_LED_VAL PORTB0

#include "uart.h"
#include <avr/io.h>


//DEBUG
//PORTD &= ~(1 << DEBUG_LED_VAL);
//_delay_ms(100);
// PORTD |= (1 << DEBUG_LED_VAL);
// DEBUG END

int main(void)
{
	// Debug LED
	DDRD |= (1 << DEBUG_LED_MODE);
	PORTD |= (1 << DEBUG_LED_VAL);
	
	sei();
	/*
	PORTD &= ~(1 << DEBUG_LED_VAL);
	_delay_ms(1000);
	PORTD |= (1 << DEBUG_LED_VAL);
	*/
	uint8_t t;
	const char meldung[]="Hier Text:";
	uart_init();
	for(int f=0;meldung[f]!='\0';f++)
	uart_send(meldung[f]);
	while (1)
	{
		t = uart_receive();
		if (t == '\0') {
			continue;
		}
		uart_send(t);
		_delay_ms(100);
		
	}
}