#ifndef AVSTAND_H
#define AVSTAND_H

void read_avstand_sensor();
uint8_t look_up_mellan (uint8_t value);
uint8_t look_up_long(uint8_t value);
void long_init();
void clear_long();

#endif