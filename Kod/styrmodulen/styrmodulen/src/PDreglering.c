#include <avr/interrupt.h>
#include <avr/io.h>
#include "asf.h"



void regulator(){

//	if(turning){ // Spinn vänster
		//motor_left = 70;
		//motor_right = 60;
		//PORTB &= ~(1 << motor_dir_right);								// Set motor direction to forward
		//PORTB |= (1 << motor_dir_left);									// Set motor direction to backward
//	}

	if((regulator_mode == 1) && (regler_ready_linje == 1)){
		left=180;				// kanske räcker med en variable för båda om båda kör exakt lika fort
		right=170;
		motor_left=left;
		motor_right=right;
		linje_main();
		regler_ready_linje = 0;
	}

	else if(regulator_mode == 0){	
		
		PORTB &= ~(1 << motor_dir_right);								// Set motor direction to forward
		PORTB &= ~(1 << motor_dir_left);	

		if(updaterad_labyrint == true && turn_delay >= 10){
		
			updaterad_labyrint = false;
			labyreg();//kalla på labyrint funktion
		}
	}
}