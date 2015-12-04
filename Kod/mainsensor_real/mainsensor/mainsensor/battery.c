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
	//if(battery_cell_1 < 180)
		//PORTD = 0x60;
		//_delay_ms(1000);
		//PORTD = 0x00;
	
	battery_cell_2 = read_adc(7); 
	//if(battery_cell_2 < 180)
		//PORTD = 0x60;
	
	uint8_t data_to_send [3];
	data_to_send[0] = 0x20;
	data_to_send[1] = battery_cell_1;
	data_to_send[2] = battery_cell_2;
		
	i2c_requestToSend(0x04, data_to_send);
	//skicka helvetet
}