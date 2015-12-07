#ifndef I2C_SLAVE_H
#define I2C_SLAVE_H

#include <stdint.h>

#define true 1
#define false 0

#define bool uint8_t
#define byte uint8_t

void i2c_setup(byte adress_);
void i2c_requestToSend(byte adress, byte data[]);
byte incomingData();
void i2c_send(byte prossesor,byte data);
byte i2c_recive();
void i2c_handel_data(const uint8_t gyro_null);

#endif