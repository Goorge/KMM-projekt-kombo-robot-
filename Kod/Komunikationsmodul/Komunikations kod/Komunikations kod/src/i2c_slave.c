void i2c_setup(byte adress_);
void requestToSend(byte adress, byte* data);
byte incomingData();
void i2c_send(byte prossesor,byte data);
byte i2c_recive();

byte dataToSend[15];
byte reciverAdress;
int bytes_to_send_i2c = 0;
int bytesSent = 0;
int bytefrom_i2c=0;
byte extradata[15];
bool newdata = false;



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

void requestToSend(byte adress, byte data[]){
	bytes_to_send_i2c = (data[0] >> 4) & 0x0f;
	for(int i = 0; i < bytes_to_send_i2c+1; i++)
		dataToSend[i] = data[i];
	reciverAdress = adress;
	bytesSent = 0;
	//TWCR = (1 << TWIE) | (1 << TWEN)| (1<<TWEA) | (1<<TWINT);
	PORTC |= (1 << PC6);
	PORTC &= ~(1 << PC6);
}


byte incomingData(){	
	int counter=0;
	if((TWSR & 0xF8)==0x60){ // rec data, ack sent	
		extradata[bytefrom_i2c] = i2c_recive();
		if(bytefrom_i2c==extradata[0]>>4 &0x0f){
			newdata=true;
			TWCR &= ~(1 << TWINT);
		}
		return extradata[bytefrom_i2c++];
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
			PORTC &= ~(1 << PC6);
		}
		
		return false;
	}
}

byte i2c_recive(){
	if((TWSR & 0xF8)!=0x80)
		return false;
	TWCR |= (1<<TWEA) | (1<<TWINT);
	return TWDR;	
}