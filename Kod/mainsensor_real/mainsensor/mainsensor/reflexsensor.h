#ifndef REFLEXSENSOR_H
#define REFLEXSENSOR_H

void read_reflex_sensors();
uint8_t look_up_value(const uint8_t sensor_value, const uint8_t i);
void package_and_send_sensor_data(const uint8_t sensor_data, const uint8_t pin);

#define TRUE 1
#define FALSE 0

#endif