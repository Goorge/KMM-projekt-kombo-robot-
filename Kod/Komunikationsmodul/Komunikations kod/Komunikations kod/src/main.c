#include <asf.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <compat/twi.h>

#include "definitions.h"
#include "i2c_slave.c"
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
	i2c_setup(0x02);
	sei(); // Enable interrupt
	_delay_ms(1000);
}

void run(void){
	while(true)	{
		bluetooth_handle_data();
		_delay_ms(1000);
		byte data[1] = {0x0f};
		requestToSend(0x04, data);
	}
}
