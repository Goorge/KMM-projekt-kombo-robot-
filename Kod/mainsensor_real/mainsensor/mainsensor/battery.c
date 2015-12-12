/*
 * battery.c
 *
 * Created: 11/6/2015 4:05:58 PM
 *  Author: Anton
 */ 

#include "battery.h"

void read_battery_voltage()
{

uint8_t battery_in;
uint8_t battery_voltage;

	DDRD |= 0x60;
	
	battery_in = read_adc(4); 
	if(battery_in < 180)
	{
		PORTD = 0x60;
	}
	
 	battery_voltage = battery_in + battery_in;
	
	uint8_t data_to_send [2] = {0x20, battery_voltage}; 
	//skicka helvetet
}