#ifndef MUX_H
#define MUX_H

#include <avr/io.h>
#include <stdint.h>

void mux_init();
uint8_t mux_sensors(uint8_t mux_select_pin);

#endif