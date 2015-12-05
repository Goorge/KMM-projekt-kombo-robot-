#include <avr/interrupt.h>
#include <avr/io.h>
#include "asf.h"

int setpoint=0; // refpunkten i mitten
float current_error=0;
float derivate=0;
int output=0; //vad motorenrna ska regleras med
float previous_error = 0;
int p_constant=110;  
int d_constant=20; 
float current_position = 0;
int scaler=1;
int dt = 1;			// 100ms loop time (kanske)


	 
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
	derivate = (current_error -previous_error) * dt;					// Tror det var dt som saknades för derivatan
	output = (p_constant * current_error + d_constant * derivate) / scaler;
	previous_error = current_error;
//***********************************
//Reglering
//***********************************		
	if (fel_antal==0){ //om utanför linje behöver kanske en tidsbegränsning 
		PORTB |= (1 << motor_dir_left); //baka tillbaka till linjen för hoppnngsvis
		PORTB |= (1 << motor_dir_right);
		motor_left = left/2;
		motor_right = right/2;
	}
	else if(output < 0){									// Utsignalen är negativ, beror på derivatan bl.a
		if(abs(output) >= right){				// För att unvika mättnad i regleringen
			PORTB |= (1 << motor_dir_left);		// Set motor direction to backward
			PORTB &= ~(1 << motor_dir_right);		// Set motor direction to forward
			motor_left = left;
			motor_right = abs(output)- right;
		}
		else {
			PORTB &= ~(1 << motor_dir_left);		// Set motor direction to forward
			PORTB &= ~(1 << motor_dir_right);		// Set motor direction to forward
			motor_left = left;
			motor_right = right + output;
		}
	}
	else if(output > 0){							// Utsignalen är posetiv, beror på derivatan bl.a
		if(output >= left){				// För att unvika mättnad i regleringen
			PORTB |= (1 << motor_dir_right);		// Set motor direction to backward
			PORTB &= ~(1 << motor_dir_left);		// Set motor direction to forward
			motor_right = right;
			motor_left = output - left;
		}
		else {
			PORTB &= ~(1 << motor_dir_left);		// Set motor direction to forward
			PORTB &= ~(1 << motor_dir_right);		// Set motor direction to forward
			motor_right = right;
			motor_left = left - output;
		}
	}
	else if(output==0){
		PORTB &= ~(1 << motor_dir_left);		// Set motor direction to forward
		PORTB &= ~(1 << motor_dir_right);		// Set motor direction to forward
		motor_left = left;																			 //Om nu detta funkar så när den hoppar mellan 0 och +/-1 kommer den köra rakt, kan nog bli lite / \ på linjen men typish rakt :D
		motor_right = right;
	}
	//PORTD ^= (1 << PD1); // heej
}

