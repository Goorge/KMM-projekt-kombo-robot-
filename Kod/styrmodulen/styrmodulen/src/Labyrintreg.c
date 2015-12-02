#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include "asf.h"
bool waypoint=false;
bool oneway_turn=false;
bool twoway_turn_left=false;
bool twoway_turn_right=false;
int sensor_front;
int sensor_left;
int sensor_right;

                 
void labyreg(){
	cli();
	int sensor_front_tmp = sensor_front;
	int sensor_right_tmp = sensor_right;
	int sensor_left_tmp = sensor_left;
	sei();
	
	int st_value = 40;
	// Normalfall
	/*if((sensor_left_tmp < st_value && sensor_front_tmp > st_value && sensor_right_tmp < st_value) &&
	(!oneway_turn && !waypoint && !twoway_turn_left && !twoway_turn_right))*/
		PD_for_lab(sensor_left_tmp,sensor_right_tmp);
	//Enkelsväng	
	/*else if(((sensor_left_tmp < st_value && sensor_front_tmp < st_value && sensor_right_tmp > st_value) 
		|| (sensor_left_tmp > st_value && sensor_front_tmp < st_value && sensor_right_tmp < st_value)) || oneway_turn)	
		oneway_turn_lab(sensor_left_tmp,sensor_right_tmp,sensor_front_tmp,st_value);
	//t-korsning rakt framifrån
	else if((sensor_left_tmp > st_value && sensor_front_tmp < st_value && sensor_right_tmp > st_value) || waypoint)		
		waypoint_lab(sensor_left_tmp,sensor_right_tmp,sensor_front_tmp,st_value);
	// T-korsning där roboten kan köra framåt eller åt vänster
	else if((sensor_left_tmp > st_value && sensor_front_tmp > st_value && sensor_right_tmp < st_value) || twoway_turn_left)
		twoway_turn_left_lab(sensor_left_tmp,sensor_right_tmp,sensor_front_tmp,st_value);
	// T-korsning där roboten kan köra framåt eller åt höger	
	else if((sensor_left_tmp < st_value && sensor_front_tmp > st_value && sensor_right_tmp > st_value) || twoway_turn_right)
		twoway_turn_right_lab(sensor_left_tmp,sensor_right_tmp,sensor_front_tmp,st_value);*/
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
