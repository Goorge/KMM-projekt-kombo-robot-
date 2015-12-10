#include <avr/interrupt.h>
#include <avr/io.h>
#include "asf.h"
int manual_function=0;
int gyro_left = 0;
int gyro_right = 0;

void req_gyro_turn(void){ // fuktion man kallar på för att starta Gyro inför 90 graders sväng
	byte data[1] = { 0x07 };
	i2c_send(0x06, data);
	turning = true;
	_delay_ms(100);
}


void turn_left(void){
	PORTB &= ~(1 << motor_dir_right);
	PORTB |= (1 << motor_dir_left);
	motor_left = 60;//left;
	motor_right = 55;//right;
}

void turn_right(void){
	PORTB |= (1 << motor_dir_right);
	PORTB &= ~(1 << motor_dir_left);
	motor_left = 57;//left;
	motor_right = 52;//right;
}

//--------------------------------------------------------------
//Funktioner för att styra robot för höger/vänster eller framåt
//--------------------------------------------------------------

void manual_drive(){
		//******************************* Om vänster eller höger-sväng har startats så kör vi klart den, skippa andra inkommandon 
		if(gyro_left == 1){
			manual_function = 6;
		}
		else if(gyro_right == 1){
			manual_function = 5;	
		}
		//*******************************
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
			if(gyro_right == 0){											// Only once when pressed
				req_gyro_turn();											// Request gyroturn
				turn_right();												// Turn motors on for left spin	
				gyro_right = 1; 
			}
			else if(gyro_right == 1){				// 90 turn done
				manual_function = 0;										// kan behövas nollställning av funktion (probably not)
				gyro_right = 0;												
			}
		}
		else if(manual_function == 6){										// Spin left
			if(gyro_left == 0){												// Only once when pressed
				req_gyro_turn();											// Request gyroturn
				turn_left();												// Turn motors on for left spin						
				gyro_left = 1;												
			}
			else if(gyro_left == 1){					// 90 turn done
				manual_function = 0;										//kanske behövs nollställa funktion  (probably not monsieur)
				gyro_left = 0;
			}
		}
		else{
			manual_function=0;												// why not :D 
		}
}