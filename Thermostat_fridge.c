/*
 * Termostato_Frigo.c
 *
 * Created: 15-03-2015 10:00:54
 *  Author: RASC
 */ 

#include <avr/io.h>
#include <stdint.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include "lcd.h"
#include "timers.h"

#define HYST 700
#define TEMP_DES 1700 //4ºC
#define COMPRESSOR_ON PORTD |= ( 1 << PD7 );
#define COMPRESSOR_OFF PORTD &= ~( 1 << PD7 );

const uint16_t xyTable[14][2]  PROGMEM = { { 500, 1120 }, { 700, 981 }, { 1000, 845 }, { 1500, 701 }, { 2000, 606 }, { 2500, 536 }, { 3000, 481 },	
                                    { 4000, 398 }, { 5000, 336 }, { 6000, 287}, { 8000, 214 }, { 10000, 159 }, { 14000, 80 }, { 20000, 1 } };

extern volatile uint16_t timer;

uint8_t linearInterp_P( uint16_t x )
{
	uint8_t xIndex = 0;
	
	//Search xIndex --> XL <= x < XH
	while (  pgm_read_word( &xyTable[ xIndex ][ 0 ] ) < x )
	{
		xIndex++;
	}

	//y = ( ( ( X - XL ) * ( YH - YL ) ) / ( XH - XL ) ) + YL
	return ( uint16_t ) ( ( ( ( ( long int ) x - pgm_read_word( &xyTable[ xIndex - 1 ][ 0 ] ) ) * ( ( int ) ( pgm_read_word( &xyTable[ xIndex ][ 1 ] ) - pgm_read_word( &xyTable[ xIndex - 1 ][ 1 ] ) ) ) ) / ( pgm_read_word( &xyTable[ xIndex ][ 0 ] ) - pgm_read_word( &xyTable[ xIndex - 1 ][ 0 ] ) ) ) + pgm_read_word( &xyTable[ xIndex - 1 ][ 1 ] ) );
}

void convert2ascii( uint16_t num, uint8_t *conversion )
{
	uint16_t number = num;
	uint8_t aux = 0;
	
	if( number < 100 )
	{
		aux = (uint8_t) ( number / 10 );
		conversion[ 0 ] = 48 + aux;
		aux = number - ( 10 * aux );
		conversion[ 1 ] = 48 + aux;
	}
	else if( number < 10 )
	{
		conversion[ 0 ] = 48 + number;
	}
	else
	{
		aux = (uint8_t) ( number / 100 );
		conversion[ 0 ] = 48 + aux;
		number = number - ( aux * 100 );
		aux = (uint8_t) ( number / 10 );
		conversion[ 1 ] = 48 + aux;
		number = number - ( aux * 10 );
		conversion[ 2 ] = 48 + number;
	}
}

int main(void)
{
	//uint16_t average = 0;
	//uint8_t n = 0;
	char temperature[6] = { 0, 0, '.', 0, 'C', '\0' };
	char ascii[ 4 ] = { 0, 0 ,0, '\0' };
	
	//PORTA configured as output
	DDRA = 0xF3;
	
	DDRD = 0x80;
	//Pull-Up Resistors
	//PORTD = 0x0C;
	
	COMPRESSOR_OFF;
	
	initTimer1();
	lcdInit();
	
	lcdPrint( "T act:" );
	lcdMoveCursor( 2, 0 );
	lcdPrint( "T prog: 4C");
	
	sei();
	
	enableICInterrupt();
	
    while( 1 )
    {		
		while( timer == 0 );
		
		disableICInterrupt();		 
		//utoa( linearInterp_P( timer * 10 ), ascii, 10 );
		convert2ascii( linearInterp_P( timer * 10 ), ascii );
		lcdMoveCursor( 1, 7);
		_delay_ms( 1 );
			
		if( 47 < ascii[ 2 ] && 58 > ascii[ 2 ]  )
		{
			temperature[ 0 ] = ascii[ 0 ];
			temperature[ 1 ] = ascii[ 1 ];
			temperature[ 3 ] = ascii[ 2 ];
		} 
		else
		{
			temperature[ 0 ] = ' ';
			temperature[ 1 ] = ascii[ 0 ];
			temperature[ 3 ] = ascii[ 1 ];
		}
		
		lcdPrint( temperature );
		_delay_ms( 2000 );
		
		if( timer < 1630 )
		{
			//liga
			COMPRESSOR_ON;
		} 
		else if( timer > 1780 )
		{
			//desliga
			COMPRESSOR_OFF;
		}

		timer = 0;
		ascii[ 0 ] = 0;
		ascii[ 1 ] = 0;
		ascii[ 2 ] = 0;			 
		enableICInterrupt();			
    }
}