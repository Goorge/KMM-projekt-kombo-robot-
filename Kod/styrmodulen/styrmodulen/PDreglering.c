#include <avr/interrupt.h>
#include <avr/io.h>
#include "asf.h"


int regler_ready=0;
int current_position=5;
int setpoint=0; // refpunkten i mitten
int current_error=0;
int derivate=0;
int output=0;
int previous_error=0;
int p_constant=3;  //satte lite random värden för att få större output
int d_constant=20; // samma som ovan
//********
int left=80;
int right=80;

				 /*
				 {	-5,	-3, -2, -1, 0,
					 2, -1,  1,  0, 0,
					 0,  3,  2,  1, 0,
					 -1,  0,  5,  3, 2,
					 1,  0, -1,  0, 2,
				 1,  0,  2,  1, 0 };
				 */
				 

void regulator(){
	PORTD ^= 0x02;
	int current_position_tmp = current_position;
	//int setpoint_tmp;  //behövs nog ingen tmp refpunkt
	int current_error_tmp = current_error;
	int derivate_tmp = derivate;
	int output_tmp;
	
	
	current_error_tmp = setpoint - current_position_tmp;
	derivate_tmp = (current_error_tmp -previous_error)/10; //
	output_tmp = p_constant*current_error_tmp+d_constant*derivate_tmp;
	previous_error= current_error_tmp;

//***********************************
//Reglering för vänstersväng  ,sensorer vänster om mitten har tänts (-5 till -1)
//***********************************	
	if(current_position_tmp < 0){
		OCR0A = left + output_tmp*2; 
		if(output_tmp < 0){					// Utsignalen är negativ, beror på derivatan bl.a
			OCR0A = left + output_tmp*2;
		}
		if(output_tmp > 0){					// Utsignalen är posetiv, beror på derivatan bl.a
			OCR0A = left - output_tmp*2;
		}
		right = 40;							// Maxa motsatt motor, här kan man reglera kanske, vi får testa.
		OCR0B = right;    
	}
//***********************************
//Reglering för högersväng  ,sensorer höger om mitten har tänts (5 till 1)
//***********************************
	if(current_position_tmp > 0){
		if(output_tmp < 0){					// Utsignalen är negativ, beror på derivatan bl.a
			OCR0B = right + output_tmp*2;
		}
		if(output_tmp > 0){					// Utsignalen är posetiv, beror på derivatan bl.a
			OCR0B = right - output_tmp*2;
		}
		left = 40;							// Maxa motsatt motor, här kan man reglera kanske, vi får testa.
		OCR0A = left;	
	}
//***********************************
//Om vi inte har något fel, åk rakt. Båda motorerna max
//***********************************
	if(current_position_tmp == 0){
		left = 40;
		right = 40;
		OCR0A = left;
		OCR0B = right;
	}
}


/*int main(void){
	timerinit();
	EIMSK |= (1 << INT1);
	EICRA |= (1 << ISC11);// | (1 << ISC10);
	initengine();
	sei();
	DDRB = 0x78;
	PORTB &=  (0 << PB5) | (0 << PB6);
	left=0;
	right=0;
	OCR0A=left;      // Initiate speed=80 on both engines
	OCR0B=right;
	while(1){

		current_position = arre[count_arre]; // används för att simulera linjeföljning, arrayen ändras i globala
		
		if(regler_ready == 1){
			//regulator();
			regler_ready=0;
		}
		PORTD |= (1 << PD0);
		//TODO:: Please write your application code
	}
}*/