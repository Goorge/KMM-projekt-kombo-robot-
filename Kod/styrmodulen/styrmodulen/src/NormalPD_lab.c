#include <avr/interrupt.h>
#include <avr/io.h>
#include "asf.h"

int previous_error_lab_right = 0;
int previous_error_lab_left = 0;
int p_constant_lab = 14;
int d_constant_lab = 210;
int current_error_lab;
int output_right;
int output_left;
float derivate;
int distance_wall_desired = 18;
int number_of_errors = 2;
int previous_errors_left[10];
int previous_errors_right[10];
int error_count = 0;

bool first_time_flag = false;

void PD_for_lab(int distance_left, int distance_right, int distance_front){
	//räkna ut fel höger
	current_error_lab = distance_left - distance_right; //-sensor_left_tmp
	
	if (first_time_flag)
	{
		for(int i = 0; i < number_of_errors; i++)
			previous_errors_right[i] = current_error_lab;
	}
	
	first_time_flag = false;
	
	derivate = current_error_lab - previous_errors_right[error_count];///10; //
	/*if (derivate < 3 && derivate > -3)
		derivate = 0;*/
	output_right = (p_constant_lab * current_error_lab + d_constant_lab * derivate)/10;
	previous_errors_right[error_count] = current_error_lab;
	
	if(output_right >= 0){
		motor_right = laby_right_speed;
		if(output_right > laby_left_speed)
			output_right = laby_left_speed;
		motor_left = laby_left_speed - output_right;	
	}
	else if(output_right < 0){
		motor_left = laby_left_speed;
		if(output_right < -laby_right_speed)
			output_right = -laby_right_speed;
		motor_right = laby_right_speed + output_right;	
	}
		
	//else if(output_right <= (-right))
	//	output_right = -right;
	
	//if(++error_count >= number_of_errors)
	//	error_count=0;
		
	/*
	//räkna ut fel höger
	current_error_lab = distance_wall_desired - distance_right; //-sensor_left_tmp
	derivate = current_error_lab - previous_errors_right[error_count];///10; //
	//if(derivate < 3)
	//	derivate = 0;
	output_right = (p_constant_lab * current_error_lab + d_constant_lab * derivate)/10;
	previous_errors_right[error_count] = current_error_lab;

	//räkna ut fel vänster
	current_error_lab = distance_wall_desired - distance_left; //-sensor_left_tmp
	derivate = (current_error_lab - previous_errors_left[error_count]);//10; //
	//if(derivate < 3)
	//	derivate = 0;
	output_left = (p_constant_lab * current_error_lab + d_constant_lab * derivate)/10;
	previous_errors_left[error_count] = current_error_lab;

	if(++error_count >= number_of_errors)
		error_count=0;
		
	if(output_left >= 0)
		output_left = 0;
	
	else if(output_left <= (-left))
		output_left = -left;
	
	if(output_right >= 0)
		output_right = 0;
		
	else if(output_right <= (-right))
		output_right = -right;
		
	*/
	/*
		if(distance_front <= 50 && current_error_lab > 6 ){
			turn_left();
			_delay_ms(95);
			stand_still();
			for(int i = 0; i < number_of_errors ; i++){
				previous_errors_right[i] = 0;
				
			}
			
		}
		else if (distance_front <= 50 && current_error_lab < -6 ){
			//turn_right;
			PORTD ^= (1 << PD0);
			PORTB |= (1 << motor_dir_right);
			PORTB &= ~(1 << motor_dir_left);
			motor_left = 57;//80;//57left;
			motor_right = 52;//52right;
			_delay_ms(95);
			stand_still();
			for(int i = 0; i < number_of_errors ; i++){
				previous_errors_right[i] = 0;
		}
		}*/
	
	/*if(previous_errors_left[er								<  ror_count] <= -10){ // testgrej
		output_right = -motor_right;
		output_left = -motor_left;
	}*/
	//if(error_count == 0)
	//	PORTD ^= (1 << PD0);
	
	//motor_left = left + output_left;
	//motor_right = right + output_right;
}