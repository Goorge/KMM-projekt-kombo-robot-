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
	choose_adc_pin &= 0b00000111;	//S�kerst�ller v�rdet p� mux-v�ljaren
	ADMUX &= 0xF0;	//Nollst�ller val av MUX-v�ljaren
	ADMUX |= (1 << ADLAR);	//V�nsterjusterar resultatet
	ADMUX |= choose_adc_pin;	//V�ljer pinne p� MUXen
	START_AD_CONVERSION;
	WAIT_FOR_AD_CONVERSION;
	return ADCH; //returnerar dom 8 h�gst signifikanta bitarna av AD-omvandlingen
}

uint16_t read_adc_gyro()
{
	ADMUX &= 0b11010000;	//H�gerjustering av resultatet och nollst�ller MUX-v�ljaren
	ADMUX |= 0x06;	//V�ljer gyro f�r AD-omvandling
	START_AD_CONVERSION;
	WAIT_FOR_AD_CONVERSION;
	return ADC;	//returnerar hela 10-bitarsv�rdet
}
