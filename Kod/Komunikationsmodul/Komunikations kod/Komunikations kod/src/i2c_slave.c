#include "i2c_slave.h"

byte dataToSend[15]; // Data waiting to be sent is put here
byte reciverAdress;  // The adress of the processor you want to send to is put here
int bytes_to_send_i2c = 0; // How many bytes are we going to send?
int bytesSent = 0; //How many bytes have we already sent? 
int bytefrom_i2c=0; //How many bytes are we going to receive? 
byte i2c_data[15]; // Here incoming data will e stored before we handle it.
bool i2c_new_data = false; // A variable that tells us if we have new data. 

//Avbrott som får om våran adress kommer från i2c.
ISR(TWI_vect){ 
	incomingData();	
	TWCR = (1 << TWIE) | (1 << TWEN)| (1<<TWEA) | (1<<TWINT);// ettsätt acknowledge flaggan (och lite annat) 
}

// Initierar I2C. Adressen som skickas in här är vilken adress som kommer användas för att anropa denna processorn
void i2c_setup(byte adress_) {
	TWAR = adress_; // sätter adressen
	TWCR = (1 << TWIE) | (1 << TWEN)| (1<<TWEA) | (1<<TWINT);
	DDRC |= (1 << PC6);
};

void i2c_requestToSend(byte adress, byte data[]){
	bytes_to_send_i2c = (data[0] >> 4) & 0x0f;
	for(int i = 0; i < bytes_to_send_i2c+1; i++)
		dataToSend[i] = data[i];
	reciverAdress = adress;
	bytesSent = 0;
	PORTC |= (1 << PC6); // Skicka avbrott till mastern om att du vill skicka data
	_delay_us(11);
	PORTC &= ~(1 << PC6); // Nolla avbrottspinnen så att vi i framtiden kan genera avbrott igen
}

//Om vi får vår adress kommer vi hit. 
//Vi kollar då upp om vi ska skicka eller ta emot data och utför enligt önskemål. 
byte incomingData(void){	
	if((TWSR & 0xF8) == 0x60){ // received data, acknowledge sent	
		i2c_data[bytefrom_i2c] = i2c_recive(); // Ta emot en byte data.
		if(bytefrom_i2c < ((i2c_data[0]>>4) &0x0f)){ // Om vi inte har tagit emot all data denna sändning gör vi oss klara för mer data
			TWCR &= ~(1 << TWINT);
		}
		else if((bytefrom_i2c) >= ((i2c_data[0]>>4)&0x0f)) // Annars sätter vi flaggan i2c_new_data till true och behandlar nästa gång i loopen.
			i2c_new_data = true;
		bytefrom_i2c++;
	}
	else if((TWSR & 0xF8)==0xA8){ // send data, acknowledge sent
		i2c_send(reciverAdress,dataToSend[bytesSent++]); // skicka en byte.
	}
	else if((TWSR & 0xF8)==0x80){// rep start, händer aldrig för stunden. 
		i2c_recive();
	}
	return 0x00;
}


// Skickar en byte till processor med adressen prossesor.
void i2c_send(byte prossesor,byte data){
	TWDR = data;
	TWCR |= (1<<TWEA) | (1<<TWINT);
	while(!(TWCR & (1<<TWINT)));
	if((TWSR & 0xF8) != TW_ST_DATA_ACK){
		TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);	// Transmition STOP
		// om vi vill skicka fler bytes så skapar vi ett nytt avbrott hos mastern här.
		// Vi kommer då tillbaks hit om en liten stund och kan på så sätt skicka hela meddelanden. 
		if(bytes_to_send_i2c >= bytesSent){ 
			_delay_us(20);
			PORTC |= (1 << PC6);
			_delay_us(11);
			PORTC &= ~(1 << PC6);
		}
		return ;
	}
}

// Tar emot en byte ifrån i2c
byte i2c_recive(void){
	TWCR |= (1<<TWEA) | (1<<TWINT);
	while(!(TWCR & (1<<TWINT)));
	byte data = TWDR;
	return data;	
}

// Behandlar i2cmodulen, i detta fall så skickar vi bara vidare datan över bluetooth när vi har tagit emot ett meddelande.
void i2c_handle_data(void){
	if(i2c_new_data == true){
		bluetooth_add_to_send_queue(i2c_data);
		i2c_new_data = false;
		bytefrom_i2c = 0;
		TWCR &= ~(1 << TWINT); // Sist sätter vi avbrottsflaggan för att visa att vi är redo att ta emot mer data. 
	}
}