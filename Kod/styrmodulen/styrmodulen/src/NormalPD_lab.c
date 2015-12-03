#include <avr/interrupt.h>
#include <avr/io.h>
#include "asf.h"

int previous_error_lab = 0;
int p_constant_lab = 20;
int d_constant_lab = 6;
int current_error_lab;
int output_tmp;
int derivate;
int distance_wall_right=20;


void PD_for_lab(int distance_left,int distance_right){

	current_error_lab = distance_left - distance_right; //-sensor_left_tmp
	derivate = (current_error_lab - previous_error_lab);///10; //
	output_tmp = p_constant_lab * current_error_lab + d_constant_lab * derivate;
	previous_error_lab = current_error_lab;

	if(output_tmp >= motor_left)
		output_tmp = motor_left;

	if(output_tmp <= (-motor_right))
		output_tmp = (-motor_right);

	if(output_tmp > 0){
		motor_left = left - output_tmp;
		motor_right = right;
	}

	else if(output_tmp < 0){
		motor_left = left;
		motor_right = right + output_tmp;
	}

	else if(output_tmp == 0)
	{
		motor_left = left;
		motor_right = right;
	}
}