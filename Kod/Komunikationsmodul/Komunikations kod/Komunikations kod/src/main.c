
#include <asf.h> //?????
#include <avr/io.h>
#include <avr/interrupt.h>
#include <compat/twi.h>

//typedef int bool;
#define true 1
#define false 0

typedef unsigned char byte;
#include "i2c_slave.c"

void run();
void initialize();

byte firstdata;
byte* extradata;
int datatoget=0;
bool data_recived=false; //flagga för om all data motagen
ISR(TWI_vect){

	firstdata=incomingData();
	/*
	if(datatoget==0)
	{
		firstdata=incomingData();
		datatoget=(firstdata>>4)&0x0f;
		if(firstdata==0x00)
			data_recived=data_recived;
		else if(datatoget=0)
			data_recived=true;
		else
			data_recived=false;
	}
	else
	{
		extradata[(firstdata>>4)&0x0f-datatoget]=incomingData();
		datatoget--;
		if (datatoget==0)
			data_recived=true;
		else
			data_recived=false;
	}
	*/
	
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
	i2c_setup(0x02);
	sei();
}

void run(){
	
	while(true)	{
		if(firstdata == 0x0f){
			DDRD = 1<<PD7;
			PORTD= 1<<PD7;
		}
	}
}