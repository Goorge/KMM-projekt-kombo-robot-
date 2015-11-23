#include <asf.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <compat/twi.h>

#include "definitions.h"
//#include "i2c_slave.c"
#include "bluetooth.h"
#include "LCD.h"

void run(void);
void initialize(void);

int main (void){
	board_init();
	initialize();
	run();
}

void initialize(void){
	bluetooth_setup((long)115200);
	lcd_setup();
	
	sei(); // Enable interrupt
	//_delay_ms(100);
}

void run(void){
	while(true)	{
		bluetooth_handle_data();
	}
}
