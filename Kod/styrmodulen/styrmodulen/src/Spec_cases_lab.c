#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include "asf.h"

bool start_turn = false;
bool ongoing_turn = false;
bool turn_done = false;
bool first_time=true;
int sensor_front_tmp; //anv�nds d� man kommer till ett v�gsk�l d�r man kan sv�nga eller k�ra rakt fram f�r att kunna �ka ut i mitte av kurvan innan sv�n inleds
int min_distance_front = 30;
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
	PORTB &= ~(1 << motor_dir_left); 
	PORTB &= ~(1 << motor_dir_right);
	motor_left = left;
	motor_right = right;
	
}

void stand_still(void){
	PORTB &= ~(1 << motor_dir_left); 
	PORTB &= ~(1 << motor_dir_right);
	motor_left = 0;
	motor_right = 0;
	
}
// Snabb dator LOL!
//-------------------------------------
//Funktioner som kommer styra roboten vid specialfall som tillexempel t-korsning eller en vanlig sv�ng
//-------------------------------------

void waypoint_lab(int distance_left,int distance_right,int distance_front,int st_value){ //t-korsning 
	
	if(!ongoing_turn && distance_front > min_distance_front){    //---k�r fram i korsningen f�r att prepa sv�ng
		drive_forward();
		turn_done = false;
		
	}
	else if(!ongoing_turn && distance_front <= min_distance_front){			// redo att p�b�rja sv�ng
		ongoing_turn = true;
		start_turn = true;
		req_gyro_turn();
	}
	else if(start_turn){					//startar sv�ngen, kollar RGB, om v�nster gira v�nster annars gira h�ger(med eller utan rgb indikation).
		
		if(RGB_data == 1){					//RGB_data="bl�" allts� sv�ng v�nster
			turn_left();
		}
		else {
			turn_right();					//RGB_data="r�d" allts� sv�ng h�ger
		}
		
		start_turn = false;					//S�ger att sv�ngen har inledits f�r att inte g� in i den h�r if-satsen igen
	}
	else if(ongoing_turn && gyro_90){	//Gyrot singnalerar att sv�ngen �r klar och det �r klart att b�rja k�ra ur den
		turn_done = true;
		count_waypoint = 0;
		gyro_turn = 0;
		drive_forward();
	}
	else if(turn_done && count_waypoint >= 5){	//sv�ng �r klar och roboten har k�rt ur kurvan, �terg� till vanlig PD-reglering
		waypoint = 0;						
		ongoing_turn = false;
	}
}


//ska den heta _lab eller inte
void oneway_turn_lab(int distance_left,int distance_right,int distance_front,int st_value){//roboten kommer fram till en enkel kurva antingen till h�ger eller v�nster
							
	
	if(!ongoing_turn && distance_front > min_distance_front){    //---k�r fram i korsningen f�r att prepa sv�ng
		drive_forward();
		turn_done = false;
		
	}
	else if(!ongoing_turn && distance_front <= min_distance_front){			// redo att p�b�rja sv�ng
		ongoing_turn = true;
		start_turn = true;
		req_gyro_turn();
	}
	else if(start_turn){									//Roboten �r inne i kurvan och redo o sv�nga 
		if(sensor_left > st_value){								//Kollar avst�nd till h�ger och v�nster f�r att -
															//se vilket h�ll det ska sv�ngas och inleder sedan sv�ng
			turn_left();
		}
		else if(sensor_right > st_value){
			turn_right();
		}
		start_turn = false;										// Signalerar att sv�ng har	inledits f�r att inte g� in i denna if igen
		
	}
	
	else if(ongoing_turn && gyro_90){					//Gyrot singnalerar att sv�ngen �r klar och det �r klart att b�rja k�ra ur den
		turn_done = true;
		count_waypoint = 0; //oneway_turn
		gyro_turn = 0;
		drive_forward();
	}
	
	else if(turn_done && count_waypoint >= 5){	//sv�ng �r klar och roboten har k�rt ur kurvan, �terg� till vanlig PD-reglering
		oneway_turn = 0;
		ongoing_turn = false;
	}
	
}



void twoway_turn_left_lab(int distance_left,int distance_right,int distance_front,int st_value){// roboten kommer fram till ett v�gsk�l d�r den antingen kan sv�nga �t h�ger/v�nster eller k�ra raktfram
	
	if(first_time){    //---k�r fram i korsningen f�r att prepa sv�ng
		sensor_front_tmp = distance_front;
		drive_forward();
		turn_done = false;
		first_time = false;
	}
	else if(!ongoing_turn && distance_front <= sensor_front_tmp - 10){	// Roboten har k�rt ut i kurvan och signalerar att den �r klar och sv�nga
		ongoing_turn = true;
		start_turn = true;
	}
	
	else if(start_turn){				//Robot �r inne i kurva och kontrollerar vilket h�ll den ska sv�nga
		
			if(RGB_data!=3){			//RGB s�ger inte att roboten ska sv�nga v�nster s� den k�r rakt fram					
				drive_forward();							
			}
			else {						//RGB s�ger att roboten ska sv�nga v�nster s� v�nstersv�ng inleds samt kall p� Gyro f�r att kontrollera 90-grader
				turn_left();
				req_gyro_turn();
			}
												
		start_turn = false;				// Signalerar att sv�ng har	inledits f�r att inte g� in i denna if igen	
	}
	
	else if(RGB_data != 3 && distance_left < st_value && distance_right < st_value && distance_front > st_value){	//Roboten har k�rt igenom kurvan och �terg�r till vanlig reglering 
		twoway_turn_left = false;
		first_time = false;
		ongoing_turn = false;
	}
	else if(RGB_data == 3 && gyro_90){ //RGB sensor visar p� att vi ska sv�nga v�nster och RGB s�ger att vi roterat 90-grader	
		turn_done = true;
		count_waypoint = 0;			//counter som anv�nds f�r att roboten ska k�ra i 0.5 sek f�r att komma ur kurvor 
		gyro_90 = false;
		RGB_data = 0;				// RGB v�rdet nolls�tts f�r att kunna kontrollera f�r n�sta kurva
		drive_forward();			// motorerna k�r fram�t f�r att komma ur kurvan
	}
	
	else if(turn_done && count_waypoint >= 5){	//sv�ng �r klar och roboten har k�rt ur kurvan, �terg� till vanlig PD-reglering
		twoway_turn_left = false;
		ongoing_turn = false;
		first_time = true;
	}
}


//INTE KLAR
void twoway_turn_right_lab(int distance_left,int distance_right,int distance_front,int st_value){// roboten kommer fram till ett v�gsk�l d�r den antingen kan sv�nga �t h�ger/v�nster eller k�ra raktfram
	
	if(first_time){    //---k�r fram i korsningen f�r att prepa sv�ng
		sensor_front_tmp = distance_front;
		drive_forward();
		turn_done = false;
		first_time = false;
	}
	else if(!ongoing_turn && distance_front <= sensor_front_tmp - 10){	// Roboten har k�rt ut i kurvan och signalerar att den �r klar och sv�nga
		ongoing_turn = true;
		start_turn = true;
	}
	
	else if(start_turn){				//Robot �r inne i kurva och kontrollerar vilket h�ll den ska sv�nga
		
		if(RGB_data!=3){			//RGB s�ger inte att roboten ska sv�nga h�ger s� den k�r rakt fram
			drive_forward();
		}
		else {						//RGB s�ger att roboten ska sv�nga h�ger s� h�gersv�ng inleds samt kall p� Gyro f�r att kontrollera 90-grader
			turn_right();
			req_gyro_turn();
		}
		
		start_turn = false;				// Signalerar att sv�ng har	inledits f�r att inte g� in i denna if igen
	}
	
	else if(RGB_data != 1 && distance_left < st_value && distance_right < st_value && distance_front > st_value){	//Roboten har k�rt igenom kurvan och �terg�r till vanlig reglering
		twoway_turn_left = false;
		first_time = false;
		ongoing_turn = false;
	}
	else if(RGB_data == 1 && gyro_90){ //RGB sensor visar p� att vi ska sv�nga h�ger och RGB s�ger att vi roterat 90-grader
		turn_done = true;
		count_waypoint = 0;			//counter som anv�nds f�r att roboten ska k�ra i 0.5 sek f�r att komma ur kurvor
		gyro_90 = false;
		RGB_data = 0;				// RGB v�rdet nolls�tts f�r att kunna kontrollera f�r n�sta kurva
		drive_forward();			// motorerna k�r fram�t f�r att komma ur kurvan
	}
	
	else if(turn_done && count_waypoint >= 5){	//sv�ng �r klar och roboten har k�rt ur kurvan, �terg� till vanlig PD-reglering
		twoway_turn_right = false;
		ongoing_turn = false;
		first_time = true;
	}
}