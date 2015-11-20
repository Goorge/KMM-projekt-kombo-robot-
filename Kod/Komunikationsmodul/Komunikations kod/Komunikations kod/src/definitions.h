/*
 * definitions.h
 *
 * Created: 2015-11-16 12:49:21
 *  Author: Pontus
 */ 

#ifndef definitions
#define definitions

//typedef int bool;
#define true 1
#define false 0

typedef unsigned char byte;

// LCD module information
#define lcd_LineOne     0x00                    // start of line 1
#define lcd_LineTwo     0x40                    // start of line 2

// LCD instructions
#define lcd_Clear           0b00000001          // replace all characters with ASCII 'space'
#define lcd_Home            0b00000010          // return cursor to first position on first line
#define lcd_EntryMode       0b00000110          // shift cursor from left to right on read/write
#define lcd_DisplayOff      0b00001000          // turn display off
#define lcd_DisplayOn       0b00001100          // display on, cursor off, don't blink character
#define lcd_FunctionReset   0b00110000			// reset the LCD
#define lcd_FunctionSet		0b00111000          
#define lcd_SetCursor       0b10000000          // set cursor position DDRAM

// BLUETOOTh pinnar
#define RXD PD0
#define TXD PD1
#define CTS PD2
#define RTS PD3

// global bt data
// extern byte dataFromBT;
// extern bool newData;

#define F_CPU 14745600UL
#include <util/delay.h>

#endif