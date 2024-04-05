#include <avr/io.h>

// F_CPU has to be defined before include; otherwise error because F_CPU would be defined by util/delay.h if not already defined before
#define F_CPU 16000000UL
#include <util/delay.h>

/*
	LEDA1 AN PD0
	LEDA4 AN PD1
	LEDA7 AN PD2
*/

#define LED0_MODE DDD0
#define LED0_VAL PORTD0
#define LED1_MODE DDD1
#define LED1_VAL PORTD1
#define LED2_MODE DDD2
#define LED2_VAL PORTD2


int main(void){
	// Activate Output and init
	DDRD |= ((1 << LED0_MODE) | (1 << LED1_MODE) | (1 << LED2_MODE));
	PORTD &= ~((1 << LED0_VAL) | (1 << LED1_VAL) | (1 << LED2_VAL));
	uint8_t count = 7;
	uint8_t tmp;

	while(1){
		_delay_ms(1000);
		tmp = PORTD;
		// 11111000 -> Clear last 3 bits
		tmp &= 0xF8; 
		// 00000XXX -> Set count on tmp without modifying first 5 bits
		tmp |= (~count & 0x07)
		PORTD = tmp;
		count = (!count) ? 7 : count - 1;
	}
}