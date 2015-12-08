/*
 * avstand.c
 *
 * Created: 11/10/2015 2:21:58 PM
 *  Author: calva694
 */ 

#include "mainsensor.h"

void avstand_init()
{
	DDRD |= 0x1B;
	PORTD |= 0x03;
	_delay_ms(40);
}

void read_avstand_sensor(uint8_t *counter)
{
	/*11 �r rakt fram
	12 �r v�nster l�ng
	13 �r h�ger l�ng
	14 �r v�nster kort
	15 �r h�ger kort*/
	
	/*uint8_t hoger = avstand(11); //avst�nd h�ger
	uint8_t vanster = avstand(13); //avst�nd v�nster
	uint8_t rakt = avstand(15);	//avst�nd fram*/
	//DDRD |= 0x60;
	//skicka hoger, vanster och rakt via I2C 
	
	uint8_t rakt = mux_sensors(11);		//rakt fram, l�ng sensor
	uint8_t vanster = mux_sensors(14);	//v�nster, mellansensor
	uint8_t hoger = mux_sensors(15);	//h�ger, mellansensor
	
	rakt = look_up_long(rakt);
	vanster = look_up_mellan(vanster);
	hoger = look_up_mellan(hoger);

	if(*counter > 9 && (vanster < 25 || hoger < 25))	//Under 25cm f�r att s�kerst�lla att vi har en v�gg i labyrinten
	{
		*counter = 0;
		_delay_ms(10);	//Med anledning av I2C:ns hastighetsbegr�nsning
	}
	else if (vanster > 34 || hoger > 34)	//�ver 34cm och vi ska anv�nda l�ng sensor f�r att f� b�ttre precision
	{
		++*counter;
		long_init();
		if(vanster > 34)					//�ver 34 cm f�r vi h�gre precision med l�nga sensorn
		{
			vanster = mux_sensors(12);		//H�mtar nytt v�rde fr�n l�nga sensorn till v�nster
			vanster = look_up_long(vanster);
		}
		if(hoger > 34)						//�ver 34 cm f�r vi h�gre precision med l�nga sensorn
		{
			hoger = mux_sensors(13);		//H�mtar nytt v�rde fr�n l�nga sensorn till h�ger
			hoger = look_up_long(hoger);
		}
		clear_long();
	}
	else //Mellan 25 och 34 cm kan vi inte s�kerst�lla att vi �r i labyrinten, men vi vill �nd� skicka mellansensorn
	{
		_delay_ms(10);	//Pga I2Cs hastighetsbegr�nsning
	}
	
	uint8_t data_to_send [4];
	data_to_send[0] = 0x31;
	data_to_send[1] = vanster;
	data_to_send[2] = rakt;
	data_to_send[3] = hoger;
	i2c_requestToSend(0x04, data_to_send);
}

uint8_t look_up_mellan (uint8_t value)
{
	if(value>83)
	{
		return 13;
	}
	else if(value>70)
	{
		return 14;
	}
	else if(value>64)
	{
		return 15;
	}
	else if(value > 60)
	{
		return 16;
	}
	else if(value>57)
	{
		return 17;	
	}
	else if(value > 55)
	{
		return 18;
	}
	else if(value > 53)
	{
		return 19;
	}
	else if(value > 51)
	{
		return 20;
	}
	else if(value > 49)
	{
		return 21;
	}
	else if(value > 47)
	{
		return 22;
	}
	else if(value > 46)
	{
		return 23;
	}
	else if(value > 45)
	{
		return 24;
	}
	else if(value > 44)
	{
		return 25;
	}
	else if(value > 40)
	{
		return 28 ;
	}
	else if(value>37)
	{
		return 30;
	}	
	else if(value>32)
	{
		return 35;
	}
	else if(value>25)
	{
		return 40;
	}
	else if(value>21)
	{
		return 50;
	}
	else if(value>17)
	{
		return 60;
	}
	else
	{
		return 70;
	}
}

uint8_t look_up_long(uint8_t value)
{
	if(value>81)
	{
		return 30;
	}
	else if(value>71)
	{
		return 35;
	}
	else if(value>58)
	{
		return 40;	
	}
	else if(value>49)
	{
		return 50;
	}
	else if(value>42)
	{
		return 60;
	}
	else if(value>35)
	{
		return 70;
	}
	else if(value>30)
	{
		return 80;
	}
	else if(value>27)
	{
		return 90;
	}
	else if(value>17)
	{
		return 100;
	}
	else if(value>14)
	{
		return 120;
	}
	else if(value>10)
	{
		return 140;
	}
	else
	{
		return 150;
	}
}

void long_init()
{
	PORTD &= ~(0x03);
	_delay_ms(1);
	PORTD |= 0x18;
	_delay_ms(50);
}

void clear_long()
{
	PORTD &= ~(0x1B);
	PORTD |= 0x03;
	_delay_ms(30);
}