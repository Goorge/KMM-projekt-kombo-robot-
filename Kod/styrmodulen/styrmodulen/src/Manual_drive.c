#include <avr/interrupt.h>
#include <avr/io.h>
#include "asf.h"
int manual_function=0;

void manual_drive(){
	
		if(manual_function == 0){											// Stand still = dont dare to move
			PORTB &= ~(1 << motor_dir_left);								// Set motor direction to forward
			PORTB &= ~(1 << motor_dir_right);								// Set motor direction to forward
			motor_left = 0;
			motor_right = 0;
		}
		else if(manual_function == 1){										// Forward
			PORTB &= ~(1 << motor_dir_left);								// Set motor direction to forward
			PORTB &= ~(1 << motor_dir_right);								// Set motor direction to forward
			motor_left = left;
			motor_right = right;
		}
		else if(manual_function == 2){										// Backward
			PORTB |= (1 << motor_dir_left) | (1 << motor_dir_right);		// Set motor direction to backward
			motor_left = left-13;											// -n for adjusting so the motors "speed" are the same :D :D  ***Change depends on speed :D***
			motor_right = right;
		}
		else if(manual_function == 3){										// Rightforward
			PORTB &= ~(1 << motor_dir_left);								// Set motor direction to forward
			PORTB &= ~(1 << motor_dir_right);								// Set motor direction to forward
			motor_left = left;
			motor_right = (right - (right/3));								// Speed value minus one third of the speedvalue
		}
		else if(manual_function == 4){										// Leftforward
			PORTB &= ~(1 << motor_dir_left);								// Set motor direction to forward
			PORTB &= ~(1 << motor_dir_right);								// Set motor direction to forward
			motor_left = (left - (left/3));									// Speed value minus one third of the speedvalue
			motor_right = right;
		}
		else if(manual_function == 5){										// Spin right
			PORTB &= ~(1 << motor_dir_left);								// Set motor direction to forward
			PORTB |= (1 << motor_dir_right);								// Set motor direction to backward
			motor_left = left;
			motor_right = right;
		}
		else if(manual_function == 6){										// Spin left
			PORTB &= ~(1 << motor_dir_right);								// Set motor direction to forward
			PORTB |= (1 << motor_dir_left);									// Set motor direction to backward
			motor_left = left;
			motor_right = right;
		}
		else{
			manual_function=0;												// why not :D 
		}
	
	
}