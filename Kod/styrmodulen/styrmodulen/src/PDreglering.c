#include <avr/interrupt.h>
#include <avr/io.h>
#include "asf.h"

int waypoint=0;
int oneway_turn=0;
int regulator_mode=1;

void regulator(){
	if(regulator_mode==1){
		linje();
	}
	else
	{
		if(waypoint==1)
			waypoint_lab();
		if(oneway_turn=1)	
			oneway_turn_lab();
		labyreg();//kalla på labyrint funktion
	}
}