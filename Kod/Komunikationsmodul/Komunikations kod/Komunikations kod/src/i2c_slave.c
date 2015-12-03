#include "i2c_slave.h"

byte dataToSend[15];
byte reciverAdress;
int bytes_to_send_i2c = 0;
int bytesSent = 0;
int bytefrom_i2c=0;
byte i2c_data[15];
bool i2c_new_data = false;


ISR(TWI_vect){
	incomingData();	
	TWCR = (1 << TWIE) | (1 << TWEN)| (1<<TWEA) | (1<<TWINT);//TWCR |= (1<<TWEA) | (1<<TWINT); 
}

void i2c_setup(byte adress_) {
	TWAR = adress_;
	//TWSR = 0x00; // Ska nog inte vara med
	TWCR = (1 << TWIE) | (1 << TWEN)| (1<<TWEA) | (1<<TWINT);
	DDRC |= (1 << PC6);
};

void i2c_requestToSend(byte adress, byte data[]){
	bytes_to_send_i2c = (data[0] >> 4) & 0x0f;
	for(int i = 0; i < bytes_to_send_i2c+1; i++)
		dataToSend[i] = data[i];
	reciverAdress = adress;
	bytesSent = 0;
	PORTC |= (1 << PC6);
	_delay_us(11);
	PORTC &= ~(1 << PC6);
}


byte incomingData(void){	
	if((TWSR & 0xF8) == 0x60){ // rec data, ack sent	
		i2c_data[bytefrom_i2c] = i2c_recive();
		if(bytefrom_i2c < ((i2c_data[0]>>4) &0x0f)){
			TWCR &= ~(1 << TWINT);
		}
		else if((bytefrom_i2c) >= ((i2c_data[0]>>4)&0x0f))
			i2c_new_data = true;
		bytefrom_i2c++;
	}
	else if((TWSR & 0xF8)==0xA8){ // send data, ack sent
		i2c_send(reciverAdress,dataToSend[bytesSent++]);
	}
	else if((TWSR & 0xF8)==0x80){// rep start? 
		i2c_recive();
	}
	
	return 0x00;
}



void i2c_send(byte prossesor,byte data){
	TWDR = data;
	TWCR |= (1<<TWEA) | (1<<TWINT);
	while(!(TWCR & (1<<TWINT)));
	if((TWSR & 0xF8) != TW_ST_DATA_ACK){
		TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);	// Transmition STOP
		if(bytes_to_send_i2c >= bytesSent){
			_delay_us(20);
			PORTC |= (1 << PC6);
			_delay_us(11);
			PORTC &= ~(1 << PC6);
		}
		return ;
	}
}

byte i2c_recive(void){
	//  if((TWSR & 0xF8)!=0x80)
	//	return false;
	TWCR |= (1<<TWEA) | (1<<TWINT);
	while(!(TWCR & (1<<TWINT)));
	byte test = TWDR;
	return test;	
}

void i2c_handle_data(void){
	if(i2c_new_data == true){
		//if(((i2c_data[0] >> 4) & 0x0f) == 3)
		bluetooth_add_to_send_queue(i2c_data);
		i2c_new_data = false;
		bytefrom_i2c = 0;
		TWCR &= ~(1 << TWINT);
	}
}