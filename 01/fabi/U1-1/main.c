#include <avr/io.h>
#include <util/delay.h>

#define LED0_MODE DDC0
#define LED0_VAL PORTC0
#define LED1_MODE DDC1
#define LED1_VAL PORTC1
#define LED2_MODE DDC2
#define LED2_VAL PORTC2

inline void clearBits();

int main(void){
    // Activate Output and init
    DDRC |= ((1 << LED0_MODE) | (1 << LED1_MODE) | (1 << LED2_MODE));
    PORTC |= ((1 << LED0_VAL) | (1 << LED1_VAL) | (1 << LED2_VAL));
    uint8_t count = 0;

    while(1){
        _delay_ms(1000);
        clearBits();
        PORTC |= count & (1 << LEDO_VAL);
        PORTC |= count & (1 << LED1_VAL);
        PORTC |= count & (1 << LED2_VAL);
        count++;
    }
}

inline void clearBits(){
    // Set bits to zero
    PORTC &= ~((1 << LED0_VAL) | (1 << LED1_VAL) | (1 << LED2_VAL));
}
