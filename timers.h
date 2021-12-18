/*
 * timers.h
 *
 * Created: 21-03-2015 17:21:10
 *  Author: RASC
 */ 


#ifndef TIMERS_H_
#define TIMERS_H_

#include <avr/interrupt.h>

volatile uint16_t timer;

ISR( TIMER1_CAPT_vect );
void disableICInterrupt();
void enableICInterrupt();
void initTimer1();

#endif /* TIMERS_H_ */