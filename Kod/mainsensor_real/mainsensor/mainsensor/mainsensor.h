#ifndef MAINSENSOR_H_
#define MAINSENSOR_H_

#include "mux.h"
#include "adc.h"
#include "rgb.h"
#include "gyro.h"
#include "kalibrering.h"
#include "reflexsensor.h"
#include "battery.h"
#include "avstand.h"
#include "i2c_slave.h"

#include "mux.c"
#include "adc.c"
#include "i2c_slave.c"
#include "rgb.c"
#include "gyro.c"
#include "kalibrering.c"
#include "reflexsensor.c"
#include "battery.c"
#include "avstand.c"


#define TRUE 1
#define FALSE 0
#define ENABLE_INT0 EIMSK |= (1 << INT0)
#define FALLING_EDGE_TRIGGER EICRA |= (1 << ISC01)
#define SHUT_DOWN_INTERRUPT EIFR |= (1<< INTF0)

int main(void);
void interrupt_init();
void variable_init();

#endif