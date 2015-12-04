/*
 * kalibrering.c
 *
 * Created: 11/5/2015 10:11:58 AM
 *  Author: calva694
 */ 

/*F�R KALIBRERING

1. St�ll robot p� golvet, helt utan tejp
2. Tryck p� kalibrering
3. N�r lysdioderna blinkar 3 g�nger funkar d� kalibrerat o fint!
*/

#include "kalibrering.h"
#include "kal_struct.h"

#define F_CPU 14.7456E6
#include <util/delay.h>

void kalibrering()
{
  const uint8_t highest_value = 205;
  uint8_t lowest_value;
  //uint8_t reflex_values[11];
  uint8_t diff;
  uint8_t i;
	
  for(i=0; i<11; ++i)
    {
      lowest_value = mux_sensors(i);
	  diff = (highest_value - lowest_value)/4;
	  
	  reflex_kal[i].low = lowest_value + diff;
      reflex_kal[i].middle = reflex_kal[i].low + diff;
      reflex_kal[i].high = reflex_kal[i].middle + diff;
    }
	
	PORTD &= ~(0x60);
	_delay_ms(200);
	PORTD |= 0x60;
	_delay_ms(500);
	PORTD &= ~(0x60);
	_delay_ms(200);
	PORTD |= 0x60;
	_delay_ms(200);
	PORTD &= ~(0x60);
	_delay_ms(200);
	PORTD |= 0x60;
	_delay_ms(200);
	PORTD &= ~(0x60);
	
  /*
    Vi vill i slut�ndan returnerna 00, 01, 10 eller 11 f�r varje reflexsensor.
    Om vi tar h�gsta v�rdet (dvs, d�r den st�r p� linjen) och tar det 
    minus det minsta v�rdet (d�r den reflekterar som mest, golvet)
    och delar p� 4 s� delar vi in i 4 stycken olika intervall, 00, 01, 10 och 11.
    F�r att f� lite marginal s� tar vi halva avst�ndet upp�t ocks�. 
  */
};

/*uint8_t find_min_value(const uint8_t reflex_values[])
{
  uint8_t i;
  uint8_t min = reflex_values[0];
	
  for(i = 1; i<11; ++i)
    {
      if(reflex_values[i] < min)
	{
	  min = reflex_values[i];
	}
    }
  return min;
}*/

/*uint8_t find_max_value(const uint8_t reflex_values[])
{
  uint8_t i;
  uint8_t max = reflex_values[0];
	
  for(i = 1; i<11; ++i)
    {
      if(reflex_values[i] > max)
	{
	  max = reflex_values[i];
	}
    }
	
  return max;
}*/
