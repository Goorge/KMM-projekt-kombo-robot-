#include <avr/interrupt.h>
#include <avr/io.h>
#include "asf.h"

int sensor_left;
int sensor_right;
int previous_error_lab= 0;
int p_constant_lab =2;
int d_constant_lab =1;
int current_error_lab;
int output_tmp;
int derivate;
int arre_lab[] = {  15, 12, 9, 6, 3,
                 10, 10,  15,  15, 15,
                 0,  0,  0,  0, 0,
                -15,  -12,  -9, -6, -3, 
                 -10,  -10, -15,  -15, -15,
                 0,  0,  0,  0, 0 };
                 /*
                 {  -5, -3, -2, -1, 0,
                     2, -1,  1,  0, 0,
                     0,  3,  2,  1, 0,
                     -1,  0,  5,  3, 2,
                     1,  0, -1,  0, 2,
                 1,  0,  2,  1, 0 };
                 */
                  

void labyreg(){
	cli();
	int sensor_right_tmp = sensor_right;
	int sensor_left_tmp = sensor_left;
	sei();
	
		current_error_lab = sensor_right_tmp - sensor_left_tmp;
		//------------reglering
		//current_error_lab=arre_lab[count_arre];
		derivate = (current_error_lab - previous_error_lab);///10; //
		output_tmp = p_constant_lab * current_error_lab + d_constant_lab * derivate;
		previous_error_lab = current_error_lab;
	
		if(output_tmp >= motor_left/2)
			output_tmp=motor_left/2;
		if(output_tmp <= (-motor_right)/2)
			output_tmp = (-motor_right)/2;	
		if(output_tmp > 0){
			motor_left = left-output_tmp;
			motor_right = right;	
		}
	
		else if(output_tmp < 0){
			motor_left = left;
			motor_right = right+output_tmp;	
		}
	
		else if(output_tmp ==0)
		{		
			motor_left=left;      // Initiate speed=0 on both engines
			motor_right=right;

	}
}