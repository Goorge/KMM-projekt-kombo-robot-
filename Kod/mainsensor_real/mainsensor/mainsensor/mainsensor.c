/*
 * Sensor_kmm.c
 *
 * Created: 11/4/2015 1:38:58 PM
 *  Author: calva694
 */ 

#include "mainsensor.h"
#include "kal_struct.h"
//#include "do_sensor_struct.h"

#include <util/delay.h>
#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>

static uint8_t kalibrate = FALSE;

ISR(INT0_vect)	// INT0 s�tter kalibrerings variabel
{	
  PORTD |= 0x60;
  _delay_ms(400);
  PORTD &= 0x9F;				
  kalibrate = TRUE;
  SHUT_DOWN_INTERRUPT;	//St�nger av interrupt under p�g�ende interrupt
}

ISR(BADISR_vect){
  //PORTD |= 0x20;
  incomingData();
  TWCR = (1 << TWIE) | (1 << TWEN)| (1<<TWEA) | (1<<TWINT);//TWCR |= (1<<TWEA) | (1<<TWINT);
}

int main(void)
{	
	DDRD |= 0x60;
	adc_init();
	uint16_t gyro_null = read_adc_gyro();
	static uint8_t do_avstand_counter = 0;
	//static uint16_t battery_counter = 0;	
	mux_init();		
	i2c_setup(0x06);
	interrupt_init();
	
	while(1)
	{
		if(kalibrate == TRUE)
		{
			gyro_null = read_adc_gyro();
			kalibrering();
			kalibrate = FALSE;
		}

		read_reflex_sensors();	//k�r reflexsensorer. Muxar hela l�ngan
		_delay_ms(10);
		read_avstand_sensor(do_avstand_counter);	//l�ser avst�ndssensorer
		_delay_ms(10);
		read_rgb();			//l�s RGB
		_delay_ms(10);
		i2c_handle_data(gyro_null);	//f�r att k�ra gyrot n�r vi tar emot att vi ska g�ra n�got s� roligt! Snurr snurr
		
		/*if(battery_counter == 1000)
		{
			read_battery_voltage();
			battery_counter = 0;
		}
		else
		{
			++battery_counter;
		}*/
		
		if(do_avstand_counter == 5)
		{
			do_avstand_counter = 0;	
		}
		else
		{
			++do_avstand_counter;
		}
    }
}

void interrupt_init()
{
  ENABLE_INT0;
  FALLING_EDGE_TRIGGER;
  sei();
}