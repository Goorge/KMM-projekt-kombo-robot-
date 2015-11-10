#include <asf.h>
bool newData = false;
byte dataFromBT = 0x00;
//PortD0 TXD
//PortD1 RXD
//PortD2 CTS
//PortD3 RTS
void usart_setup(unsigned int baudrate);

void bluetooth_setup(unsigned int baudrate){
	DDRD = 0x05 | (0xf0 & DDRD); // Borde sätta den låga delen av D men behålla Den höga som den är, den övre delen behövs vid lcd. 
	
	

	usart_setup(baudrate);
}

void USART_Transmit( byte data )
{	
	PORTD &= (0 << PD3); // RTS ska vara aktivt låg?
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE0)) );

	/* Put data into buffer, sends the data */
	UDR0 = data;
}

void bluetooth_send(data){
	USART_Transmit(data);
	newData = false;
}

void usart_setup(unsigned int baudrate){
	/* Set baud rate */
	UBRR0H = (unsigned char)(baudrate>>8);
	UBRR0L = (unsigned char)baudrate;
	/* Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	/* Set frame format: 8data, 1stop bit */
	UCSR0C = (3<<UCSZ00);
}



unsigned char USART_Receive()
{
	/* Wait for data to be received */
	while ( !(UCSR0A & (1<<RXC0)) );

	/* Get and return received data from buffer */
	return UDR0;
}

ISR ( INT1_vect ){//interrupt for incoming usart
	dataFromBT = USART_Receive();
	newData = true;
}
