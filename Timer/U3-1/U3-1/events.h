/*
 * events.h
 *
 * Created: 18.04.2024 14:29:12
 *  Author: ref41518
 */ 
#ifndef EVENT_H_
#define EVENT_H_

#include <stdint.h>
#include <avr/interrupt.h>
#include <avr/common.h>

typedef uint16_t event_type;

volatile event_type events=0;

void setEvent(event_type ev) {
	unsigned char sreg_old = SREG;
	cli();
	events |= ev;
	SREG = sreg_old;
}

void clearEvent(event_type ev) {
	unsigned char sreg_old = SREG;
	cli();
	events &= ~ev;
	SREG = sreg_old;
}

int eventIsSet(event_type ev){
	return (events&ev);
}

#endif /* EVENT_H_ */