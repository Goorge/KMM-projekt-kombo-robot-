#include <avr/io.h>
#include <avr/interrupt.h>
#include <compat/twi.h>
#include <asf.h>
#include <util/delay.h>

typedef unsigned char byte;

#include "linje.c"
#include "SPI_master.c"
#include "i2c_master.c"

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
	board_init();
	//SPI_MasterInit();
	i2c_setup(1);
	_delay_ms(1000);
	sei();				//Enable Global Interrupt
	byte data[2]={0x12,0x0f};
	while(1){
		//FUNKTIONSLOP
		_delay_ms(1000);
	};
	return 0;


};
