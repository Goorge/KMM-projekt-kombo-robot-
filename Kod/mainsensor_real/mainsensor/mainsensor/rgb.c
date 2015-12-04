/*
 * rgb.c
 *
 * Created: 11/5/2015 12:31:58 PM
 *  Author: calva694
 */ 

#include "rgb.h"

//void rgb_init()
//{
	//DDRA |= 0xD0;
//}

void read_rgb()
{
	uint8_t color_value;
	uint8_t data_to_send[1];
	
	DDRD |= 0x60;
	
	color_value = read_adc(1); //röd
	if(color_value > 40)
	{	
		data_to_send[1] = 0x03;
		i2c_requestToSend(0x04, data_to_send);	//skickar röd med I2C
	}
	
	color_value = read_adc(2); //grön
	if(color_value > 20)
	{
		data_to_send[1] = 0x04;
		i2c_requestToSend(0x04, data_to_send);	//skickar grön med I2C
	}
	
	color_value = read_adc(3); //blå
	if(color_value > 40)
	{
		data_to_send[1] = 0x04;
		i2c_requestToSend(0x04, data_to_send);	//skickar blå med I2C
	}
}