
#define F_CPU 16000000UL
#define BAUDRATE 9600

typedef enum{
	false,
	true
} bool;

#include "events.h"
#include "timer.h"
#include "uart.h"
#include <avr/pgmspace.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h> 
#include <math.h>



//DEBUG
//PORTD &= ~(1 << DEBUG_LED_VAL);
//_delay_ms(100);
// PORTD |= (1 << DEBUG_LED_VAL);
// DEBUG END

void printMenu();
inline void sendCRLF();
inline void sendPGMString(const char *);
void getCommand();

const char menuTextHeader[] PROGMEM = "Fancy Menu:\0";
const char menuTextDivider[] PROGMEM = "-----------------";
const char menuTextAction[]PROGMEM = "Choose an action (1-4)";
const char menuTextAction1[] PROGMEM = "1 - Start Timer";
const char menuTextAction2[] PROGMEM = "2 - Stop Timer";
const char menuTextAction3[] PROGMEM = "3 - Configure Starting Time";
const char menuTextAction4[] PROGMEM = "4 - Print Starting Time";
const char menuTextInput[] PROGMEM = "Input (1-4): ";
const char menuTextReturn[] PROGMEM = "Press RETURN to go back to Main Menu";
const char menuTextConfigTime[] PROGMEM = "Input time in ms: ";

const char* const menu[] PROGMEM = {menuTextHeader, menuTextDivider, menuTextAction, \
    menuTextAction1, menuTextAction2, menuTextAction3, menuTextAction4};
const uint8_t menuSize PROGMEM = 7;

char msg[30] = {0};

int main(void){
	// Debug LED
	//DDRD |= (1 << DEBUG_LED_MODE);
	//PORTD |= (1 << DEBUG_LED_VAL); 
	
	timer_init();
    uart_init();


	// Declare 50 ms timer for button flank detection
	//declareTimer(TIMER1, 50000, checkButtons);
	// Declare 1s timer for counting down
	//declareTimer(TIMER2, 1000000, countdown);

	// Enable global interrupt
	sei();
	
	//startTimer(TIMER1);
	//startTimer(TIMER2);
	while(1){
        printMenu();
        getCommand();
	}
}

void printMenu(){
    uint8_t end = pgm_read_byte(&menuSize);
    uint16_t adr;

    for(uint8_t i = 0; i < end; i++){
        adr = pgm_read_word(&menu[i]);
        sendPGMString((char *) adr);
        sendCRLF();
    }
}

inline void sendCRLF(){
    uart_send(0x0D);
    uart_send(0x0A);
}

inline void sendPGMString(const char * string){
    uint8_t index = 0, c;

    while((c = pgm_read_byte(string + index)) != 0){
        uart_send(c);
        index++;
    }   
}

inline void sendString(char * string){
    uint8_t index = 0;

    while(string[index] != 0){
        uart_send(string[index]);
        index++;
    }   
}

void getCommand(){
    sendPGMString(menuTextInput);
    uint8_t c;

    while((c = uart_receive()) == '\0') ;
    clearScreen();
    sendPGMString(menuTextInput);
    uart_send(c);
    sendCRLF();

    c -= '0';

    if(c > 0 && c < 5){
        switch(c){
            case 1:
                // Start Timer
                break;
            case 2:
                break;
            case 3:
                sendPGMString(menuTextConfigTime);
                uint8_t c, count = 0;
                uint8_t arr[10] = {0};
                do {
                    while((c = uart_receive()) == '\0') ;
                    uart_send(c);
                    arr[count++] = (c - '0');
                } while(c != 13);

                timerArray[0].interval = 0;
                for(int i = 9; i >= 0; i--){
                    timerArray[0].interval += arr[i] * (uint32_t) pow(10, 9 - i);
                };

                snprintf(msg, 30, "Timer was set to %lu", (unsigned long) timerArray[0].interval);
                sendString(msg);
                sendCRLF();
                break;
            case 4:
                // Return starting time
                snprintf(msg, 30, "Timer is set to %lu", (unsigned long) timerArray[0].interval);
                sendString(msg);
                sendCRLF();
                break;
        }
    } 
    while(1);
    sendPGMString(menuTextReturn);

    while((c = uart_receive()) != 13) ;
    clearScreen();
}