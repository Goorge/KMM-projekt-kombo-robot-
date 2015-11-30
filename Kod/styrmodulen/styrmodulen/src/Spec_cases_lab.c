#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include "asf.h"

int sensor_front=15;
bool start_turn=false;
bool ongoing_turn=false;
bool turn_done=false;
int RGB_sensor=0;
int sensor_front_tmp; //anv�nds d� man kommer till ett v�gsk�l d�r man kan sv�nga eller k�ra rakt fram f�r att kunna �ka ut i mitte av kurvan innan sv�n inleds

void req_gyro_turn(void){ // fuktion man kallar p� f�r att starta Gyro inf�r 90 graders sv�ng
	;
}
//hej
//-----------------------------------------
//Funktioner f�r att styra robot f�r h�ger/v�nster eller fram�t
//-----------------------------------------
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
//-------------------------------------
//Funktioner som kommer styra roboten vid specialfall som tillexempel t-korsning eller en vanlig sv�ng
//-------------------------------------

void waypoint_lab(void){ //t-korsning 
	
	if(!ongoing_turn && sensor_front > 15){    //---k�r fram i korsningen f�r att prepa sv�ng
		PORTB &= (0 << motor_dir_left) | (0 << motor_dir_right);
		motor_left = left;
		motor_right = right;
		turn_done=false;
		
	}
	else if(!ongoing_turn && sensor_front <= 15){			// redo att p�b�rja sv�ng
		ongoing_turn=true;
		start_turn=true;
		req_gyro_turn();
	}
	else if(start_turn){					//startar sv�ngen, kollar RGB, om v�nster gira v�nster annars gira h�ger(med eller utan rgb indikation).
		
		if(RGB_sensor==1){					//RGB_data="bl�" allts� sv�ng v�nster
			turn_left();
		}
		else {
			turn_right();					//RGB_data="r�d" allts� sv�ng h�ger
		}
		
		start_turn=false;					//S�ger att sv�ngen har inledits f�r att inte g� in i den h�r if-satsen igen
	}
	else if(ongoing_turn && gyro_turn==1){	//Gyrot singnalerar att sv�ngen �r klar och det �r klart att b�rja k�ra ur den
		turn_done=true;
		count_waypoint=0;
		gyro_turn=0;
		drive_forward();
	}
	else if(turn_done && count_waypoint >= 5){	//sv�ng �r klar och roboten har k�rt ur kurvan, �terg� till vanlig PD-reglering
		waypoint = 0;						
		ongoing_turn = false;
	}


}


//ska den heta _lab eller inte
void oneway_turn_lab(void){//roboten kommer fram till en enkel kurva antingen till h�ger eller v�nster
							
	
	if(!ongoing_turn && sensor_front > 15){    //---k�r fram i korsningen f�r att prepa sv�ng
		drive_forward();
		turn_done=false;
		
	}
	else if(!ongoing_turn && sensor_front <= 15){			// redo att p�b�rja sv�ng
		ongoing_turn=true;
		start_turn=true;
		req_gyro_turn();
	}
	else if(start_turn){									//Roboten �r inne i kurvan och redo o sv�nga 
		if(sensor_left > 57){								//Kollar avst�nd till h�ger och v�nster f�r att -
															//se vilket h�ll det ska sv�ngas och inleder sedan sv�ng
			turn_left();
		}
		else if(sensor_right > 57){
			turn_right();
		}
		start_turn=0;										// Signalerar att sv�ng har	inledits f�r att inte g� in i denna if igen
		
	}
	
	else if(ongoing_turn && gyro_turn==1){					//Gyrot singnalerar att sv�ngen �r klar och det �r klart att b�rja k�ra ur den
		turn_done=true;
		count_waypoint=0; //oneway_turn
		gyro_turn=0;
		drive_forward();
	}
	
	else if(turn_done && count_waypoint >= 5){	//sv�ng �r klar och roboten har k�rt ur kurvan, �terg� till vanlig PD-reglering
		oneway_turn= 0;
		ongoing_turn = false;
	}
	
}



void twoway_turn_lab(void){// roboten kommer fram till ett v�gsk�l d�r den antingen kan sv�nga �t h�ger/v�nster eller k�ra raktfram
	if(!ongoing_turn ){    //---k�r fram i korsningen f�r att prepa sv�ng
		sensor_front_tmp=sensor_front;
		drive_forward();
		turn_done=false;
	}
}