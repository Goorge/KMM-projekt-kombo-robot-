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

int main(void){	
	DDRD |= 0x60;
	avstand_init();
	mux_init();		
	adc_init();
	i2c_setup(0x06);
	interrupt_init();
	const uint8_t gyro_null = read_adc(6);
	
	/*static uint16_t cntr = 0;
	static uint8_t avstand_counter = 0;*/
	
	while(1)
	{
		if(kalibrate == TRUE)
		{
			kalibrering();
			kalibrate = FALSE;
		}

		read_reflex_sensors();	//kör reflexsensorer. Muxar hela längan
		//read_avstand_sensor(&avstand_counter);	//läser avståndssensorer
		//read_rgb();			//läs RGB
		i2c_handle_data(gyro_null);	//för att köra gyrot när vi tar emot att vi ska göra något så roligt! Snurr snurr
		
		/*if(cntr == 1000)
		{
			//read_battery_voltage();
			cntr = 0;
		}
		else
		{
			++cntr;	
		}*/
    }
}

void interrupt_init()
{
  ENABLE_INT0;
  FALLING_EDGE_TRIGGER;
  sei();
}

/*void variable_init()
{
	do_sensor.counter = 0;
	do_sensor.long_counter = 0;
	do_sensor.avstand = TRUE;
	do_sensor.reflex = TRUE;
	do_sensor.battery = TRUE;
	do_sensor.kalib = FALSE;
	do_sensor.gyro_null = read_adc(6);
}*/