#ifndef bt
#define bt

#include "asf.h"

bool bluetooth_get_new_data(void);
void usart_setup(unsigned int baudrate);
void bluetooth_setup(unsigned long baudrate);
void bluetooth_send_byte(byte data);
void bluetooth_clear_to_send(void);
byte* bluetooth_fetch_new_data(void);
void bluetooth_handle_data(void);
bool bluetooth_add_to_send_queue(byte* data);

#endif