#include <asf.h>

bool newData = false;
bool RequestingToSend = false;

byte dataFromBT = 0x00;
byte dataToSend = 0x00;

//PortD0 TXD
//PortD1 RXD
//PortD2 CTS
//PortD3 RTS

void usart_setup(unsigned int baudrate);

void bluetooth_setup(unsigned int baudrate){
	DDRD = 0x05 | (0xf0 & DDRD); // Borde sätta den låga delen av D men behålla Den höga som den är, den övre delen behövs vid lcd. 
	
	EIMSK = 1<<INT1;	// Enable INT0
	MCUCR = 1<<ISC11;	//Falling edge | 1<<ISC10;	// Trigger INT0 on rising edge
	
	usart_setup(baudrate);
}

void USART_Transmit( byte data )
{	
	PORTD &= (0 << PD3); // RTS ska vara aktivt låg?
	int i = 0; 
	while(PD2 && i < 400 ){++i}; // Give the bluetooth some time to send ctsde j
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE0)) );

	/* Put data into buffer, sends the data */
	UDR0 = data;
}

void RequestToSend(byte data){
	RequestingToSend = true;
	dataToSend = data;
}

void bluetooth_send(){
	USART_Transmit(dataToSend);
	RequestingToSend = false;
	newData = false;
}

void bluetooth_receive(){
	PORTD &= (0 << PD3);
	dataFromBT = USART_Receive();
	newData = true;
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
	if (RequestingToSend){
		bluetooth_send();
	}
	else{
		bluetooth_receive();
	}
}
