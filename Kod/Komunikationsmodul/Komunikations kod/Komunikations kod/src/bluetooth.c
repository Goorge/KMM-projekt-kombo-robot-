#include <asf.h>

bool newData = false;
bool RequestingToSend = false;
bool SendingData = false;

byte dataFromBT = 0x00;
byte dataToSendBT = 0x00;

//PortD0 TXD
//PortD1 RXD
//PortD2 CTS
//PortD3 RTS

void bluetooth_setup(unsigned int baudrate){
	DDRD = 0x05 | (0xf0 & DDRD); // Borde sätta den låga delen av D men behålla Den höga som den är, den övre delen behövs vid lcd. 
	
	EIMSK = 1<<INT1;	// Enable INT1
	MCUCR = 1<<ISC11;	// Falling edge | 1<<ISC10;	// Trigger INT0 on rising edge
	
	usart_setup(baudrate);
}

void RequestToSend(byte data){
	if(RequestingToSend == false && SendingData == false){
		RequestingToSend = true;
		dataToSendBT = data;
	}
}

void bluetooth_send(){
	PORTD &= (0 << PD3); // RTS ska vara aktivt låg?
	_delay_ms(1);
	PORTD |= 1 << PD3;
	USART_Transmit(dataToSendBT);
	RequestingToSend = false;
	SendingData = true;
}


void ClearToSend(){
	PORTD &= (0 << PD3); // Clear To Send;
}

ISR ( INT1_vect ){//interrupt for incoming usart
	if (RequestingToSend){ //If we have sent a request to send we now get that we are clear to send
		bluetooth_send();
	}
	else{ // else we they are asking to send us data, thus we need to be prepared to recieve
		ClearToSend();
	}
}

byte fetchNewData(){
	newData = false;
	return dataFromBT;
}

ISR ( USART0_TX ){ //send complete
	SendingData = false;
}	 

ISR ( USART0_RX ){ //send complete
	dataFromBT = UDR0;
	newData = true;
}	