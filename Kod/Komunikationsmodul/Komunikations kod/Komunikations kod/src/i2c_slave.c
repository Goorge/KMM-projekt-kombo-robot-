void i2c_setup(byte adress_);
void requestToSend(byte adress, byte* data);
byte incomingData();
void i2c_send(byte prossesor,byte data);
byte i2c_recive();

byte* dataToSend;
byte reciverAdress;
int bytesSent = 0;

byte firstdata;
byte extradata[15];
int datatoget=0;

byte return_firstdata(){
	return firstdata;
	}

ISR(TWI_vect){
	cli();
	firstdata=incomingData();
	sei();
	
}

void i2c_setup(byte adress_) {
	TWAR = adress_;
	TWSR =0x00;
	TWCR = (1 << TWIE) | (1 << TWEN)| (1<<TWEA) | (1<<TWINT) & (0<<TWSTA) & (0<<TWSTO);
};

void requestToSend(byte adress, byte* data){
	dataToSend = data;
	reciverAdress = adress;
	bytesSent = 0;
}


byte incomingData(){
	
	int counter=0;
	if((TWSR & 0xF8)==0x60)
	{	
		return i2c_recive();
	}
	else if((TWSR & 0xF8)==0xA8)
	{
		i2c_send(reciverAdress,dataToSend[bytesSent++]);
	}
	else if((TWSR & 0xF8)==0x80)// blir 0xF8
	{
		return i2c_recive();
	}
	return 0x00;
}



void i2c_send(byte prossesor,byte data){
	TWDR = data;
	while(!(TWCR & (1<<TWINT))); // wait for SLA+W transmited and ACK/NACK recived
};

byte i2c_recive(){
	TWCR = (1<<TWEA) | (1<<TWINT) & (0<<TWSTO);
	//if((TWSR & 0xF8)!=0x80)
	//	return false;
	TWCR = (1<<TWEA) | (1<<TWINT);
	return TWDR;	
};