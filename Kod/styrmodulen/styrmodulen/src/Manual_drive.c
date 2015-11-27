#include <avr/interrupt.h>
#include <avr/io.h>
#include "asf.h"
int manual_function=0;

void manual_drive(){
	if(counter_timer < 5){
		if(manual_function == 0){              //stand still
			PORTB = (0 << motor_dir_left) | (0 << motor_dir_right);
			motor_left = 0;
			motor_right = 0;
		}
		else if(manual_function == 1){              //forward
			PORTB &= (0 << motor_dir_left) | (0 << motor_dir_right);
			motor_left = left;
			motor_right = right;
		}
		else if(manual_function == 2){          //backward
			PORTB |= (1 << motor_dir_left) | (1 << motor_dir_right);
			motor_left = left-13;
			motor_right = right;
		}
		else if(manual_function == 3){          //rightforward
			PORTB &= (0 << motor_dir_left) | (0 << motor_dir_right);
			motor_left = left;
			motor_right = right;
		}
		else if(manual_function == 4){          //leftforward
			PORTB |= (0 << motor_dir_left) | (0 << motor_dir_right);
			motor_left = left;
			motor_right = right;
		}
		else if(manual_function == 5){          //spin right
			PORTB &= (0 << motor_dir_left);
			PORTB |= (1 << motor_dir_right);
			motor_left = left;
			motor_right = right;
		}
		else if(manual_function == 6){          //spin left
			PORTB &= ~(1 << motor_dir_right);
			PORTB |= (1 << motor_dir_left);
			motor_left = left;
			motor_right = right;
		}
		else{
			manual_function=0;
		}
	}
	else{
		PORTB = (0 << motor_dir_left) | (0 << motor_dir_right);
		motor_left = 0;
		motor_right = 0;
	}
	
}