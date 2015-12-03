#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include "asf.h"

bool start_turn = false;
bool ongoing_turn = false;
bool turn_done = false;
bool first_time=true;
int sensor_front_tmp; //används då man kommer till ett vägskäl där man kan svänga eller köra rakt fram för att kunna åka ut i mitte av kurvan innan svän inleds
int min_distance_front = 30;
void req_gyro_turn(void){ // fuktion man kallar på för att starta Gyro inför 90 graders sväng
	;
}
//hej
//-----------------------------------------
//Funktioner för att styra robot för höger/vänster eller framåt
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
//Funktioner som kommer styra roboten vid specialfall som tillexempel t-korsning eller en vanlig sväng
//-------------------------------------

void waypoint_lab(int distance_left,int distance_right,int distance_front,int st_value){ //t-korsning 
	
	if(!ongoing_turn && distance_front > min_distance_front){    //---kör fram i korsningen för att prepa sväng
		drive_forward();
		turn_done = false;
		
	}
	else if(!ongoing_turn && distance_front <= min_distance_front){			// redo att påbörja sväng
		ongoing_turn = true;
		start_turn = true;
		req_gyro_turn();
	}
	else if(start_turn){					//startar svängen, kollar RGB, om vänster gira vänster annars gira höger(med eller utan rgb indikation).
		
		if(RGB_data == 1){					//RGB_data="blå" alltså sväng vänster
			turn_left();
		}
		else {
			turn_right();					//RGB_data="röd" alltså sväng höger
		}
		
		start_turn = false;					//Säger att svängen har inledits för att inte gå in i den här if-satsen igen
	}
	else if(ongoing_turn && gyro_90){	//Gyrot singnalerar att svängen är klar och det är klart att börja köra ur den
		turn_done = true;
		count_waypoint = 0;
		gyro_turn = 0;
		drive_forward();
	}
	else if(turn_done && count_waypoint >= 5){	//sväng är klar och roboten har kört ur kurvan, återgå till vanlig PD-reglering
		waypoint = 0;						
		ongoing_turn = false;
	}
}


//ska den heta _lab eller inte
void oneway_turn_lab(int distance_left,int distance_right,int distance_front,int st_value){//roboten kommer fram till en enkel kurva antingen till höger eller vänster
							
	
	if(!ongoing_turn && distance_front > min_distance_front){    //---kör fram i korsningen för att prepa sväng
		drive_forward();
		turn_done = false;
		
	}
	else if(!ongoing_turn && distance_front <= min_distance_front){			// redo att påbörja sväng
		ongoing_turn = true;
		start_turn = true;
		req_gyro_turn();
	}
	else if(start_turn){									//Roboten är inne i kurvan och redo o svänga 
		if(sensor_left > st_value){								//Kollar avstånd till höger och vänster för att -
															//se vilket håll det ska svängas och inleder sedan sväng
			turn_left();
		}
		else if(sensor_right > st_value){
			turn_right();
		}
		start_turn = false;										// Signalerar att sväng har	inledits för att inte gå in i denna if igen
		
	}
	
	else if(ongoing_turn && gyro_90){					//Gyrot singnalerar att svängen är klar och det är klart att börja köra ur den
		turn_done = true;
		count_waypoint = 0; //oneway_turn
		gyro_turn = 0;
		drive_forward();
	}
	
	else if(turn_done && count_waypoint >= 5){	//sväng är klar och roboten har kört ur kurvan, återgå till vanlig PD-reglering
		oneway_turn = 0;
		ongoing_turn = false;
	}
	
}



void twoway_turn_left_lab(int distance_left,int distance_right,int distance_front,int st_value){// roboten kommer fram till ett vägskäl där den antingen kan svänga åt höger/vänster eller köra raktfram
	
	if(first_time){    //---kör fram i korsningen för att prepa sväng
		sensor_front_tmp = distance_front;
		drive_forward();
		turn_done = false;
		first_time = false;
	}
	else if(!ongoing_turn && distance_front <= sensor_front_tmp - 10){	// Roboten har kört ut i kurvan och signalerar att den är klar och svänga
		ongoing_turn = true;
		start_turn = true;
	}
	
	else if(start_turn){				//Robot är inne i kurva och kontrollerar vilket håll den ska svänga
		
			if(RGB_data!=3){			//RGB säger inte att roboten ska svänga vänster så den kör rakt fram					
				drive_forward();							
			}
			else {						//RGB säger att roboten ska svänga vänster så vänstersväng inleds samt kall på Gyro för att kontrollera 90-grader
				turn_left();
				req_gyro_turn();
			}
												
		start_turn = false;				// Signalerar att sväng har	inledits för att inte gå in i denna if igen	
	}
	
	else if(RGB_data != 3 && distance_left < st_value && distance_right < st_value && distance_front > st_value){	//Roboten har kört igenom kurvan och återgår till vanlig reglering 
		twoway_turn_left = false;
		first_time = false;
		ongoing_turn = false;
	}
	else if(RGB_data == 3 && gyro_90){ //RGB sensor visar på att vi ska svänga vänster och RGB säger att vi roterat 90-grader	
		turn_done = true;
		count_waypoint = 0;			//counter som används för att roboten ska köra i 0.5 sek för att komma ur kurvor 
		gyro_90 = false;
		RGB_data = 0;				// RGB värdet nollsätts för att kunna kontrollera för nästa kurva
		drive_forward();			// motorerna kör framåt för att komma ur kurvan
	}
	
	else if(turn_done && count_waypoint >= 5){	//sväng är klar och roboten har kört ur kurvan, återgå till vanlig PD-reglering
		twoway_turn_left = false;
		ongoing_turn = false;
		first_time = true;
	}
}


//INTE KLAR
void twoway_turn_right_lab(int distance_left,int distance_right,int distance_front,int st_value){// roboten kommer fram till ett vägskäl där den antingen kan svänga åt höger/vänster eller köra raktfram
	
	if(first_time){    //---kör fram i korsningen för att prepa sväng
		sensor_front_tmp = distance_front;
		drive_forward();
		turn_done = false;
		first_time = false;
	}
	else if(!ongoing_turn && distance_front <= sensor_front_tmp - 10){	// Roboten har kört ut i kurvan och signalerar att den är klar och svänga
		ongoing_turn = true;
		start_turn = true;
	}
	
	else if(start_turn){				//Robot är inne i kurva och kontrollerar vilket håll den ska svänga
		
		if(RGB_data!=3){			//RGB säger inte att roboten ska svänga höger så den kör rakt fram
			drive_forward();
		}
		else {						//RGB säger att roboten ska svänga höger så högersväng inleds samt kall på Gyro för att kontrollera 90-grader
			turn_right();
			req_gyro_turn();
		}
		
		start_turn = false;				// Signalerar att sväng har	inledits för att inte gå in i denna if igen
	}
	
	else if(RGB_data != 1 && distance_left < st_value && distance_right < st_value && distance_front > st_value){	//Roboten har kört igenom kurvan och återgår till vanlig reglering
		twoway_turn_left = false;
		first_time = false;
		ongoing_turn = false;
	}
	else if(RGB_data == 1 && gyro_90){ //RGB sensor visar på att vi ska svänga höger och RGB säger att vi roterat 90-grader
		turn_done = true;
		count_waypoint = 0;			//counter som används för att roboten ska köra i 0.5 sek för att komma ur kurvor
		gyro_90 = false;
		RGB_data = 0;				// RGB värdet nollsätts för att kunna kontrollera för nästa kurva
		drive_forward();			// motorerna kör framåt för att komma ur kurvan
	}
	
	else if(turn_done && count_waypoint >= 5){	//sväng är klar och roboten har kört ur kurvan, återgå till vanlig PD-reglering
		twoway_turn_right = false;
		ongoing_turn = false;
		first_time = true;
	}
}