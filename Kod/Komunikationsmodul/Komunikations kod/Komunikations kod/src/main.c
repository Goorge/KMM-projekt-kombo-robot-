
#include <asf.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <compat/twi.h>

#include "i2c_slave.c"
#include "bluetooth.h"
#include "LCD.h"


void run(void);
void initialize(void);

byte a;

ISR(TWI_vect){
	a=incomingData();
}

int main (void)
{
	board_init();
	initialize();
	run();
}

void initialize(void){
	//i2c_setup(0x02);
	DDRB = 0xff;
	bluetooth_setup((long)115200);
	
	lcd_setup();
	
	//sei();
	_delay_ms(100);
}

void run(void){
	char test[2][16] = {"test", "Mer Test"};
	//lcd_write_string(test);
	lcd_write_instruction(lcd_Clear);
	_delay_us(80);
	lcd_write_char('A');
	
	
	while(true)	{		
		if(PORTD & (1<<CTS) == 0){ // Vi har tillåtelse att skicka data
			bluetooth_send_char(0x04);
		}
	}
}