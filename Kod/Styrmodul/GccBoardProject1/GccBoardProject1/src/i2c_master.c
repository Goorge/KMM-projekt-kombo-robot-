void i2c_setup(bool master);
bool i2c_send(byte prossesor,byte* data);
byte i2c_recive(byte prossesor);
void i2c_store_data(byte data);
void i2c_handel_data();


byte i2c_data[15];
bool i2c_newdata=false;

ISR(INT0_vect)
{	
	if (PINC&(1<<PC6)!=0){	//komunikation vill skicka
		i2c_store_data(i2c_recive(0x02)); // processor 1
		
	}
	else if(PINC&(1<<PC7)!=0){ //sensor vill skicka
		i2c_store_data(i2c_recive(0x06)); // processor 3
	}
}

void i2c_setup(bool master) {
	if (master){
		DDRC &=~(1<<PC6);
		//PORTC = 1 <<PC6;
		
		DDRC &=~(1<<PC7);
		//PORTC = 1 <<PC7;
		
		EIMSK = 1<<INT0;					// Enable INT0
		EICRA |= (1<<ISC01)|(1<<ISC00); // Trigger INT0 on rising edge
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
	
byte i2c_recive(byte prossesor){
	byte* data;
	int counter=0;
	int start =TW_START;
	int size = 0;

	TWCR |= (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);//START
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
	
	TWCR |= (1<<TWINT)|(1<<TWEN);
	while(!(TWCR & (1<<TWINT)));
	if((TWSR & 0xF8) != 0x50)
	{
		TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);	// Transmition STOP
		
		return false;
	}
	if(counter == 0){
		data = data[(TWDR>>4) & 0x0f];
		size= (TWDR>>4) & 0x0f;
	}
	data[counter]=TWDR;
	counter++;	
	start=TW_REP_START;
	TWCR = (1<<TWINT);
	while(!(TWCR & (1<<TWINT)));
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);	// Transmition STOP
	return data;
};



void i2c_store_data(byte data)
{
	static int counter;
	static int size;
	if(counter=0)
		size= (data>>4) & 0x0f;
	else if(counter<size)
	{
		i2c_data[counter];
		counter++;
	}
	else
		{
			counter=0;
			i2c_newdata=true;  
		}
	
}


void i2c_handel_data(){
	if(i2c_newdata==true)
	{
		if((i2c_data[0]>>3)&0x01==0)
			i2c_send(0x02,i2c_data);
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