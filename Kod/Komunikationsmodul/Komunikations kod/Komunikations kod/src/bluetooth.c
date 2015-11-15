#include "bluetooth.h"

bool newData = false;
bool RequestingToSend = false;
bool SendingData = false;

byte dataFromBT = 0x04;
byte dataToSendBT = 0x00;

//PortD0 RXD (IN)
//PortD1 TXD (OUT)
//PortD2 CTS (IN)
//PortD3 RTS (OUT)


void usart_setup(unsigned int baudrate){
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);// (1<<RXCIE0)|(1<<TXCIE0)|(1<<RXEN0)|(1<<TXEN0);/* Enable receiver and transmitter */
	UCSR0C = (1<<USBS0)|(3<<UCSZ00);/* Set frame format: 8data, 1stop bit */
	
	/* Set baud rate */
	unsigned long baudPreScale = ((((14.7456 * 1000000) / (baudrate * 16UL))) - 1); // == 7
	
	UBRR0H = (unsigned char)(baudPreScale>>8);
	UBRR0L = (unsigned char)baudPreScale;
	
	_delay_ms(200);
}

void bluetooth_setup(unsigned long baudrate){
	DDRD = (1 << TXD) | (1 << RTS);// Sätter blåtandsdelen av D porten, Sedan behöver LCD köra sin init för att ändra RS/E. Men blåtand init måste ske först. 
	
	EIMSK = 1<<INT0;	// Enable INT0
	//MCUCR = 1<<ISC11;	// Falling edge | 1<<ISC10; Behövs nog inte
	
	usart_setup(baudrate);
}

//Behövs nog inte
//void RequestToSend(byte data){
//	if(RequestingToSend == false && SendingData == false){
//		PORTD &= (0 << CTS); // RTS ska vara aktivt låg?
//		_delay_us(1);		 // RTS
//		PORTD |= (1 << CTS);
///		RequestingToSend = true;
//		dataToSendBT = data;
//	}
//}

void bluetooth_send_char(char data){
		while ( !( UCSR0A & (1<<UDRE0)) );/* Wait for empty transmit buffer */
		UDR0 = data; // Put the data in to the buffer and send it
		_delay_ms(500);
		PORTB = 0xff;
		_delay_ms(500);
		PORTB = 0x00;

}


void ClearToSend(void){
	PORTD &= (0 << CTS); // Clear To Send;
}

// BEHÖVS NOG INTE 
ISR ( INT0_vect ){//interrupt saying usart is clear to send
	//if (RequestingToSend){ //If we have sent a request to send we now get that we are clear to send
	//	bluetooth_send();
	//}
	//else{ // else we they are asking to send us data, thus we need to be prepared to recieve
	//	ClearToSend();
	//}
	bluetooth_send_char('B');
}

byte fetchNewData(void){
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