#ifndef I2C_SLAVE
#define I2C_SLAVE

#include <compat/twi.h>
#include "definitions.h"
#include "bluetooth.h"

void i2c_setup(byte adress_);
void i2c_requestToSend(byte adress, byte* data);
byte incomingData(void);
void i2c_send(byte prossesor,byte data);
byte i2c_recive(void);
void i2c_handle_data(void);

#endif