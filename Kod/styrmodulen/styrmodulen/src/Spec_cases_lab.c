#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include "asf.h"

int sensor_front=15;
bool start_turn=false;
bool ongoing_turn=false;
bool turn_done=false;
bool ongoing_function=false;
int RGB_sensor=0;
int sensor_front_spec=0;

void req_gyro_turn(void){
	;
}

void turn_left(){
	PORTB &= ~(1 << motor_dir_right);
	PORTB |= (1 << motor_dir_left);
	motor_left = left;
	motor_right = right;
}

void turn_right(){
	PORTB |= (1 << motor_dir_right);
	PORTB &= ~(1 << motor_dir_left);
	motor_left = left;
	motor_right = right;
}


void drive_forward(void){
	PORTB &= ~(1 << motor_dir_left);
	PORTB &= ~(1 << motor_dir_right);
	motor_left = left;
	motor_right = right;
	
}
// Snabb dator LOL!

void waypoint_lab(void){
	
	if(!ongoing_turn && sensor_front > 15){    //---k�r fram i korsningen f�r att prepa sv�ng
		drive_forward();
		turn_done=false;
		
	}
	else if(!ongoing_turn && sensor_front <= 15){			// redo att p�b�rja sv�ng
		ongoing_turn=true;
		start_turn=true;

	}
	else if(start_turn){					//startar sv�ngen, kollar RGB, om v�nster gira v�nster annars gira h�ger(med eller utan rgb inkikation).
		req_gyro_turn();
		if(RGB_sensor==1){					//RGB_data="bl�"
			turn_left();
		}
		else {
			turn_right();					//RGB_data="r�d"
		}
		
		start_turn=false;
	}
	else if(ongoing_turn && gyro_turn==1){
		turn_done=true;
		count_waypoint=0;
		gyro_turn=0;
		drive_forward();
	}
	else if(turn_done && count_waypoint >= 5){
		waypoint = 0;						//sv�ng �r klar, �terg� till reglering
		ongoing_turn = false;
	}


}



void oneway_turn_lab(void){//ska den heta _lab eller inte
	
	if(!ongoing_turn && sensor_front > 15){    //---k�r fram i korsningen f�r att prepa sv�ng
		drive_forward();
		turn_done=false;	
	}
		
	else if(!ongoing_turn && sensor_front <= 15){			// redo att p�b�rja sv�ng
		ongoing_turn=true;
		start_turn=true;
		req_gyro_turn();
	}
	
	else if(start_turn){
		if(sensor_left > 57){
			turn_left();
		}
		else if(sensor_right > 57){
			turn_right();
		}
		start_turn=false;	
	}
	
	else if(ongoing_turn && gyro_turn==1){
		turn_done=true;
		count_waypoint=0;
		gyro_turn=0;
		drive_forward();
	}
	
	else if(turn_done && count_waypoint >= 5){
		oneway_turn = 0;						//sv�ng �r klar, �terg� till reglering
		ongoing_turn = false;
	}
}

void two_way_turn(void){
	if(!ongoing_function){    //---k�r fram i korsningen f�r att prepa sv�ng
		drive_forward();
		turn_done=false;
		sensor_front_spec= sensor_front;
	}
	if(!ongoing_function && sensor_front+15 >= sensor_front_spec);
		
	
}