#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include "asf.h"

int sensor_front=15;
bool start_turn=false;
bool ongoing_turn=false;
bool turn_done=false;
int RGB_sensor=0;
int sensor_front_tmp; //används då man kommer till ett vägskäl där man kan svänga eller köra rakt fram för att kunna åka ut i mitte av kurvan innan svän inleds

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
	PORTB &= ~(1 << motor_dir_left) | (1 << motor_dir_right);
	motor_left = left;
	motor_right = right;
	
}
// Snabb dator LOL!
//-------------------------------------
//Funktioner som kommer styra roboten vid specialfall som tillexempel t-korsning eller en vanlig sväng
//-------------------------------------

void waypoint_lab(void){ //t-korsning 
	
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
	else if(start_turn){					//startar svängen, kollar RGB, om vänster gira vänster annars gira höger(med eller utan rgb indikation).
		
		if(RGB_sensor==1){					//RGB_data="blå" alltså sväng vänster
			turn_left();
		}
		else {
			turn_right();					//RGB_data="röd" alltså sväng höger
		}
		
		start_turn=false;					//Säger att svängen har inledits för att inte gå in i den här if-satsen igen
	}
	else if(ongoing_turn && gyro_turn==1){	//Gyrot singnalerar att svängen är klar och det är klart att börja köra ur den
		turn_done=true;
		count_waypoint=0;
		gyro_turn=0;
		drive_forward();
	}
	else if(turn_done && count_waypoint >= 5){	//sväng är klar och roboten har kört ur kurvan, återgå till vanlig PD-reglering
		waypoint = 0;						
		ongoing_turn = false;
	}


}


//ska den heta _lab eller inte
void oneway_turn_lab(void){//roboten kommer fram till en enkel kurva antingen till höger eller vänster
							
	
	if(!ongoing_turn && sensor_front > 15){    //---kör fram i korsningen för att prepa sväng
		drive_forward();
		turn_done=false;
		
	}
	else if(!ongoing_turn && sensor_front <= 15){			// redo att påbörja sväng
		ongoing_turn=true;
		start_turn=true;
		req_gyro_turn();
	}
	else if(start_turn){									//Roboten är inne i kurvan och redo o svänga 
		if(sensor_left > 57){								//Kollar avstånd till höger och vänster för att -
															//se vilket håll det ska svängas och inleder sedan sväng
			turn_left();
		}
		else if(sensor_right > 57){
			turn_right();
		}
		start_turn=0;										// Signalerar att sväng har	inledits för att inte gå in i denna if igen
		
	}
	
	else if(ongoing_turn && gyro_turn==1){					//Gyrot singnalerar att svängen är klar och det är klart att börja köra ur den
		turn_done=true;
		count_waypoint=0; //oneway_turn
		gyro_turn=0;
		drive_forward();
	}
	
	else if(turn_done && count_waypoint >= 5){	//sväng är klar och roboten har kört ur kurvan, återgå till vanlig PD-reglering
		oneway_turn= 0;
		ongoing_turn = false;
	}
	
}



void twoway_turn_lab(void){// roboten kommer fram till ett vägskäl där den antingen kan svänga åt höger/vänster eller köra raktfram
	if(!ongoing_turn ){    //---kör fram i korsningen för att prepa sväng
		sensor_front_tmp=sensor_front;
		drive_forward();
		turn_done=false;
	}
}