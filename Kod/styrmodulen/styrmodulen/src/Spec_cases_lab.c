#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include "asf.h"

int sensor_front=15;
bool start_turn=false;
bool ongoing_turn=false;
bool turn_done=false;
int RGB_sensor=0;


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
	PORTB &= ~(1 << motor_dir_left) | (1 << motor_dir_right);
	motor_left = left;
	motor_right = right;
	
}
// Snabb dator LOL!

void waypoint_lab(void){
	
	if(!ongoing_turn && sensor_front > 15){    //---kör fram i korsningen för att prepa sväng
		PORTB &= (0 << motor_dir_left) | (0 << motor_dir_right);
		motor_left = left;
		motor_right = right;
		turn_done=false;
		
	}
	else if(!ongoing_turn && sensor_front <= 15){			// redo att påbörja sväng
		ongoing_turn=true;
		start_turn=true;
		req_gyro_turn();
	}
	else if(start_turn){					//startar svängen, kollar RGB, om vänster gira vänster annars gira höger(med eller utan rgb inkikation).
		
		if(RGB_sensor==1){					//RGB_data="blå"
			turn_left();
		}
		else {
			turn_right();					//RGB_data="röd"
		}
		
		start_turn=false;
	}
	else if(ongoing_turn && gyro_turn==1){
		turn_done=true;
		count_waypoint=0;
		gyro_turn=0;
		PORTB &= (0 << motor_dir_left) | (0 << motor_dir_right);
		motor_left = left;
		motor_right = right;
	}
	else if(turn_done && count_waypoint >= 5){
		waypoint = 0;						//sväng är klar, återgå till reglering
		ongoing_turn = false;
	}


}



void oneway_turn_lab(void){//ska den heta _lab eller inte
	
	
	if(!ongoing_turn && sensor_front > 15){    //---kör fram i korsningen för att prepa sväng
		PORTB &= (0 << motor_dir_left) | (0 << motor_dir_right);
		motor_left = left;
		motor_right = right;
		turn_done=false;
		
	}
	else if(!ongoing_turn && sensor_front <= 15){			// redo att påbörja sväng
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
		start_turn=0;
		
	}
	
	
	
}