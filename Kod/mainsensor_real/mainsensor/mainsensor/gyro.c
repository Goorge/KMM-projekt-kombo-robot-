/*
 * gyro.c
 *
 * Created: 11/4/2015 5:58:58 PM
 *  Author: calva694
 */ 

#include "gyro.h"
#include <stdlib.h>
	
void read_gyro(const uint8_t wanted_degrees, const uint8_t gyro_null)
{
	/*
	Gyrots känslighet är 300 dgs/s
	0.5 volt till 4.5 volt. 2.5 volt är mitten, dvs. svänger inte
	Det motsvarar värdena 26-230 efter A/D-omvandling, där 128 är mitten
	Gyrot ska kollas 100 ggr/sekund
	Det betyder att det går mellan -3 till 3 grader per koll
	
	230-128=102
	Om vi ser vad det blir per grad så är det alltså 34 enhetsvärden per grad
	Om vi då uppskalar 34 gånger så man en sömlös lösning
	
	Man börjat med att ta antalet grader man vill rotera*34
	Sen tar man inputen efter ADomvanlingen -128 (grader_34=input-128) <------ INPUT-GYRO_NULL
	grader_34_totalt+=grader_34
	
	Och jämför grader_34_totalt med antalet grader man ska rotera med antalet grader man ska rotera*34.
	Är absolutbeloppet på grader totalt större eller lika med rotera*34 så har man roterat 90 grader		
	*/

	const int wanted_degrees_34 = wanted_degrees*34;
	int degrees_rotated_34 = 0;
	DDRD |= 0x60;
	
	while(abs(degrees_rotated_34) <= wanted_degrees_34)
	{	
		PORTD |= 0x20;
		if(TCNT0 >= 143) //timer 100gånger/sekund
		{
			TCNT0 = 0;
			degrees_rotated_34 += read_adc(6)-gyro_null;
		}
	}
	
	uint8_t i;
	
	for(i=0; i<10 ; ++i)
	{
		PORTD |= 0x20;
		_delay_ms(100);
		PORTD &= 0x9F;
		_delay_ms(200);
		PORTD |= 0x40;
		_delay_ms(100);
		PORTD &= 0x9F;
		_delay_ms(100);	
	}
	uint8_t data_to_send[1];
	data_to_send[1] = 0x06;	
	i2c_requestToSend(0x04, data_to_send);
	degrees_rotated_34 = 0;
}

void init_timer0()
{
	PRESCALE_1024;	//Prescale 1024
	INIT_COUNTER;							//Init counter
}