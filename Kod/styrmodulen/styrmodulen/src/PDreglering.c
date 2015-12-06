#include <avr/interrupt.h>
#include <avr/io.h>
#include "asf.h"

void regulator(){
	if(turning){ // Spinn vänster
		PORTB &= ~(1 << motor_dir_right);								// Set motor direction to forward
		PORTB |= (1 << motor_dir_left);									// Set motor direction to backward
		motor_left = left;
		motor_right = right;
	}
	else if(regulator_mode == 1){
		linje_main();
	}
	else
	{	
		PORTB &= ~(1 << motor_dir_right);								// Set motor direction to forward
		PORTB &= ~(1 << motor_dir_left);	
		if(updaterad_labyrint){
			updaterad_labyrint = false;
			labyreg();//kalla på labyrint funktion
		}
	}
}