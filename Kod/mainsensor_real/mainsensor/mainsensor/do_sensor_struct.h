#ifndef DO_SENSOR_STRUCT
#define DO_SENSOR_STRUCT

struct do_sensor_struct 
{
	uint8_t counter;
	uint16_t long_counter;
	uint8_t avstand;
	uint8_t reflex;
	uint8_t battery;
	uint8_t kalib;
	uint8_t gyro_null;
} do_sensor;

#endif
