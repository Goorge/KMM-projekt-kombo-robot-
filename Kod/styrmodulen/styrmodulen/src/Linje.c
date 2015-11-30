#include <avr/interrupt.h>
#include <avr/io.h>
#include "asf.h"

int setpoint=0; // refpunkten i mitten
int current_error=0;
int derivate=0;
int output=0;
int previous_error=0;
int p_constant=54;  //satte lite random värden för att få större output
int d_constant=20 ; // samma som ovan
int current_position=0;
int scaler=0;
int output_old=0;

	 
void linje(void){
	//**********Scaler, depends on P and D, check with matlab to get output to be : (output = speedvalue - 20)****
	scaler = 6; //6 is good for the speedvalue of 100, max output could be is +/- 78
	//************************************************************************************************************
	cli();
	int current_position_tmp = current_position;
	sei();

	current_error = setpoint - current_position_tmp;
	derivate = (current_error -previous_error);///10; //
	output = (p_constant*current_error+d_constant*derivate)/scaler;
	previous_error= current_error;
//***********************************
//Reglering
//***********************************	
		/*if(output>(left*3/4))					//fixade denna delen med scaler istället, man får dock kolla på matlab för att hitta max outputen och skala efter den
			output=left*3/4;
		else if((output)<(right*3/4))
			output=-right*3/4;*/
		if(output < 0){							// Utsignalen är negativ, beror på derivatan bl.a
			motor_left = left + output;
			motor_right = right;
		}
		if(output > 0){							// Utsignalen är posetiv, beror på derivatan bl.a
			motor_right = right - output;
			motor_left = left;
		}
		
// test kanske kan funka :D	, testa imon 
		/*if(output < 0){							// Utsignalen är negativ, beror på derivatan bl.a
			if((output > output_old) || (output_old == 0){
				motor_left = left + output;
				motor_right = right;
			}
			else if(output < output_old){
				motor_right = right - output;
				motor_left = left;
			}
		}
		if(output > 0){							// Utsignalen är posetiv, beror på derivatan bl.a
			if((output > output_old) || (output_old == 0){
				motor_right = right - output;
				motor_left = left;
			}
			else if(output < output_old){
				motor_left = left + output;
				motor_right = right;
			}
		}	
	*/
//***********************************
//Om vi inte har något fel, åk rakt. Båda motorerna max
//***********************************
int max=13;
int min=4;
	if((abs(output) < min && abs(output_old) < max)&&(abs(output_old) < min || abs(output) < max)){  //output==0 kommer typ aldrig ske, eftersom man kommer hoppa mellan +/- 1 och 0 och kommer då aldrig nå output==0
		motor_left = left;																			 //Om nu detta funkar så när den hoppar mellan 0 och +/-1 kommer den köra rakt, kan nog bli lite / \ på linjen men typish rakt :D
		motor_right = right;
	}
		output_old = output;
}

