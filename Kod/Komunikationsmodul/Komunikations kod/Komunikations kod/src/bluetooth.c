#include <asf.h>

bool newData = false;
bool RequestingToSend = false;
bool SendingData = false;

byte dataFromBT = 0x04;
byte dataToSendBT = 0x00;

//PortD0 RXD
//PortD1 TXD
//PortD2 RTS
//PortD3 CTS

void usart_setup(unsigned int baudrate){
	/* Enable receiver and transmitter */
	UCSR0B = (1<<RXCIE0)|(1<<TXCIE0)|(1<<RXEN0)|(1<<TXEN0);
	/* Set frame format: 8data, 1stop bit */
	UCSR0C = (3<<UCSZ00);
	
	/* Set baud rate */
	unsigned long baudPreScale = ((((14.7456 * 1000000) / (baudrate * 16UL))) - 1); // == 7
	
	UBRR0H = (unsigned char)(baudPreScale>>8);
	UBRR0L = (unsigned char)baudPreScale;
	
	_delay_ms(200);
}

void bluetooth_setup(unsigned int baudrate){
	DDRD = 0x06;// Sätter blåtandsdelen av D porten, Sedan behöver LCD köra sin init för att ändra RS/E. Men blåtand init måste ske först. 
	
	EIMSK = 1<<INT1;	// Enable INT1
	//MCUCR = 1<<ISC11;	// Falling edge | 1<<ISC10; Behövs nog inte
	
	usart_setup(baudrate);
}

//Behövs nog inte
void RequestToSend(byte data){
	if(RequestingToSend == false && SendingData == false){
		PORTD &= (0 << PD2); // RTS ska vara aktivt låg?
		_delay_us(1);		 // RTS
		PORTD |= (1 << PD2);
		RequestingToSend = true;
		dataToSendBT = data;
	}
}

void bluetooth_send_char(char data){
	while ( !( UCSR0A & (1<<UDRE0)) );/* Wait for empty transmit buffer */
	UDR0 = data; // Put the data in to the buffer and send it
}


void ClearToSend(){
	PORTD &= (0 << PD3); // Clear To Send;
	//_delay_us(1);
	//PORTD |= (1 << PD3); 
}

// BEHÖVS NOG INTE 
ISR ( INT1_vect ){//interrupt saying usart is clear to send
	//if (RequestingToSend){ //If we have sent a request to send we now get that we are clear to send
	//	bluetooth_send();
	//}
	//else{ // else we they are asking to send us data, thus we need to be prepared to recieve
	//	ClearToSend();
	//}
	bluetooth_send_char('B');
}

byte fetchNewData(){
	newData = false;
	return dataFromBT;
}

//Behövs nog inte sen
ISR ( USART0_TX ) { //send complete
	SendingData = false;
}
	 
//Hämtar datan
ISR ( USART0_RX ){ //recieve complete
	dataFromBT = UDR0;
	newData = true;
}	