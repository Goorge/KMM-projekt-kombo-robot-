#include <avr/interrupt.h>
#include <avr/io.h>
#include "asf.h"


int regulator_mode=0;


void regulator(){
	if(regulator_mode==1){
		//linje();
		linje_main();
	}
	else
	{	
		labyreg();//kalla på labyrint funktion
	}
}