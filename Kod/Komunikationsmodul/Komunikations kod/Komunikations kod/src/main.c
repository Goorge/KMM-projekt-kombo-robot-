#include <asf.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "definitions.h"
#include "i2c_slave.h"
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
	_delay_ms(2000);
}
char data[15];

void run(void){	
	data[0]=0x2a;
	data[1]=0xa0;
	data[2]=0x8f;
	bluetooth_send_byte(0x0f);
	while(true)	{
		i2c_requestToSend(0x04,data);
		bluetooth_handle_data();
		i2c_handle_data();
		_delay_ms(300);
	}
}
