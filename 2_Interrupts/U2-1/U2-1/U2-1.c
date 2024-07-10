#include <avr/io.h>
#include <avr/interrupt.h>

// F_CPU has to be defined before include; otherwise error because F_CPU would be defined by util/delay.h if not already defined before
#define F_CPU 16000000UL
#include <util/delay.h>

/*
	LEDA1 AN PB0
	LEDA4 AN PB1
	LEDA7 AN PB2
	TASTER A3 AN PCINT8/ PC0/ ADC0
	TASTER A4 AN INT0/ PD2/ PCINT18
*/

#define LED0_MODE DDB0
#define LED0_VAL PORTB0
#define LED1_MODE DDB1
#define LED1_VAL PORTB1
#define LED2_MODE DDB2
#define LED2_VAL PORTB2

#define DIP1_VAL PIND3
#define DIP1_PULL_UP PORTD3
#define DIP2_VAL PIND4
#define DIP2_PULL_UP PORTD4
#define DIP3_VAL PIND5
#define DIP3_PULL_UP PORTD5

#define TASTER_A3 PINC0
#define TASTER_A3_PULL_UP PORTC0
#define TASTER_A4 PIND2
#define TASTER_A4_PULL_UP PORTD2

void getDipValues();
void setLEDS();


// If the counter is running or not
volatile uint8_t running = 1;
volatile uint8_t count = 7;

ISR(INT0_vect){
	running = 1;
}
ISR(PCINT1_vect) {
	running = 0;
	getDipValues();
	setLEDS();
}

int main(void){
	// Activate Output and Input Pull-Ups and init
	DDRB |= ((1 << LED0_MODE) | (1 << LED1_MODE) | (1 << LED2_MODE));
	PORTB &= ~((1 << LED0_VAL) | (1 << LED1_VAL) | (1 << LED2_VAL));
	PORTD |= ((1 << DIP1_PULL_UP) | (1 << DIP2_PULL_UP) | (1 << DIP3_PULL_UP) | (1 << TASTER_A4_PULL_UP));
	PORTC |= (1 << TASTER_A3_PULL_UP);
	
	// Set INT0 to detect falling edge
	EICRA |= (1 << ISC01);
	
	// Enable local interrupt for INT0
	EIMSK |= (1 << INT0);
	
	// Enable interrupts for PCINT14 - PCINT8
	PCICR |= (1 << PCIE1);
	
	// Enable interrupt for PCINT8
	PCMSK1 |= (1 << PCINT8);
	
	// Enable global interrupt
	sei();

	
	while(1){
		if(running){
			count = (!count) ? 7 : (count - 1);
			setLEDS();
		}
      _delay_ms(1000);
	}
}

void getDipValues() {
	count = (~PIND >> 3); 
}

void setLEDS() {
	uint8_t tmp = PORTB;
	// 11111000 -> Clear last 3 bits
	tmp &= 0xF8;
	// 00000XXX -> Set count on tmp without modifying first 5 bits
	tmp |= (~count & 0x07);
	PORTB = tmp;
}
