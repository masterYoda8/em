/*
 * Usart_Test.c
 *
 * Created: 07.03.2020 23:16:48
 * Author : mea39511
 */ 

#define F_CPU 16000000UL

#define BAUDRATE 9600
#define BAUD_CONST (((F_CPU/(BAUDRATE*16UL)))-1)

#include <avr/io.h>
#include <util/delay.h>


void USART_Init(){
	UBRR0H = (BAUD_CONST >> 8);
	UBRR0L = BAUD_CONST;
	UCSR0B |= (1<<RXEN0)|(1<<TXEN0);
}


void USART_Transmit(unsigned char data){
	while(!(UCSR0A & (1<<UDRE0))) ;
	UDR0 = data;
}


unsigned char USART_Receive(){
	while(!(UCSR0A & (1<<RXC0))) ;
	return UDR0;
}

int main(void)
{
	char name[10];
	int g = 0;
	const char meldung[]="Hier ATmega. Wer da?";
	USART_Init();
	for(int f=0;meldung[f]!='\0';f++)
		USART_Transmit(meldung[f]);
    while (1) 
    {
		name[g] = USART_Receive();
		if (name[g]!=0x0d){
			USART_Transmit(name[g]);
			g++;
			continue;	
		}
		USART_Transmit(0x0d);
		USART_Transmit('H');
		USART_Transmit('i');
		USART_Transmit(' ');
		for(int f=0;f<g;f++)
			USART_Transmit(name[f]-32);
		g = 0;
		USART_Transmit(0x0d);
    }
}


