#ifndef GYRO_H
#define GYRO_H

void init_timer0();
void read_gyro(const uint8_t wanted_degrees);

#define PRESCALE_1024 TCCR0B |= (1 << CS02)|(1 << CS00)
#define INIT_COUNTER TCNT0 = 0

#endif