
#include <asf.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <compat/twi.h>

#include "i2c_slave.c"
#include "usart.c"
#include "bluetooth.c"

void run();
void initialize();

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

void initialize(){
	i2c_setup(0x02);
	bluetooth_setup(115200);
	DDRB = 0xff;
	sei();
}

void run(){
	while(true == true)	{
		RequestToSend(0x02);
		
		if(a == 0xf0){
			DDRD = 1<<PD7;
			PORTD= 1<<PD7;
		}
	}
}