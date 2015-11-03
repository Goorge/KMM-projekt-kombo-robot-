void i2c_setup(bool master);
bool i2c_send(byte prossesor,byte* data);
bool i2c_recive(byte prossesor);

byte adress;

void i2c_setup(byte adress_) {
	adress = adress_;
};

bool i2c_send(byte prossesor,byte* data){
	int number_bytes =(byte[0]>>4)&0x0f;
	int counter = 0;
	do{
		TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN); //send START
		while(!(TWCR & (1<<TWINT))); //Wait for TWINT, START is now sent
		if((TWSR & 0xF8) != TW_START) // om status en start
		return false;
		TWDR = prossesor;
		TWCR = (1<<TWINT) | (1<<TWEN);// start transmito of addres
		while(!(TWCR & (1<<TWINT))); // wait for SLA+W transmited and ACK/NACK recived
		if((TWSR & 0xF8) != TW_MT_SLA_ACK)
		return false;
		TWDR = data[counter];
		TWCR = (1<<TWINT) | (1<<TWEN);	// start send data
		while(!(TWCR & (1<<TWINT))); //wait for data transmitted and ACK/NACK
		if((TWSR & 0xF8) != TW_MT_DATA_ACK)
		return false;
		counter++;
	}while (counter < number_bytes)
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);	// Transmition STOP
	return true;
};

byte* i2c_recive(byte prossesor){
	byte* data;
	int counter=0;
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);//START
	while(!(TWCR & (1<<TWINT))); //Wait for TWINT, START is now sent
	if((TWSR & 0xF8) != TW_START) // om status en start
	return NULL;
	do{
		TWDR = prossesor;
		while(!(TWCR & (1<<TWINT))); // wait for SLA+R transmited and ACK/NACK recived
		if((TWSR & 0xF8) != TW_MT_SLA_ACK)
		return false;
		while(!TWINT);
		if(counter == 0)
		data = byte[TWDR<<4&0x0f];
		data[counter]=TWDR
		counter++;
	}while(TWSR==TW_REP_START)
	if(!(TWCR==(1<<TWINT)|(1<<TWSTO)|(1<<TWEN))
	return NULL;
	return data;
};