

void usart_setup(unsigned int baudrate){
	/* Set baud rate */
	unsigned long baudPreScale = ((((14.7456 * 1000000) / (baudrate * 16UL))) - 1);
	UBRR0H = (unsigned char)(baudPreScale>>8);
	UBRR0L = (unsigned char)baudPreScale;
	/* Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	/* Set frame format: 8data, 1stop bit */
	UCSR0C = (3<<UCSZ00);
	
	_delay_ms(200);
}

void USART_Transmit( byte data )
{	
	PORTD &= (0 << PD3); // RTS ska vara aktivt låg?
	PORTD |= 1 << PD3;
	
	while ( !( UCSR0A & (1<<UDRE0)) );/* Wait for empty transmit buffer */

	UDR0 = data; // Put the data in to the buffer and send it
}

unsigned char USART_Receive()
{
	/* Wait for data to be received */
	while ( !(UCSR0A & (1<<RXC0)) );

	/* Get and return received data from buffer */
	return UDR0;
}
