/*
 * Sensor_kmm.c
 *
 * Created: 11/4/2015 1:38:58 PM
 *  Author: calva694
 */ 



#include "mainsensor.h"
#include "kal_struct.h"

#include <util/delay.h>
#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>

uint8_t do_kalibrering = TRUE;
//uint8_t gyro_null;

ISR(INT0_vect)	// INT0 sätter kalibrerings variabel
{	
  PORTD |= 0x60;
  _delay_ms(400);
  PORTD &= 0x9F;				
  do_kalibrering = TRUE;
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

  uint16_t cntr = 0;
  uint8_t data_to_send[1];
  data_to_send[0] = 0x0A;
	int counter = 0;
  while(1)
    {
      if(do_kalibrering == TRUE)
	{
	  kalibrering();
	  do_kalibrering = FALSE;
	}
	
	if(counter++ == 10){
		//_delay_ms(1);
		//read_rgb();
		//_delay_ms(1);
		counter = 0;
		reflex = true;
		avstand = true;
	}
	if(reflex)
		read_reflex_sensors();	
	if(avstand) 
		read_avstand_sensor();
      //_delay_ms(40);
      //i2c_requestToSend(0x04, data_to_send);
      //read_reflex_sensors();
      //read_avstand_sensor();     
      //read_rgb();
		
		/*if(cntr++ >= 1000)
		{
			read_battery_voltage();
			cntr = 0;
		}*/
	}
}

void interrupt_init()
{
  ENABLE_INT0;
  FALLING_EDGE_TRIGGER;
  sei();
}
