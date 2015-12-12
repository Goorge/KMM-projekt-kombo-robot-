/*
 * adc.c
 *
 * Created: 11/4/2015 5:00:58 PM
 *  Author: calva694
 */ 

#include "adc.h"

void adc_init()
{
	ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0) | (0 << ADIE); // ADC Enabled, Prescaler of 128, interrupts NOT enabled
	ADMUX |= (1 << REFS0); //AREF=AVcc
}

uint8_t read_adc(uint8_t choose_adc_pin)
{
	choose_adc_pin &= 0b00000111;	//Säkerställer värdet på mux-väljaren
	ADMUX &= 0xF0;	//Nollställer val av MUX-väljaren
	ADMUX |= (1 << ADLAR);	//Vänsterjusterar resultatet
	ADMUX |= choose_adc_pin;	//Väljer pinne på MUXen
	START_AD_CONVERSION;
	WAIT_FOR_AD_CONVERSION;
	return ADCH; //returnerar dom 8 högst signifikanta bitarna av AD-omvandlingen
}

uint16_t read_adc_gyro()
{
	ADMUX &= 0b11010000;	//Högerjustering av resultatet och nollställer MUX-väljaren
	ADMUX |= 0x06;	//Väljer gyro för AD-omvandling
	START_AD_CONVERSION;
	WAIT_FOR_AD_CONVERSION;
	return ADC;	//returnerar hela 10-bitarsvärdet
}
