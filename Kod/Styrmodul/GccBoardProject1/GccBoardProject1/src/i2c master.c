void i2c_setup(bool master);
bool i2c_send(byte prossesor,byte* data);
bool i2c_recive();

void i2c_setup(bool master) {
	if (master){
		DDRC =1<<PC6;
		PORTC = 1 <<PC6;
		
		DDRC =1<<PC7;
		PORTC = 1 <<PC7;
		
		EIMSK = 1<<INT0;					// Enable INT0
		MCUCR = 1<<ISC01 | 1<<ISC00;	// Trigger INT0 on rising edge
	}
	
};
	
bool i2c_send(byte prossesor,byte* data){
	
};
	
bool i2c_recive(){
	
};