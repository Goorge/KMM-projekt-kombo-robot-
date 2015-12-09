#ifndef AVSTAND_H
#define AVSTAND_H

void read_avstand_sensor(uint8_t do_avstand_counter);
uint8_t look_up_mellan (uint8_t value);
uint8_t look_up_long(uint8_t value);
void clear_long();

#define TRUE 1
#define FALSE 0

#endif