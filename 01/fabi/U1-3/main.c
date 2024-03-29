#include <avr/io.h>
#include <util/delay.h>

#define DIP0_VAL PINC0
#define DIP1_VAL PINC1
#define DIP2_VAL PINC2
#define DIP0_PULLUP PORTC0
#define DIP1_PULLUP PORTC1
#define DIP2_PULLUP PORTC2
#define DIPREG PINC

#define RESET_VAL PINC6
#define RESET_PULLUP PORTC6
#define START_VAL PINC7
#define START_PULLUP PORTC7

#define LED0_MODE DDC3
#define LED0_VAL PORTC3
#define LED1_MODE DDC4
#define LED1_VAL PORTC4
#define LED2_MODE DDC5
#define LED2_VAL PORTC5

typedef enum {  
  false,
  true
} bool;

int int main(void)
{
  
  bool Reset = false, Start = false;

  
}

// flank detection
void checkButtonVal(uint8_t portReg, uint8_t port, bool* val){
  bool currentVal = portReg & (1 << port);
  if(currentVal){
    &val = (&val) ? false : true
  } else if (&val){
    &val = false;
  }
} 


void getDipVal(uint8_t* val){
  &val = 0;
  &val |= DIPREG & ((1 << DIP0_VAL) | (1 << DIP1_VAL) | (1 << DIP2_VAL));
} 
