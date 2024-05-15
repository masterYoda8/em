#define F_CPU 16000000UL
#define BAUDRATE 9600

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

typedef enum{
	false,
	true
} bool;

#include "timer.h"
#include "uart.h"

/*
	LEDA1 AN PB0
	LEDA4 AN PB1
	LEDA7 AN PB2
*/

//DEBUG
//PORTD &= ~(1 << DEBUG_LED_VAL);
//_delay_ms(100);
// PORTD |= (1 << DEBUG_LED_VAL);
// DEBUG END

#define LED0_MODE DDB0
#define LED0_VAL PORTB0
#define LED1_MODE DDB1
#define LED1_VAL PORTB1
#define LED2_MODE DDB2
#define LED2_VAL PORTB2
#define DEBUG_LED_MODE DDD0
#define DEBUG_LED_VAL PORTD0

void setLEDS();
void countdown();
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
const char menuTextConfigStartVal[] PROGMEM = "Input time in ms: ";

const char* const menu[] PROGMEM = {menuTextHeader, menuTextDivider, menuTextAction, \
    menuTextAction1, menuTextAction2, menuTextAction3, menuTextAction4};
const uint8_t menuSize PROGMEM = 7;

char msg[30] = {0};

// If the counter is running or not
volatile uint8_t running = 1;
// The current count
volatile uint8_t count;
// The starting count (reset value)
volatile uint8_t startingCount = 7;

ISR(PCINT2_vect){
	count = startingCount;
	setLEDS();
}

int main(void){
	// Debug LED
	DDRD |= (1 << DEBUG_LED_MODE);
	PORTD |= (1 << DEBUG_LED_VAL); 
	
	
	// Activate Output and Input Pullups and init
	DDRB |= ((1 << LED0_MODE) | (1 << LED1_MODE) | (1 << LED2_MODE));
	DDRD |= (1 << PORTD1);
	PORTB &= ~((1 << LED0_VAL) | (1 << LED1_VAL) | (1 << LED2_VAL));

    timer_init();
    uart_init();
	// Declare 1s timer for counting down
	declareTimer(TIMER1, 1000000, countdown);

	// Enable global interrupt
	sei();

	count = 8;
	
	startTimer(TIMER1);

	while(1){
      printMenu();
      getCommand();
	}
}

void countdown(){
	if(running) {
		if(count) {
			count--;
			setLEDS();
		} else {
			// Trigger interrupt
			PORTD ^= (1 << PORTD1);
		}
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
                startTimer(TIMER1);
                break;
            case 2:
                // Stop Timer
                cancelTimer(TIMER1);
                break;
            case 3:
                // Configure starting time
                sendPGMString(menuTextConfigStartVal);
                uint8_t c;
                do{
                  while((c = uart_receive()) == '\0') ;
                    uart_send(c);
                } while(!isValidStartValue(c - '0'));
                // Set new count value
                startingCount = c - '0';
                snprintf(msg, 30, "Start value was set to %d", (int) startingCount);
                sendString(msg);
                sendCRLF();
                break;
            case 4:
                // Return starting time
                snprintf(msg, 30, "Start value is %d", (int) startingCount);
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

bool isValidStartValue(uint8_t c){
    if(c < 0 || c > 9){
        sendPGMString("Invalid input, please enter a number between 0 and 9");
        sendCRLF();
        return false;
    }
    return true;
}
