
#include <asf.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <compat/twi.h>

#include "i2c_slave.c"
#include "bluetooth.c"

void run();
void initialize();

byte a;

ISR(TWI_vect){
	a=incomingData();
	
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
<<<<<<< HEAD
	while(true)	{bluetooth_send(0x02);
		if(newData){
			
			PORTB = 0xff;
		}
		if(a == 0xf0){
=======
	
	while(true)	{
		if(a == 0xff){
>>>>>>> 185953bdb6adb98e3fc4f4be5fb7c5a4b0bdddb9
			DDRD = 1<<PD7;
			PORTD= 1<<PD7;
		}
	}
}