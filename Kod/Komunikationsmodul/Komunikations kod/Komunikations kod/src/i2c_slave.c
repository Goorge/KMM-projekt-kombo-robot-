void i2c_setup(byte adress_);
void requestToSend(byte adress, byte* data);
byte incomingData();
void i2c_send(byte prossesor,byte data);
byte i2c_recive();

byte adress;
byte* dataToSend;
byte reciverAdress;
int bytesSent = 0;

void i2c_setup(byte adress_) {
	adress = adress_;
	TWAR = adress;
	TWCR = (1 << TWIE) | (1 << TWEN)| (1<<TWEA) | (1<<TWINT);
};

void requestToSend(byte adress, byte* data){
	dataToSend = data;
	reciverAdress = adress;
	bytesSent = 0;
}

byte incomingData(){
	int counter=0;
	TWAR = adress;
	if((TWSR & 0xF8)==0x60)
	{
		i2c_send(reciverAdress,dataToSend[bytesSent++]);
	}
	else if((TWSR & 0xF8)==0xA8)// blir 0xF8
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
	while(!(TWCR & (1<<TWINT))); // wait for SLA+W transmited and ACK/NACK recived
	if((TWSR & 0xF8) != TW_MT_SLA_ACK)
		return false;
	return TWDR;
};