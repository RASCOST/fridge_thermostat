/*
 * timers.c
 *
 * Created: 21-03-2015 17:21:36
 *  Author: RASC
 */ 
#include "timers.h"

extern volatile uint16_t timer = 0;

ISR( TIMER1_CAPT_vect )
{
	if ( ! ( TCCR1B & ( 1 << ICES1 ) ) ) //if falling edge
	{
		//Select Rising Edge
		TCCR1B |= ( 1 << ICES1 );
		//Clear Timer1
		TCNT1 = 0;
	}
	else// if ( ( TCCR1B & ( 1 << ICES1 ) ) ) rise edge
	{
		//Select Falling Edge
		TCCR1B &= ~ ( 1 << ICES1 );
		//Read Capture Time
		timer = ICR1;
		//flag = TEMPERATURE;
	}
}

void disableICInterrupt()
{
	//Input Capture Interrupt Disable
	TIMSK &= ~( 1 << TICIE1 );
	//Stop Timer1
	TCCR1B &= ~( 1 << CS10 );
	
}

void enableICInterrupt()
{
	//Input Capture Select Edge -> fall
	TCCR1B &= ~ ( 1 << ICES1 );
	//Input Capture Interrupt Enable
	TIMSK |= ( 1 << TICIE1 );
	//Start Timer1 Without Prescalar
	TCCR1B |= ( 1 << CS10 );
}

void initTimer1()
{
	//Input Capture Select Edge -> fall
	TCCR1B &= ~ ( 1 << ICES1 );
	//Noise Canceler
	TCCR1B |= ( 1 << ICNC1 );
	//Timer1 Normal Mode
	TCCR1A |= ( ( 0 << WGM11) | ( 0 << WGM10 ) );
	TCCR1B |= ( ( 0 << WGM13 ) | ( 0 << WGM12 ) );
}