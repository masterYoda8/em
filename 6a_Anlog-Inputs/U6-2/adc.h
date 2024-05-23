#ifndef _ADC_H_
#define _ADC_H_

#include <avr/io.h>
#include <util/delay.h>

typedef enum {
    A0 = 0,
    A1,
    A2,
    A3,
    A4,
    A5,
    A6,
    A7,
    TEMPERATURE
} ADC_PORT;

void adcInit(){
    // Enable ADC
    PRR &= ~(1<<PRADC);
    ADCSRA |= (1 << ADEN);

    // Set prescaler frequency to 125 kHz - (16 MHz / 125 kHz = 128)
    ADCSRA |= ((1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0));
}

inline void adcSetPort(ADC_PORT port){
    //clear regs
    ADMUX &= ~((1 << MUX3) | (1 << MUX2) | (1 << MUX1) | (1 << MUX0));
    // enum is uint8_t first 4 bits are always empty
    ADMUX |= port;
}

inline void adcSetRevVoltage(ADC_PORT port){
    if(port == TEMPERATURE){
        // 1.1V Reference Voltage
        ADMUX = ((1 << REFS1) | (1 << REFS0));
    } else {
        // AVCC Pin
        ADMUX &= ~(1 << REFS1) | ;
        //ADMUX |= (1 << REFS0);
    }
}

void adcRead(ADC_PORT port, uint16_t* value){
    adcSetRevVoltage(port);
    adcSetPort(port);

    _delay_ms(5);

    // Start conversion
    ADCSRA |= (1 << ADSC);

    // Wait until conversion is finished
    while(ADCSRA & (1 << ADSC)) ;

    // Read value

    *value = 0;
    *value = ADCL;
    *value |= (ADCH << 8);
}

#endif // _ADC_H_