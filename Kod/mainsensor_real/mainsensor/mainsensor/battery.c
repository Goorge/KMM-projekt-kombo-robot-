/*
 * battery.c
 *
 * Created: 11/6/2015 4:05:58 PM
 *  Author: Anton
 */ 

#include "battery.h"

void read_battery_voltage()
{

uint8_t battery_cell_1;
uint8_t battery_cell_2;

	DDRD |= 0x60;
	
	battery_cell_1 = read_adc(4); 
	if(battery_cell_1 < 170)
	{
		PORTD = 0x60;
	}
	
	battery_cell_2 = read_adc(5);
 	battery_cell_2 = battery_cell_2 + battery_cell_2 - battery_cell_1;
	if(battery_cell_2 < 170)
	{
		PORTD = 0x60;
	}
	
	uint8_t data_to_send [3] = {0x20, battery_cell_1, battery_cell_2}; 
	//skicka helvetet
}