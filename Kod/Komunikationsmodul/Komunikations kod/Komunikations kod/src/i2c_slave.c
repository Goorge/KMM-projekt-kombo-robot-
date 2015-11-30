#include "i2c_slave.h"

byte dataToSend[15];
byte reciverAdress;
int bytes_to_send_i2c = 0;
int bytesSent = 0;
int bytefrom_i2c=0;
byte i2c_data[15];
bool newdata = false;
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
		if(bytefrom_i2c == ((i2c_data[0]>>4) &0x0f)){
			newdata = true;
			TWCR &= ~(1 << TWINT);
		}
		if((bytefrom_i2c+1) == ((i2c_data[0]>>4)&0x0f))
			i2c_new_data=true;
		return i2c_data[bytefrom_i2c++];
		
	}
	else if((TWSR & 0xF8)==0xA8){ // send data, ack sent
		i2c_send(reciverAdress,dataToSend[bytesSent++]);
	}
	else if((TWSR & 0xF8)==0x80){// rep start? 
		return i2c_recive();
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
	if((TWSR & 0xF8)!=0x80)
		return false;
	TWCR |= (1<<TWEA) | (1<<TWINT);
	return TWDR;	
}

void i2c_handle_data(void){
	if(i2c_new_data==true)
	{
		bluetooth_add_to_send_queue(i2c_data);
		i2c_new_data=false;
		switch (i2c_data[0] & 0x0f){
			case 0x00 :
			
			break;
			case 0x01 :
			
			break;
			case 0x02 :
			
			break;
			case 0x03 :
			
			break;
			case 0x04 :
			
			break;
			case 0x05 :
			
			break;
			case 0x06 :
			
			break;
			case 0x07 :
			
			break;
			case 0x08 :
			
			break;
			case 0x09 :
			
			break;
			case 0x0a :
			
			break;
			case 0x0b :
			
			break;
			case 0x0c :
			
			break;
			case 0x0d :
			
			break;
			case 0x0e :
			
			break;
			case 0x0f :
			
			break;
			default :
			break;
		}
	}
}