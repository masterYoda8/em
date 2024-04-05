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

#define DIP1_VAL PIND3
#define DIP1_PULL_UP PORTD3
#define DIP2_VAL PIND4
#define DIP2_PULL_UP PORTD4
#define DIP3_VAL PIND5
#define DIP3_PULL_UP PORTD5

#define TASTER_A3 PIND6
#define TASTER_A3_PULL_UP PORTD6
#define TASTER_A4 PIND7
#define TASTER_A4_PULL_UP PORTD7

inline void getDipValues(uint8_t*);
inline void setLEDS(uint8_t *);
inline void getButtonValues(uint8_t *, uint8_t *);

int main(void){
	// Activate Output and Input Pullupss and init
	DDRD |= ((1 << LED0_MODE) | (1 << LED1_MODE) | (1 << LED2_MODE));
	PORTD &= ~((1 << LED0_VAL) | (1 << LED1_VAL) | (1 << LED2_VAL));
	PORTD |= ((1 << DIP1_PULL_UP) | (1 << DIP2_PULL_UP) | (1 << DIP3_PULL_UP) | (1 << TASTER_A3_PULL_UP) | (1 << TASTER_A4_PULL_UP));
	uint8_t count = 7;
	// Caches previous button presses -- 1 = not pressed; 0 = pressed
	uint8_t pressed = 0x02;
	// Flank detection value 0: no flank; 1: flank detected
	uint8_t values = 0;
	// If the counter is running or not
	uint8_t running = 1;
	
	while(1){
		start:
		for (int i = 0; i < 10; i++){
			getButtonValues(&pressed, &values);
			// if timer is running and button A3 pressed
			if(running && (~values) & 0x01){
				running = 0;
				getDipValues(&count);
				setLEDS(&count);
				goto start
			} 
			// if not running and button A4 pressed
			else if (!running && (~values) & 0x02){
				running = 1;
				goto start
			}
			_delay_ms(100);
		}

		if(running){
			count = (!count) ? 7 : count - 1;
			setLEDS(&count);
		}
	}
}

inline void getDipValues(uint8_t *reg) {
	*reg = (~PIND >> 3); 
}

inline void setLEDS(uint8_t *values) {
	uint8_t tmp = PORTD;
	// 11111000 -> Clear last 3 bits
	tmp &= 0xF8;
	// 00000XXX -> Set count on tmp without modifying first 5 bits
	tmp |= (~(*values) & 0x07);
	PORTD = tmp;
}

inline void getButtonValues(uint8_t *pressed, uint8_t *values) {
	uint8_t tmp = (PIND >> 6);
	// Reset values
	*values &= ~(0x03);
	// Taster A3 at 0x01 positive flank
	if (~tmp & (0x01) && ((*pressed) & (0x01))) {
		*values |= 0x01;
	}
	// Taster A4 at 0x02 positive flank
	if (~tmp & (0x02) && ((*pressed) & (0x02))) {
		*values |= 0x02;
	}
	*pressed = tmp;
}