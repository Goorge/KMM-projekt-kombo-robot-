#include <asf.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "definitions.h"
#include "i2c_slave.h"
#include "bluetooth.h"
#include "LCD.h"

#define F_CPU 14745600UL

void run(void);
void initialize(void);
void doLCD(char* s1, char* s2);

int main (void){
	board_init();
	initialize();
	run();
}

void initialize(void){
	bluetooth_setup((long)115200);
	lcd_init(LCD_DISP_ON); /* initialize lcd, display on, cursor on */
	i2c_setup(0x02);
	sei(); // Enable interrupt
	_delay_ms(2000);
}
char data[15];

void run(void){	
	doLCD("ProcessorKlubben" , "Kombo-Monstret");
	while(true)	{
		//i2c_requestToSend(0x04,data);
		bluetooth_handle_data();
		i2c_handle_data();
	}
}

void doLCD(char* s1, char* s2){
	lcd_clrscr();             /* clear screen of lcd */
	lcd_gotoxy(0,0);              /* bring cursor to 0,0 */
	lcd_puts(s1);        /* type something random */
	lcd_gotoxy(0,1);          /* go to 0nd col, 1nd row (or 1, 2 depending on how you count)*/
	lcd_puts(s2);  /* type something random */
}