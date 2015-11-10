
#include <asf.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <compat/twi.h>

#include "i2c_slave.c"
#include "bluetooth.c"

void run();
void initialize();

byte a;

ISR(TWI){
	a=incomingData();
	DDRD = 1<<PD6;
	PORTD= 1<<PD6;
}

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */

	//board_init();
	initialize();
	run();
	/* Insert application code here, after the board has been initialized. */
}

void initialize(){
	EIMSK = 1<<INT1;					// Enable INT0
	//MCUCR = 1<<ISC01 | 1<<ISC00;	// Trigger INT0 on rising edge
	i2c_setup(0x02);
	bluetooth_setup(115200);
	DDRB = 0xff;
	sei();
}

void run(){
	while(true)	{bluetooth_send(0x02);
		if(newData){
			
			PORTB = 0xff;
		}
		if(a == 0xf0){
			DDRD = 1<<PD7;
			PORTD= 1<<PD7;
		}
	}
}