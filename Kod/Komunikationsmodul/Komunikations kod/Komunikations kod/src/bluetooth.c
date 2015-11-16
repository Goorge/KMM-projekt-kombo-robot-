#include "definitions.h"
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

bool bluetooth_get_new_data(void){ return newData; }

void usart_setup(unsigned int baudrate){
	UCSR0B = (1<<RXCIE0)|(1<<TXCIE0)|(1<<RXEN0)|(1<<TXEN0);/* Enable receiver and transmitter */
	UCSR0C = (1<<USBS0)|(3<<UCSZ00);/* Set frame format: 8data, 1stop bit */
	
	/* Set baud rate */
	unsigned long baudPreScale = ((((14.7456 * 1000000) / (baudrate * 16UL))) - 1); // == 7
	
	UBRR0H = (unsigned char)(baudPreScale>>8);
	UBRR0L = (unsigned char)baudPreScale;
	
	_delay_ms(200);
}

void bluetooth_setup(unsigned long baudrate){
	DDRD = (1 << TXD) | (1 << RTS);// S�tter bl�tandsdelen av D porten, Sedan beh�ver LCD k�ra sin init f�r att �ndra RS/E. Men bl�tand init m�ste ske f�rst. 
	
	//EIMSK = 1<<INT0;	// Enable INT0
	//MCUCR = 1<<ISC11;	// Falling edge | 1<<ISC10; Beh�vs nog inte
	
	usart_setup(baudrate);
}

// Skicka en byte �ver BT
void bluetooth_send_byte(byte data){
	while ( !( UCSR0A & (1<<UDRE0)) );/* Wait for empty transmit buffer */
	UDR0 = data; // Put the data in to the buffer and send it
}

// S�g till bl�tandsenheten att du �r redo att ta emot mer data. 
void bluetooth_clear_to_send(void){
	PORTD &= (0 << RTS); // Clear To Send;
}

//H�mta ut datan fr�n BT
byte bluetooth_fetch_new_data(void){
	newData = false;
	return dataFromBT;
}

//H�mtar datan
ISR ( USART0_RX_vect){ //recieve complete // USART0_RX_vect
	PORTB = 0xff;
	newData = true;
	dataFromBT = UDR0;		
}	