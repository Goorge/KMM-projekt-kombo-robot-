#include <avr/interrupt.h>
#include <avr/io.h>
#include "asf.h"

void regulator(){
	if(regulator_mode == 1){
		linje_main();
	}
	else
	{	
		labyreg();//kalla på labyrint funktion
	}
}