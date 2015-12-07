void i2c_setup(bool master);
bool i2c_send(byte prossesor,byte* data);
byte i2c_recive(byte prossesor);
void i2c_store_data(byte data);
void i2c_handel_data();

byte i2c_data[15];
bool i2c_newdata=false;
int Reflex_data;
//aidshub was here
ISR(INT0_vect)// händer om annan processor kör request to send
{	
	_delay_us(1);
	if (PINC&(1<< PC6)){	//komunikation vill skicka
		i2c_store_data(i2c_recive(0x02)); // adress processor 1
		//PORTD ^=(1 << PD0);
	}
	else if(PINC&(1<< PC7)){ //sensor vill skicka  
		i2c_store_data(i2c_recive(0x06)); // adress processor 3
	}
}

void i2c_setup(bool master) {
	if (master){
		DDRC &=~(1<<PC6);
		//PORTC = 1 <<PC6;
		
		DDRC &=~(1<<PC7);
		//PORTC = 1 <<PC7;
		
		EIMSK  |= 1<<INT0;					// Enable INT0
		EICRA |= (1<<ISC01)|(1<<ISC00); // Trigger INT0 on rising edge
		TWBR = 0x10;
		TWSR = (0<<TWPS1)|(0<<TWPS0); // set TWI till master
	}
};
	
bool i2c_send(byte prossesor,byte data[]){ //skicka arayen data till prossesor
	int number_bytes =(( data[0]>>4 ) & 0x0f);
	int counter = 0;
	int start = TW_START;
	
	do{
	
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN); //send START
	while(!(TWCR & (1<<TWINT))); //Wait for TWINT, START is now sent
	
	if((TWSR & 0xF8) != start) // om status inte är start
		return false;		
	TWDR = prossesor&0xfe;//sista bit R/W sätt till Write
	TWCR = (1<<TWINT) | (1<<TWEN);// start transmito of addres
	while(!(TWCR & (1<<TWINT))); // wait for SLA+W transmited and ACK/NACK recived
	if((TWSR & 0xF8) !=0x18) //om adress skickat och fått ack
	{
		TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);	// Transmition STOP	
		return false;
	}
		
	TWDR = data[counter];		//placera data i TWI buferten
	TWCR = (1<<TWINT) | (1<<TWEN);	// start send data	
	while(!(TWCR & (1<<TWINT))); //wait for data transmitted and ACK/NACK	
	if((TWSR & 0xF8) != TW_MT_DATA_ACK) //om data sent och ack recived
	{
		TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);	// Transmition STOP
		return false;
	}
	counter++;
	
	start=TW_REP_START;	// om vi ska skicka mer data gör repeated start istället för start
	}while (counter <= number_bytes);	//om vi har mer data att skicka skicka den
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);	// Transmition STOP	
	return true;
};
	
byte i2c_recive(byte prossesor){
	byte* data;
	int counter=0;
	int start =TW_START;
	//int size = 0;

	TWCR |= (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);//START
	while(!(TWCR & (1<<TWINT))); //Wait for TWINT, START is now sent
	if((TWSR & 0xF8) != start) // om status en start eventuellt bara tw_start
	{
		TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);	// Transmition STOP
		return false;
	}
	TWDR = prossesor | 0x01;	//adress och Read 
	TWCR = (1<<TWINT)|(1<<TWEN);
	while(!(TWCR & (1<<TWINT))); // wait for SLA+R transmited and ACK/NACK recived
	if((TWSR & 0xF8) != TW_MR_SLA_ACK) // om slav adress har skickats och ack har fåtts
	{
		TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);	// Transmition STOP
		return false;
	}
	
	TWCR |= (1<<TWINT)|(1<<TWEN);
	while(!(TWCR & (1<<TWINT)));
	/*if((TWSR & 0xF8) != 0x50) //missar att få ack 
	{
		TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);	// Transmition STOP
		
		return false;
	}*/
	/*if(counter == 0){
		data = data[(TWDR>>4) & 0x0f];
		size= (TWDR>>4) & 0x0f;
	}*/
	data = TWDR; //ta emot data 
	counter++;	
	start=TW_REP_START;
	//TWCR = (1<<TWINT);
	//while(!(TWCR & (1<<TWINT)));
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);	// Transmition STOP
	return data;
}



/*void i2c_store_data(byte data)
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
		EIMSK &= ~(1<<INT0);
		
	}
	
}*/
/*void i2c_store_data(byte data)
{
	static int counter;
	static int size;
	if(counter == 0){
		size = (data>>4) & 0x0f;
		i2c_data[counter] = data;
		counter++;
	}
	else if(counter < size){
		i2c_data[counter] = data;
		counter++;
	}
	else{
		i2c_data[counter] = data;
		i2c_newdata = true;
		counter = 0;
		EIMSK &= ~(1<<INT0);
	}
	
}
*/ //viktor tog bort

void i2c_store_data(byte data) //bygger ihop data till en aray liknande den som blev skickad och sätter flaggan i2c_newdata då all data i ett paket motaget
{
	static int counter;
	static int size;
	if(counter == 0){ //om första databiten ta ut mängd data som kommer komma 
		size = (data>>4) & 0x0f;
		i2c_data[counter] = data;
		counter++;
	}
	else if(counter < size+1){ //fyll på arayen med data
		i2c_data[counter] = data;
		counter++;
	}
	else{
		i2c_data[counter] = data;
	}
	if(counter>=size+1){ // aray färdig all data motagen
		i2c_newdata = true;
		counter = 0;
		EIMSK &= ~(1<<INT0);
	}

}

void i2c_handel_data(void){ //hanterar den motagna datan och läger den på rätt plats och skickar vidare sensordata till komunikation
	if(i2c_newdata==true)
	{
		i2c_newdata=false;
		if(!((i2c_data[0]>>3)&0x01)){
			_delay_us(300);
			i2c_send(0x02,i2c_data);// send to komunikation
		}
		switch (i2c_data[0] & 0x0f){
			case 0x00 ://batteri nivå
				batteri=i2c_data[1],i2c_data[2];
				break;
			case 0x01 ://avståndssensorer
				distans_right = i2c_data[3];
				distans_fram = i2c_data[2];;
				distans_left = i2c_data[1];
				//updaterad_labyrint = true;
				break;
			case 0x02 :// refelxsensor data
				regler_ready_linje = 1;					// Ny data har lästs in då uppdaterar vi regleringen
				Reflex_data = (i2c_data[2]<<8) + i2c_data[1];
				Reflex_data2 = i2c_data[3];// går inte att få in mer än 16 bitar i en int...
				break;
			case 0x03 :
				RGB_data=1;//"röd";"
				break;
			case 0x04 :
				RGB_data=2;//"grön";
				break;		
			case 0x05 :
				RGB_data=3;//"blå";
				break;
			case 0x06 : // gyro klart
				gyro_90 = true;
				turning = false;
				start = 0;
				break;
			case 0x07 :// gyro data
				// Kod vi skickar till sensor
				break;
			case 0x08 : // manuel stå still
				manual_function=0;
				break;
			case 0x09 : //manuel kar rakt fram
				manual_function=1;			
				break;
			case 0x0a : //manuel kör bakåt
				manual_function=2;				
				break;
			case 0x0b ://manuel vänster fram
				manual_function=4;
				break;
			case 0x0c ://manuel höger fram
				manual_function=3;
				break;
			case 0x0d :// spin på stället åt vänster
				manual_function=6;
				break;
			case 0x0e :// spin på stället åt höger
				manual_function=5;
				break;
			case 0x0f :	// GUI skickar en extra byte där vi behandlar knapparna "man/auto" samt "start" (av/på) (används för extrafunktioner i almenhet som inte får plats i vanliga data tabellen)
				if(i2c_data[1]==0xf0){
					if(start==1){
						start=0;
					}
					else{
						start=1;
					}
				}
				else if(i2c_data[1]==0x0f){
					if(drive_mode==1){
						PORTD |= (1 << PD1);
						PORTD &= ~(1 << PD0);
						drive_mode=0;
						start=0;
					}
					else{
						PORTD |= (1 << PD0);
						PORTD &= ~(1 << PD1);
						drive_mode=1;
						start=0;
					}
				}
				else if(i2c_data[1]==0x00){			// Ändra P och D konstant för linje
					d_constant = i2c_data[2];
					p_constant = i2c_data[3];
				}
				else if(i2c_data[1]==0x01){			// Ändra P och D konstant för labyrint
					d_constant_lab = i2c_data[2];
					p_constant_lab = i2c_data[3];
				}
				else if(i2c_data[1]==0x02){
					left = i2c_data[2];
					right = i2c_data[3];
					/*if((left < 0) || (right < 0)){		// safe för att inte användaren kan mata in negativa värden
						left = abs(left);
						right = abs(right);
					}*/
				}
				
				break;		
			default :
				break;
		}
		EIMSK |= (1<<INT0);
	}	
}