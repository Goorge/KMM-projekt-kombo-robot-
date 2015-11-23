#include "definitions.h"
#include "bluetooth.h"

bool newData = false;

byte dataFromBT = 0x00;
byte dataToSendBT = 0x00;

short number_of_bytes_from_bt = 0; //How many bytes is there during current read
//short number_of_bytes_from_i2c = 0;
	
short bytes_left_from_bt = -1;// how many bytes is left to read=
//short bytes_left_from_i2c = 0;

byte data_from_bt[16];// the byte we are currently reading
//byte data_from_i2c[16];
	
short number_of_bytes_to_bt = 0;//How many bytes is there during current read
//short number_of_bytes_to_i2c = 0;
	
short bytes_left_to_bt = -1;// how many bytes is left to read=
//short bytes_left_to_i2c = 0;
		
byte data_to_bt[16];// the byte we are currently reading
//byte data_to_i2c[16];

bool dataSend = false;	
	
//PortD0 RXD (IN)
//PortD1 TXD (OUT)
//PortD2 CTS (IN)
//PortD3 RTS (OUT)

void usart_setup(unsigned int baudrate){
	UCSR0B = (1<<RXCIE0)|(1<<RXEN0)|(1<<TXEN0);/* Enable receiver and transmitter */
	UCSR0C = (3<<UCSZ00);/* Set frame format: 8data, 1stop bit */
	
	/* Set baud rate */
	unsigned long baudPreScale = 7; //((((14.7456 * 1000000) / (baudrate * 16UL))) - 1); // == 7
	
	UBRR0H = (unsigned char)(baudPreScale>>8);
	UBRR0L = (unsigned char)baudPreScale;
	
	_delay_ms(200);
}

void bluetooth_setup(unsigned long baudrate){
	DDRD |= (1 << TXD) | (1 << RTS);// Sätter blåtandsdelen av D porten, Sedan behöver LCD köra sin init för att ändra RS/E. Men blåtand init måste ske först. 
	
	//EIMSK = 1<<INT0;	// Enable INT0
	//MCUCR = 1<<ISC11;	// Falling edge | 1<<ISC10; Behövs nog inte
	
	usart_setup(baudrate);
}

// Skicka en byte över BT
void bluetooth_send_byte(byte data){
	while ( !( UCSR0A & (1<<UDRE0)) );/* Wait for empty transmit buffer */
	UDR0 = data; // Put the data in to the buffer and send it
}

// Säg till blåtandsenheten att du är redo att ta emot mer data. 
void bluetooth_clear_to_send(void){	PORTD &= ~(1 << RTS); }// Clear To Send;

//Hämta ut datan från BT
byte bluetooth_fetch_new_data(void){
	newData = false;
	return dataFromBT;
}

// Kolla om vi har fått in någon ny data
bool bluetooth_get_new_data(void){ return newData; }

// Behandla bluetooth
void bluetooth_handle_data( void ){		
	// Hantera eventuell ny data ifrån BT
	if(bluetooth_get_new_data() == true){ // Om vi har fått data sätts newData till true i BT och vi kollar på den här
		byte data = bluetooth_fetch_new_data(); // Hämta ut data å sätt newData false
			
		// Om vi inte har någon kö för stunden så kör
		if (bytes_left_from_bt == -1){ 
			number_of_bytes_from_bt = (data >> 4); 
			bytes_left_from_bt = number_of_bytes_from_bt;
		}
			
		// Lägg in datan i vektorn
		data_from_bt[number_of_bytes_from_bt - bytes_left_from_bt] = data;
		--bytes_left_from_bt;
			
		// Vill vi ha mer data från samma sändning? 				
		if(bytes_left_from_bt != -1)// Om rts är 0 lagara blåtandseneheten data å skickar via usart när usarten är redo
			bluetooth_clear_to_send(); // Säg att du vill ha mer BT data
		else dataSend = true;	
	} 

	if( dataSend ){ //bytes_left_from_bt == -1 && bytes_left_to_bt == -1){
		for(int i = 0; i < NELEMS(data_from_bt); i++)
			data_to_bt[i] = data_from_bt[i];
		dataSend = false;
		number_of_bytes_to_bt = number_of_bytes_from_bt;
		bytes_left_to_bt = number_of_bytes_from_bt;
		bluetooth_clear_to_send();
		// Då det bara kommer skickas styrdata via bt är det dags att skicka data till styrmodulen
		// i2csendstuff.
	}
	
	// Skicka data via BT
	if(((PIND & (1<<CTS)) == 0) && bytes_left_to_bt != -1){ // Vi har tillåtelse att skicka data & data att skicka
		bluetooth_send_byte(data_to_bt[number_of_bytes_to_bt - bytes_left_to_bt]);
		--bytes_left_to_bt;
	}
}

//Hämtar datan
ISR ( USART0_RX_vect ){ //recieve complete // USART0_RX_vect
	PORTD |= (1 << RTS);//Säg att du inte vill ha mer data atm
	dataFromBT = UDR0;		// Hämta ut datan
	newData = true; // Tala om för main att vi har fått data
}	