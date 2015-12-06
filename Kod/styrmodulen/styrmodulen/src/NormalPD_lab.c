#include <avr/interrupt.h>
#include <avr/io.h>
#include "asf.h"

int previous_error_lab_right = 0;
int previous_error_lab_left = 0;
int p_constant_lab = 60;
int d_constant_lab = 10;
int current_error_lab;
int output_right;
int output_left;
float derivate;
int distance_wall_desired = 20;
int number_of_errors = 3;
int previous_errors_left[3];
int previous_errors_right[3];
int error_count = 0;

void PD_for_lab(int distance_left, int distance_right, int distance_front){
	//räkna ut fel höger
	current_error_lab = distance_wall_desired - distance_right; //-sensor_left_tmp
	derivate = current_error_lab - previous_errors_right[error_count];///10; //
	output_right = (p_constant_lab * current_error_lab + d_constant_lab * derivate)/10;
	previous_errors_right[error_count] = current_error_lab;

	//räkna ut fel vänster
	current_error_lab = distance_wall_desired - distance_left; //-sensor_left_tmp
	derivate = (current_error_lab - previous_errors_left[error_count]);///10; //
	output_left = (p_constant_lab * current_error_lab + d_constant_lab * derivate)/10;
	previous_errors_left[error_count] = current_error_lab;

	
	
	
	if(++error_count >= number_of_errors)
		error_count=0;
		
	if(output_left >= 0)
		output_left = 0;
	
	else if(output_left <= (-motor_left))
		output_left = -motor_left;
	
	if(output_right >= 0)
		output_right = 0;
		
	else if(output_right <= (-motor_right))
		output_right = -motor_right;

	if(distance_front <= 31){
		motor_left = 0; 
		motor_right = 0;
		byte data[1] = { 0x07 };
		i2c_send(0x06, data);
		turning = true;
		_delay_ms(100);
		return;
	}
	
	if(previous_errors_left[error_count] <= -10){ // testgrej
		output_right = -motor_right;
		output_left = -motor_left;
	}

	motor_left = left + output_left;
	motor_right = right + output_right;
	
	/*if(output_tmp > 0){
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
	}*/
}