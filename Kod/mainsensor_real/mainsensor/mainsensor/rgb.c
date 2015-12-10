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
	const uint8_t time_delay = 10;
	
	color_value = read_adc(1); //r�d
	if(color_value > 70)
	{	
		PORTD |= 0x20;
		_delay_ms(time_delay);
		PORTD &= ~(0x60);
		data_to_send[0] = 0x03;
		i2c_requestToSend(0x04, data_to_send);	//skickar r�d med I2C
	}
	
	color_value = read_adc(2); //gr�n
	if(color_value > 60)
	{
		PORTD |= 0x40;
		_delay_ms(time_delay);
		PORTD &= ~(0x60);
		data_to_send[0] = 0x04;
		i2c_requestToSend(0x04, data_to_send);	//skickar gr�n med I2C
	}
	
	color_value = read_adc(3); //bl�
	if(color_value > 110)
	{
		PORTD |= 0x60;
		_delay_ms(time_delay);
		PORTD &= ~(0x60);
		data_to_send[0] = 0x05;
		i2c_requestToSend(0x04, data_to_send);	//skickar bl� med I2C
	}
}