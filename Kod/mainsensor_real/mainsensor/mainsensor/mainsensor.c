/*
 * Sensor_kmm.c
 *
 * Created: 11/4/2015 1:38:58 PM
 *  Author: calva694
 */ 

#include "mainsensor.h"
#include "kal_struct.h"
#include "do_sensor_struct.h"

#include <util/delay.h>
#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>

//uint8_t gyro_null;

ISR(INT0_vect)	// INT0 sätter kalibrerings variabel
{	
  PORTD |= 0x60;
  _delay_ms(400);
  PORTD &= 0x9F;				
  do_sensor.kalib = TRUE;
  SHUT_DOWN_INTERRUPT;	//Stänger av interrupt under pågående interrupt
}

ISR(BADISR_vect){
  //PORTD |= 0x20;
  incomingData();
  TWCR = (1 << TWIE) | (1 << TWEN)| (1<<TWEA) | (1<<TWINT);//TWCR |= (1<<TWEA) | (1<<TWINT);
}

int main(void){	
	DDRD |= 0x60;
	avstand_init();
	mux_init();		
	adc_init();
	init_timer0();
	i2c_setup(0x06);
	interrupt_init();
	variable_init();
	
	while(1)
	{
		if(do_sensor.kalib == TRUE)
		{
			kalibrering();
			do_sensor.kalib = FALSE;
		}
		
		if(do_sensor.counter == 100)
		{
			do_sensor.avstand = TRUE;
			do_sensor.reflex = TRUE;
			do_sensor.counter = 0;
		}
		else
		{
			++do_sensor.counter;
		}
		
		if(do_sensor.reflex == TRUE)
		{
			read_reflex_sensors();	
		}
		
		if(do_sensor.avstand == TRUE)
		{
			read_avstand_sensor();
		}
		
		//PORTD &= ~(0x60);  
		read_rgb();
		i2c_handle_data();	//för att köra gyrot när vi tar emot att vi ska göra något så roligt! Snurr snurr
		
		/*if(do_sensor.long_counter == 1000)
		{
			read_battery_voltage();
			do_sensor.long_counter = 0;
		}
		else
		{
			++do_sensor.long_counter;	
		}*/
    }
}

void interrupt_init()
{
  ENABLE_INT0;
  FALLING_EDGE_TRIGGER;
  sei();
}

void variable_init()
{
	do_sensor.counter = 0;
	do_sensor.long_counter = 0;
	do_sensor.avstand = TRUE;
	do_sensor.reflex = TRUE;
	do_sensor.battery = TRUE;
	do_sensor.kalib = FALSE;
	do_sensor.gyro_null = read_adc(6);
}