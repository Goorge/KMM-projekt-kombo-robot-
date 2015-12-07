/*
 * reflexsensor.c
 *
 * Created: 11/5/2015 2:39:58 PM
 *  Author: calva694
 */ 

#include "reflexsensor.h"
#include "kal_struct.h"

#define F_CPU 14.7456E6
#include <util/delay.h>
#include <stdint-gcc.h>

void read_reflex_sensors()
{
	uint8_t i;
	uint8_t looked_up_value;
	uint8_t sensor_value;
	
	for(i=0; i<11; ++i)
	{
		sensor_value = mux_sensors(i);
		looked_up_value = look_up_value(sensor_value, i);
		package_and_send_sensor_data(looked_up_value, i);
	}
}

uint8_t look_up_value(const uint8_t sensor_value, const uint8_t i)
{
	if(sensor_value < reflex_kal[i].low)	//00
	{
		return 0x00; //00
	}
	else if (sensor_value < reflex_kal[i].middle)	//01
	{	
		return 0x01; //01
	}
	else if (sensor_value < reflex_kal[i].high)	//10
	{
		return 0x02; //10
	}
	else	//11
	{
		return 0x03;//11
	}
}

void package_and_send_sensor_data(const uint8_t sensor_data, const uint8_t pin)
{
	uint8_t byte_1;
	uint8_t byte_2;
	uint8_t byte_3;
	
	if(pin == 0)
	{
		byte_1 |= sensor_data;
	}
	else if(pin == 1)
	{
		byte_1 |= (sensor_data << 2);
	}
	else if(pin == 2)
	{
		byte_1 |= (sensor_data << 4);
	}
	else if(pin == 3)
	{
		byte_1 |= (sensor_data << 6);
	}
	else if(pin == 4)
	{
		byte_2 |= sensor_data;
	}
	else if(pin == 5)
	{
		byte_2 |= (sensor_data << 2);
	}
	else if(pin == 6)
	{
		byte_2 |= (sensor_data << 4);
	}
	else if(pin == 7)
	{
		byte_2 |= (sensor_data << 6);
	}
	else if(pin == 8)
	{
		byte_3 |= sensor_data;
	}
	else if(pin == 9)
	{
		byte_3 |= (sensor_data << 2);
	}
	else if(pin == 10)
	{
		byte_3 |= (sensor_data << 4);
		
		uint8_t data_to_send [4];
		data_to_send[0] = 0x32;
		data_to_send[1] = byte_1;
		data_to_send[2] = byte_2;
		data_to_send[3] = byte_3;
		
		i2c_requestToSend(0x04, data_to_send);
	}
}
