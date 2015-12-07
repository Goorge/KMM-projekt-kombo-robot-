/*
 * mux.c
 *
 * Created: 11/4/2015 1:47:58 PM
 *  Author: calva694
 */ 

#include "mux.h"

#define F_CPU 14.7456E6
#include <util/delay.h>

void mux_init()
{
	DDRB |= 0x0F;
}

uint8_t mux_sensors(uint8_t mux_select_pin)
{
	uint8_t adc_value;
	PORTB &= 0xF0; 
	PORTB |= mux_select_pin;
	_delay_ms(4);
	adc_value = read_adc(0x00);
	return adc_value;
}