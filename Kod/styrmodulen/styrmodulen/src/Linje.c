#include <avr/interrupt.h>
#include <avr/io.h>
#include "asf.h"

int setpoint=0; // refpunkten i mitten
float current_error=0;
float derivate=0;
int output=0; //vad motorenrna ska regleras med
float previous_error = 0;
int p_constant=40;  
int d_constant=180; 
float current_position = 0;



	 
void linje(void){
	cli();
	int current_position_tmp = current_position;
	sei();
	
	current_error = setpoint - current_position_tmp;
	derivate = (current_error -previous_error);					// Tror det var dt som saknades för derivatan
	output = (p_constant * current_error + d_constant * derivate);
	previous_error = current_error;
//***********************************
//Reglering
//***********************************
		
	if (fel_antal==0){ //om utanför linje behöver kanske en tidsbegränsning 
		PORTB |= (1 << motor_dir_left); //baka tillbaka till linjen för hoppnngsvis
		PORTB |= (1 << motor_dir_right);
		motor_left = left/(2 * RGB_slow);
		motor_right = right/(2 * RGB_slow);
	}
	else if(output < 0){							// Utsignalen är negativ, beror på derivatan bl.a
		if(abs(output) >= (right / RGB_slow)){					// För att unvika mättnad i regleringen
			//PORTB |= (1 << motor_dir_left);		// Set motor direction to backward  ****ändrade iomed att vi defineat fel motor
			//PORTB &= ~(1 << motor_dir_right);		// Set motor direction to forward   ****ändrade iomed att vi defineat fel motor
			PORTB |= (1 << motor_dir_right);		// Set motor direction to backward  
			PORTB &= ~(1 << motor_dir_left);		// Set motor direction to forward   
			motor_left = left / RGB_slow;
			motor_right = (abs(output)- right) / RGB_slow;
		}
		else {
			PORTB &= ~(1 << motor_dir_left);		// Set motor direction to forward
			PORTB &= ~(1 << motor_dir_right);		// Set motor direction to forward
			motor_left = left / RGB_slow;
			motor_right = (right + output) / RGB_slow;
		}
	}
	else if(output > 0){							// Utsignalen är posetiv, beror på derivatan bl.a
		if(output >= (left / RGB_slow)){							// För att unvika mättnad i regleringen
			//PORTB |= (1 << motor_dir_right);		// Set motor direction to backward  ****ändrade iomed att vi defineat fel motor
			//PORTB &= ~(1 << motor_dir_left);		// Set motor direction to forward   ****ändrade iomed att vi defineat fel motor
			PORTB |= (1 << motor_dir_left);			// Set motor direction to backward  
			PORTB &= ~(1 << motor_dir_right);		// Set motor direction to forward   
			motor_right = right / RGB_slow;
			motor_left = (output - left) / RGB_slow;
		}
		else {
			PORTB &= ~(1 << motor_dir_left);		// Set motor direction to forward
			PORTB &= ~(1 << motor_dir_right);		// Set motor direction to forward
			motor_right = right / RGB_slow;
			motor_left = (left - output) / RGB_slow;
		}
	}
	else if(output==0){
		PORTB &= ~(1 << motor_dir_left);		// Set motor direction to forward
		PORTB &= ~(1 << motor_dir_right);		// Set motor direction to forward
		motor_left = left / RGB_slow;																			 //Om nu detta funkar så när den hoppar mellan 0 och +/-1 kommer den köra rakt, kan nog bli lite / \ på linjen men typish rakt :D
		motor_right = right / RGB_slow;
	}
}

