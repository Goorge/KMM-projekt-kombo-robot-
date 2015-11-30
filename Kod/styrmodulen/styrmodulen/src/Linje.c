#include <avr/interrupt.h>
#include <avr/io.h>
#include "asf.h"

int setpoint=0; // refpunkten i mitten
int current_error=0;
int derivate=0;
int output=0;
int previous_error=0;
int p_constant=70;  //satte lite random värden för att få större output
int d_constant=10 ; // samma som ovan
int current_position=0;
		 
void linje(void){
	cli();
	int current_position_tmp = current_position;
	sei();

	current_error = setpoint - current_position_tmp;
	derivate = (current_error -previous_error);///10; //
	output = p_constant*current_error+d_constant*derivate;
	previous_error= current_error;
//***********************************
//Reglering för vänstersväng  ,sensorer vänster om mitten har tänts (-5 till -1)
//***********************************	
	//if(current_position_tmp < 0){
		if(output>(left*3/4))
			output=left*3/4;
		else if((output)<(right*3/4))
			output=-right*3/4;
		if(output < 0){							// Utsignalen är negativ, beror på derivatan bl.a
			motor_left = left + output;
			motor_right = right;
		}
		if(output > 0){							// Utsignalen är posetiv, beror på derivatan bl.a
			motor_right = right - output;
			motor_left = left;
		}	
	//}
//***********************************
//Reglering för högersväng  ,sensorer höger om mitten har tänts (5 till 1)
//***********************************
	/*if(current_position_tmp > 0){
		if(output < 0){							// Utsignalen är negativ, beror på derivatan bl.a
			motor_right = right + output*2;
			motor_left = left;
		}
		if(output > 0){							// Utsignalen är posetiv, beror på derivatan bl.a
			motor_left = left - output*2;
			motor_right = right;
		}	
	}*/
//***********************************
//Om vi inte har något fel, åk rakt. Båda motorerna max
//***********************************
	if(output == 0){
		motor_left = left;
		motor_right = right;
	}
		
}

