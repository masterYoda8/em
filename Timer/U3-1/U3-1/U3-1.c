#include <avr/io.h>
#include <avr/interrupt.h>
#include "events.h"

// F_CPU has to be defined before include; otherwise error because F_CPU would be defined by util/delay.h if not already defined before
#define F_CPU 16000000UL
#include <util/delay.h>

/*
	LEDA1 AN PB0
	LEDA4 AN PB1
	LEDA7 AN PB2
	TASTER A3 AN PCINT8/ PC0/ ADC0
	TASTER A4 AN INT0/ PD2/ PCINT18
	DIP0 AN PD3
	DIP1 AN PD4
	DIP2 AN PD5
*/

#define LED0_MODE DDB0
#define LED0_VAL PORTB0
#define LED1_MODE DDB1
#define LED1_VAL PORTB1
#define LED2_MODE DDB2
#define LED2_VAL PORTB2
#define DEBUG_LED_MODE DDD0
#define DEBUG_LED_VAL PORTD0

#define DIP0_VAL PIND3
#define DIP0_PULL_UP PORTD3
#define DIP1_VAL PIND4
#define DIP1_PULL_UP PORTD4
#define DIP2_VAL PIND5
#define DIP2_PULL_UP PORTD5

#define TASTER_A3 PINC0
#define TASTER_A3_PULL_UP PORTC0
#define TASTER_A4 PIND2
#define TASTER_A4_PULL_UP PORTD2

void getDipValues();
void setLEDS();

// If the counter is running or not
volatile uint8_t running = 1;
volatile uint8_t count;
volatile uint8_t buttonReset = 1;
volatile uint8_t buttonStart = 1;

event_type CHECK_BUTTON = 1;
event_type COUNTDOWN_EV = 2;

ISR(TIMER0_OVF_vect){
	static uint16_t countdown = 1000;
	countdown--;
	
	if (!(countdown % 50)) {
		setEvent(CHECK_BUTTON);
	}
	
	if(!countdown) {
		setEvent(COUNTDOWN_EV);
		countdown = 1000;
	}
	
	TCNT0 = 6;
}

ISR(PCINT2_vect){
	uint8_t softwareInterrupt = (PORTD & (1 << PORTD1));
	if (softwareInterrupt) {
			//DEBUG
			//PORTD &= ~(1 << DEBUG_LED_VAL);
			//_delay_ms(100);
			//PORTD |= (1 << DEBUG_LED_VAL);
			//DEBUG END
			count = 7;
			PORTD &= ~(1 << PORTD1);
			setLEDS();
	}
}

int main(void){
	// Debug LED
	DDRD |= (1 << DEBUG_LED_MODE);
	PORTD |= (1 << DEBUG_LED_VAL); 
	
	
	// Activate Output and Input Pullups and init
	DDRB |= ((1 << LED0_MODE) | (1 << LED1_MODE) | (1 << LED2_MODE));
	DDRD |= (1 << PORTD1);
	PORTB &= ~((1 << LED0_VAL) | (1 << LED1_VAL) | (1 << LED2_VAL));
	PORTD |= ((1 << DIP0_PULL_UP) | (1 << DIP1_PULL_UP) | (1 << DIP2_PULL_UP) | (1 << TASTER_A4_PULL_UP));
	PORTC |= (1 << TASTER_A3_PULL_UP);
	
	// Enable interrupts for PCINT23 - PCINT16
	PCICR |= (1 << PCIE2);
	
	// Enable interrupt for PCINT17
	PCMSK2 |= (1 << PCINT17);
	
	TCCR0B |= ((1 << CS01) | (1 << CS02));
	TCNT0 = 6;
	TIMSK0 |= (1 << TOIE0);
	
	
	// Enable global interrupt
	sei();

	count = 8;

	while(1){
		if(eventIsSet(CHECK_BUTTON)) {
			checkButtons();
			clearEvent(CHECK_BUTTON);
		}
		if (eventIsSet(COUNTDOWN_EV)){
			countdown():
			clearEvent(COUNTDOWN_EV);
		}
		//_delay_ms(1000);
	}
}

void countdown() {
	if(running) {
		if(count) {
			count--;
			setLEDS();
		} else {
			// Trigger interrupt
			PORTD |= (1 << PORTD1);
		}
	}
}

void checkButtons(){
	uint8_t resetButtonPressed = (~PINC) & (1 << PINC0);
	if (buttonReset && !resetButtonPressed) {
		running = 0;
		getDipValues();
		setLEDS();
	}
	buttonReset = resetButtonPressed;
	
	uint8_t startButtonPressed = (~PINB) & (1 << PINB0);
	if (buttonStart && !startButtonPressed) {
		running = 1;
	}
	buttonStart = startButtonPressed;
	
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