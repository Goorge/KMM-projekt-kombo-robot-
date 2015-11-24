void i2c_setup(bool master);
bool i2c_send(byte prossesor,byte* data);
byte* i2c_recive(byte prossesor);

void i2c_setup(bool master) {
	if (master){
		DDRC =0<<PC6;
		//PORTC = 1 <<PC6;
		
		DDRC =0<<PC7;
		//PORTC = 1 <<PC7;
		
		EIMSK = 1<<INT0;					// Enable INT0
		MCUCR = 1<<ISC01 | 1<<ISC00;	// Trigger INT0 on rising edge
		
		TWBR = 0x10;
		TWSR = (0<<TWPS1)|(0<<TWPS0);
	}
	
};
	
bool i2c_send(byte prossesor,byte data[]){
	int number_bytes =( data[0]>>4 ) & 0x0f;
	int counter = 0;
	int start = TW_START;
	
	do{
	
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN); //send START
	while(!(TWCR & (1<<TWINT))); //Wait for TWINT, START is now sent
	
	if((TWSR & 0xF8) != start) // om status en start
		return false;		
	TWDR = prossesor&0xfe;//sista bit R/W
	TWCR = (1<<TWINT) | (1<<TWEN);// start transmito of addres
	while(!(TWCR & (1<<TWINT))); // wait for SLA+W transmited and ACK/NACK recived
	if((TWSR & 0xF8) !=0x18)
	{
		TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);	// Transmition STOP	
		return false;
	}
		
	TWDR = data[counter];
	TWCR = (1<<TWINT) | (1<<TWEN);	// start send data	
	while(!(TWCR & (1<<TWINT))); //wait for data transmitted and ACK/NACK	
	if((TWSR & 0xF8) != TW_MT_DATA_ACK)
	{
		TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);	// Transmition STOP
		return false;
	}
	counter++;
	
	start=TW_REP_START;
	}while (counter <= number_bytes);
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);	// Transmition STOP	
	return true;
};
	
byte* i2c_recive(byte prossesor){
	byte* data;
	int counter=0;
	int start =TW_START;
	int size = 0;
	do{
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);//START
	while(!(TWCR & (1<<TWINT))); //Wait for TWINT, START is now sent
	if((TWSR & 0xF8) != start) // om status en start eventuellt bara tw_start
	{
		TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);	// Transmition STOP
		return false;
	}
	TWDR = prossesor | 0x01;
	TWCR = (1<<TWINT)|(1<<TWEN);
	while(!(TWCR & (1<<TWINT))); // wait for SLA+R transmited and ACK/NACK recived
	if((TWSR & 0xF8) != TW_MR_SLA_ACK)
	{
		TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);	// Transmition STOP
		return false;
	}
	TWCR = (1<<TWINT)|(1<<TWEN);
	if(counter == 0){
		data = data[TWDR<<4&0x0f];
		size= TWDR<<4&0x0f;
	}
	data[counter]=TWDR;
	counter++;	
	start=TW_REP_START;
	}while(counter<=size);
	if(!(TWCR==(1<<TWINT)|(1<<TWSTO)|(1<<TWEN)))
	{
		TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);	// Transmition STOP
		return false;
	}
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);	// Transmition STOP
	return data;
};