/*
 * kalibrering.c
 *
 * Created: 11/5/2015 10:11:58 AM
 *  Author: calva694
 */ 

/*FÖR KALIBRERING

1. Ställ robot på golvet, helt utan tejp
2. Tryck på kalibrering
3. När lysdioderna blinkar 3 gånger funkar dä kalibrerat o fint!
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
    Vi vill i slutändan returnerna 00, 01, 10 eller 11 för varje reflexsensor.
    Om vi tar högsta värdet (dvs, där den står på linjen) och tar det 
    minus det minsta värdet (där den reflekterar som mest, golvet)
    och delar på 4 så delar vi in i 4 stycken olika intervall, 00, 01, 10 och 11.
    För att få lite marginal så tar vi halva avståndet uppåt också. 
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
