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

ISR(INT0_vect)	// INT0 sätter kalibrerings variabel
{	
  PORTD |= 0x60;
  _delay_ms(400);
  PORTD &= 0x9F;				
  kalibrate = TRUE;
  SHUT_DOWN_INTERRUPT;	//Stänger av interrupt under pågående interrupt
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

		read_reflex_sensors();	//kör reflexsensorer. Muxar hela längan
		_delay_ms(10);
		read_avstand_sensor(do_avstand_counter);	//läser avståndssensorer
		_delay_ms(10);
		read_rgb();			//läs RGB
		_delay_ms(10);
		i2c_handle_data(gyro_null);	//för att köra gyrot när vi tar emot att vi ska göra något så roligt! Snurr snurr
		
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