#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
  // Set pin 13 as output
  DDRB |= (1<<PB5);


  // Flash on board LED atmega328p arduino pro mini
  while(1){
    PORTB |= (1<<PB5);
    _delay_ms(1000);
    PORTB &= ~(1<<PB5);
    _delay_ms(1000);
  }
    
}
