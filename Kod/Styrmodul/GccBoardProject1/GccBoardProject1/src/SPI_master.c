void SPI_MasterInit(void);
void SPI_MasterTransmit(char cData[],char Prossesor);
void SPI_Recive(char Prossesor);

#define DDR_SPI DDRB
#define DD_MISO PB3
#define DD_MOSI PB2
#define DD_SCK PB1
#define DD_SS PB0

char recived_data[10];
bool recive_flag;

void SPI_MasterInit(void)
{
	/* Set MOSI and SCK output, all others input */
	DDR_SPI = (1<<DD_MOSI)|(1<<DD_SCK)|(1<<DD_SS);
	/* Enable SPI, Master, set clock rate fck/16 */
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
	// enable slave request
	EIMSK = 1<<INT0;					// Enable INT0
	MCUCR = 1<<ISC01 | 1<<ISC00;	// Trigger INT0 on rising edge
}

void SPI_MasterTransmit(char cData[],char Prossesor)
{
	int counter=0;
	if (Prossesor==0x01) //send to komunikation
		PORTB=(1<<DD_SS);
	else //send to sensor
		PORTB=(1<<PB3);
	do{	
	/* Start transmission */
	SPDR = cData[counter];
	counter++;
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)));
	
	}while(counter<(cData[0]>>4 ) & 0x0f);
	PORTB=(0<<DD_SS) & (0<<PB3); 
}

void SPI_Recive(char Prossesor)
{
	int counter=0;
	do{
	if (Prossesor==0x01) //recive from komunikation
		PORTB=(1<<DD_SS);
	else //recive from sensor
		PORTB=(1<<PB3);
	/* Wait for reception complete */
	while(!(SPSR & (1<<SPIF)));
	/* Return Data Register */
	recived_data[counter] = SPDR;
	counter++;
	}while(PC6|PC7); //still sending data
	
	//kod för att skicka vidare data
	if (recived_data[0] & 0x08==0x08)
	{	
		SPI_MasterTransmit(recived_data,0x01); //till komunikation
		recive_flag=1;
	}
	else if (recived_data[0] & 0x0f==0x00)
		SPI_MasterTransmit(recived_data,0x03); //till sensor
	else
		recive_flag=1;
}

