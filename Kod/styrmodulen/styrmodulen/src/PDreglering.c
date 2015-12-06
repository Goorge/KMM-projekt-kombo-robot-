#include <avr/interrupt.h>
#include <avr/io.h>
#include "asf.h"

void regulator(){
	if((regulator_mode == 1) && (regler_ready_linje == 1)){
		linje_main();
		regler_ready_linje = 0;
	}
	else if(regulator_mode == 0){	
		//labyreg();//kalla på labyrint funktion	
	}
}