/*
 * adc.c
 *
 * Created: 11/4/2015 5:00:58 PM
 *  Author: calva694
 */ 

#include "adc.h"

void adc_init()
{
	ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (0 << ADIE); // ADC Enabled, Prescaler of 64, interrupts NOT enabled
	ADMUX |= (1 << ADLAR);
}

uint8_t read_adc(uint8_t choose_adc_pin)
{
	ADMUX &= 0xF0; 
	ADMUX |= choose_adc_pin;
	START_AD_CONVERSION;
	WAIT_FOR_AD_CONVERSION;
	return ADCH; //returnerar dom 8 högst signifikanta bitarna
}
