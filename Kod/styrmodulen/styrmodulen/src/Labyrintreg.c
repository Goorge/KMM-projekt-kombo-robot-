#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include "asf.h"

bool waypoint = false;
bool oneway_turn = false;
bool twoway_turn_left = false;
bool twoway_turn_right = false;


int sensor_front;
int sensor_left;
int sensor_right;
int tejp_counter = 0;
                 
void labyreg(){
	cli();
	int sensor_front_tmp = distans_fram;
	int sensor_right_tmp = distans_right;
	int sensor_left_tmp = distans_left;
	sei();
	int st_value = 40;
	int st_value_front = 40;
	/*if(true)      //testplats f�r reglering
		drive_forward_right(sensor_right_tmp, sensor_front_tmp);
	
	else*/ 
	if(Reflex_data != 0 || Reflex_data2 != 0){
		tejp_counter++;
		if(tejp_counter >= 3)
			regulator_mode = 1;
	}
	
	else if(Reflex_data == 0 && Reflex_data2 == 0)
		tejp_counter = 0;

	if(waypoint){
		waypoint_lab(sensor_left_tmp,sensor_right_tmp,sensor_front_tmp,st_value);
		return;	
	}
	else if(oneway_turn){
		oneway_turn_lab(sensor_left_tmp,sensor_right_tmp,sensor_front_tmp,st_value);
		return;
	}
	else if(twoway_turn_left){
		twoway_turn_left_lab(sensor_left_tmp,sensor_right_tmp,sensor_front_tmp,st_value);
		return;
	}
	else if(twoway_turn_right){
		twoway_turn_right_lab(sensor_left_tmp,sensor_right_tmp,sensor_front_tmp,st_value);
		return;
	}	
	
	else if(count_waypoint < 4){
		count_waypoint++;
		if(sensor_front_tmp <= 35)
			count_waypoint = 4;
		_delay_us(10);
	}
	//�terv�ndsgr�nd
	else if(sensor_left_tmp < st_value && sensor_front_tmp <= 35 && sensor_right_tmp < st_value){
		req_gyro_turn();
		turn_right();//	output_right = -right;//motor_right = 0;
		PORTD ^= (1 << PD0);
		first_time_flag = true;
	}
	// Normalfall
	else if(sensor_left_tmp < st_value/* && sensor_front_tmp > 30*/ && sensor_right_tmp < st_value){
		PD_for_lab(sensor_left_tmp, sensor_right_tmp, sensor_front_tmp);
	}
	
	// f�rbered specialfall
	else if((sensor_left_tmp >= st_value || sensor_right_tmp >= st_value) && !prepare_special_case /*sensor_front_tmp >= st_value_front*/){
		prepare_special_case = true;
		count_waypoint = 0;
		if(!sensor_front_tmp <= 35)
			drive_forward();
		first_time_flag = true;
	}
	
	//Enkelsv�ng	
	else if(((sensor_left_tmp < st_value && sensor_front_tmp < st_value_front && sensor_right_tmp > st_value) 
		|| (sensor_left_tmp > st_value && sensor_front_tmp < st_value_front && sensor_right_tmp < st_value)))	{
		oneway_turn_lab(sensor_left_tmp,sensor_right_tmp,sensor_front_tmp,st_value);
		oneway_turn = true;
		first_time_flag = true;
	}
	
	//t-korsning rakt framifr�n
	else if((sensor_left_tmp > st_value && sensor_front_tmp < st_value_front && sensor_right_tmp > st_value))	{	
		waypoint_lab(sensor_left_tmp,sensor_right_tmp,sensor_front_tmp,st_value);
		waypoint = true;
		//PORTD |= (1 << PD1);
		first_time_flag = true;
	}
	
	// T-korsning d�r roboten kan k�ra fram�t eller �t v�nster
	else if(sensor_left_tmp > st_value && sensor_front_tmp > st_value_front && sensor_right_tmp < st_value){
		twoway_turn_left = true;
		twoway_turn_left_lab(sensor_left_tmp,sensor_right_tmp,sensor_front_tmp,st_value);
		first_time_flag = true;
	}
	
	// T-korsning d�r roboten kan k�ra fram�t eller �t h�ger	
	else if(sensor_left_tmp < st_value && sensor_front_tmp > st_value_front && sensor_right_tmp > st_value){
		twoway_turn_right_lab(sensor_left_tmp,sensor_right_tmp,sensor_front_tmp,st_value);
		twoway_turn_right = true;
		first_time_flag = true;
	}
	else{
		stand_still();
		//req_gyro_turn();
		//turn_left();
		//motor_left = 0;
		//motor_right = 0;
		//PORTD ^= (1 << PD1);
		//stand_still();
	}
}




/*current_error_lab = distance_wall_right - sensor_right_tmp; //-sensor_left_tmp
		//------------reglering
		//current_error_lab=arre_lab[count_arre];
		derivate = (current_error_lab - previous_error_lab);///10; //
		output_tmp = p_constant_lab * current_error_lab + d_constant_lab * derivate;
		previous_error_lab = current_error_lab;
	
		if(output_tmp >= motor_left*3/4)
			output_tmp=motor_left*3/4;
		
		if(output_tmp <= (-motor_right)*3/4)
			output_tmp = (-motor_right)*3/4;
				
		if(output_tmp > 0){
			motor_left = left - output_tmp;
			motor_right = right;	
		}
	
		else if(output_tmp < 0){
			motor_left = left;
			motor_right = right + output_tmp;	
		}
	
		else if(output_tmp ==0)
		{		
			motor_left=left;      
			motor_right=right;
	}*/
