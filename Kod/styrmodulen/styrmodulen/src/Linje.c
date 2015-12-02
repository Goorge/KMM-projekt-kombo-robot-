#include <avr/interrupt.h>
#include <avr/io.h>
#include "asf.h"

int setpoint=0; // refpunkten i mitten
int current_error=0;
int derivate=0;
int output=0;
int previous_error=0;
int p_constant=1;  
int d_constant=1 ; 
int current_position=0;
int scaler=0;
int dt = 10;			// 100ms loop time


	 
void linje(void){
	//**********Scaler, depends on P and D, check with matlab to get output to be : (output = speedvalue - 20)****
	scaler = 1; //testa i matlab, nya derivatan med dt ger mycket högre värden
	//************************************************************************************************************
	cli();
	int current_position_tmp = current_position;
	sei();
	if(current_position < 0)
	PORTD ^= (1 << PD0);
	if(current_position > 0)
	PORTD ^= (1 << PD1);
	current_error = setpoint - current_position_tmp;
	derivate = (current_error -previous_error)*dt;					// Tror det var dt som saknades för derivatan
	output = (p_constant*current_error+d_constant*derivate)/scaler;
	previous_error= current_error;
//***********************************
//Reglering
//***********************************		
		if(output < 0){									// Utsignalen är negativ, beror på derivatan bl.a
				if(abs(output) >= right){				// För att unvika mättnad i regleringen
					motor_right=0;					// Sätta något lågt värde men så att den inte stänger av motorn helt...
					motor_left = left;
				}
				else {
					motor_left = left;
					motor_right = right + output;
				}		
		}
		else if(output > 0){							// Utsignalen är posetiv, beror på derivatan bl.a
				if(abs(output) >= left){				// För att unvika mättnad i regleringen
					motor_left=0;
					motor_left = right;
				}
				else {
					motor_right = right;
					motor_left = left - output;
				}
				
		}
		else if(output==0){
			motor_left = left;																			 //Om nu detta funkar så när den hoppar mellan 0 och +/-1 kommer den köra rakt, kan nog bli lite / \ på linjen men typish rakt :D
			motor_right = right;
		}
		//PORTD ^= (1 << PD1); // heej
}

