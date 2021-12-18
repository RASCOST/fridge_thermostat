/*
 * lcd.h
 *
 * Created: 15-03-2015 10:04:02
 *  Author: RASC
 */ 


#ifndef LCD_H_
#define LCD_H_

#include <util/delay.h>
#include <avr/io.h>

void lcdCmd( uint8_t cmd );
void lcdData( uint8_t data );
void lcdPrint( char *msg );
void lcdMoveCursor( uint8_t line, uint8_t row );
void lcdCursorLeftShift();
void lcdClear();
void lcdCursorBlink();
void lcdClearCursor();
void lcdInit( void );

#endif /* LCD_H_ */