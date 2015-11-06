#include <avr/io.h>
#include <avr/interrupt.h>
#include <compat/twi.h>

typedef short bool;
#define true 1
#define false 0

typedef unsigned char byte;

#include "i2c_master.c"
#include "linje.c"

//avbrotsrutiner
ISR(INT0_vect)
{
	if (PC6){	//komunikation vill skicka
		i2c_recive(0x02); // processor 1
	}
	else if(PC7){ //sensor vill skicka
		i2c_recive(0x06); // processor 3
	}
	
}


int main(){
	//init
	i2c_setup(true);
	
	sei();				//Enable Global Interrupt
	
	while(1){
		//FUNKTIONSLOP
		i2c_send(0x02, 0xf0);
		led_test();
	};
	return 0;


};
