#include <avr/io.h>
#include <avr/interrupt.h>
#include <compat/twi.h>
#include <asf.h>
#define F_CPU 147456000UL
#include <util/delay.h>

typedef unsigned char byte;

#include "linje.c"
#include "SPI_master.c"
#include "i2c_master.c"


int main(){
	//init
	board_init();
	//SPI_MasterInit();
	i2c_setup(1);
	sei();	
	_delay_ms(1000);
				//Enable Global Interrupt
	byte data[2]={0x12,0x0f};
	while(1){
		//FUNKTIONSLOP
		//i2c_send(0x02,data);
	};
	return 0;


};
