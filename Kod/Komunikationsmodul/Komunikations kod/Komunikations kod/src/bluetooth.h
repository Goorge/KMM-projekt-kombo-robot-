#ifndef bt
#define bt

#include "asf.h"

void usart_setup(unsigned int baudrate);
void bluetooth_setup(unsigned long baudrate);
void bluetooth_send_char(char data);
void ClearToSend(void);
byte fetchNewData(void);

#endif