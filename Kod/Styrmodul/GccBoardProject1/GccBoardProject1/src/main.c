#include <avr/io.h>
#include <avr/interrupt.h>
#include <compat/twi.h>

typedef short bool;
#define true 1
#define false 0

typedef unsigned char byte;
#include "i2c_master.c"

//avbrotsrutiner
ISR(INT0_vect)
{
	if (PC6){	//komunikation vill skicka
		
	}
	else if(PC7){ //sensor vill skicka
		
	}
	
}

int main(){
	//init
	i2c_setup(true);

	

	sei();				//Enable Global Interrupt
	while(1){
		//FUNKTIONSLOP



	};
	return 0;


};
