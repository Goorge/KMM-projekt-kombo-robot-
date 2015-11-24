#include "bluetooth.h"
void i2c_setup(byte adress_);
void requestToSend(byte adress, byte* data);
byte incomingData();
void i2c_send(byte prossesor,byte data);
byte i2c_recive();

byte dataToSend[15];
byte reciverAdress;
int bytesSent = 0;
int bytefrom_i2c=0;
byte extradata[15];
bool newdata = false;



ISR(TWI_vect){
	incomingData();	
	TWCR = (1 << TWIE) | (1 << TWEN)| (1<<TWEA) | (1<<TWINT);
}

void i2c_setup(byte adress_) {
	DDRC |= (1 << PC6);
	TWAR = adress_;
	TWSR =0x00;
	TWCR = (1 << TWIE) | (1 << TWEN)| (1<<TWEA) | (1<<TWINT) & (0<<TWSTA) & (0<<TWSTO);
};

void requestToSend(byte adress, byte data[]){
	for(int i = 0; i < NELEMS(data); i++)
		dataToSend[i] = data[i];
	reciverAdress = adress;
	bytesSent = 0;
	//TWCR = (1 << TWIE) | (1 << TWEN)| (1<<TWEA) | (1<<TWINT);
	PORTC |= (1 << PC6);
}


byte incomingData(){	
	int counter=0;
	if((TWSR & 0xF8)==0x60)
	{	
		extradata[bytefrom_i2c] = i2c_recive();
		if(bytefrom_i2c==extradata[0]>>4 &0x0f){
			newdata=true;
			TWCR &= ~(1 << TWINT) ;
		}
		return extradata[bytefrom_i2c++];
	}
	else if((TWSR & 0xF8)==0xA8)
	{
		PORTC &= ~(1 << PC6);
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
	TWCR = (1<<TWEA) | (1<<TWINT);
	bluetooth_send_byte(0x0A);
	//while(!(TWCR & (1<<TWINT))); // wait for SLA+W transmited and ACK/NACK recived
	//if((TWSR & 0xF8)!=0xB8){
	//	bluetooth_send_byte(0x0f);
	//	return false;
	//}
	bluetooth_send_byte(0x09);
	TWCR = (1<<TWEA) | (1<<TWINT);
};

byte i2c_recive(){
	if((TWSR & 0xF8)!=0x80)
		return false;
	TWCR = (1<<TWEA) | (1<<TWINT);
	return TWDR;	
};