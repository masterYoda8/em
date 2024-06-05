#define F_CPU 16000000UL
#define BAUDRATE 9600 

#include <stdio.h>
#include <avr/io.h>

#include "uart.h"
#include "uart-utils.h"
#include "adc.h"



#define POTIPIN A2

const char menuLine1[] PROGMEM = "ADC Results";
const char menuLine2[] PROGMEM = "------------------";
const char menuLine3[] PROGMEM = "Temperature: ";
const char menuLine4[] PROGMEM = "Potentiometer: ";

int main(){

    uint16_t adcVal = 0;
    char msgVal[20] = {0};

    uart_init();
    adcInit();
    sei();

    while (1){
        sendPGMString(menuLine1);
        sendCRLF();
        sendPGMString(menuLine2);
        sendCRLF();

        sendPGMString(menuLine3);
        adcRead(TEMPERATURE, &adcVal);
        
        // Convert the temprature
        // 242mV +45 + 1 LSB Offset Error
        adcVal -= 288;

        snprintf(msgVal, 20, "%d °C", (int16_t) adcVal);
        sendString(msgVal);
        sendCRLF();

        sendPGMString(menuLine4);
        adcRead(POTIPIN, &adcVal);

        // Convert ADC to mV
        // 1.100 / (2^10)
        adcVal *= 1.07421875;

        snprintf(msgVal, 20, "%d mV", adcVal);
        sendString(msgVal);
        sendCRLF();

        _delay_ms(1000);
        clearScreen();
    }
}