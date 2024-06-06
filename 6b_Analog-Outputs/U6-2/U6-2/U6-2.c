#define F_CPU 16000000UL
#define BAUDRATE 9600 

#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "adc.h"

#define POTIPIN A2


int main(){

    // Store the ADC value
    uint16_t adcVal = 0;

    adcInit();

    // Init PWM Timer
    TCNT0 = 0;
    
	// Set OCR0A | Inverted mode because LED is wired to 5V
	OCR0A = 250;
	OCR0B = 125;
	TCCR0A |= ((1 << COM0B1) | (1 << COM0B0));
	    
	// OCR0B is on PD5
	DDRD |= (1 << PD5);
	
    // Fast PWM (WGM2:0 = 111) 1 kHz | 64 Prescaler (CS2:0 = 011)
    TCCR0A |= ((1 << WGM01) | (1 << WGM00));
    TCCR0B |= ((1 << WGM02) | (1 << CS01) | (1 << CS00));

    sei();
    while (1){
        adcRead(POTIPIN, &adcVal);

        // Convert ADC to mV
        // 250 / (2^10) = 1/4
        adcVal /= 4;
        OCR0B = (uint8_t) adcVal;

        _delay_ms(10);
    }
}


