#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include "asf.h"

bool start_turn = false;
bool ongoing_turn = false;
bool turn_done = false;
bool first_time = true;
bool regler_against_wall = false;
bool prepare_special_case = false;

int previous_right = 0;
int previous_left = 0;
int p_wall = 200;
int d_wall = 10;
int sensor_front_tmp; //används då man kommer till ett vägskäl där man kan svänga eller köra rakt fram för att kunna åka ut i mitte av kurvan innan svän inleds
int min_distance_front = 30;
int previous_errors[10];
int number_of_errors_sc = 4;

short regulate_side = 0;



void drive_forward_left(int distance){
	int current_error = distance_wall_desired - distance; //-sensor_left_tmp
	derivate = current_error - previous_errors[error_count];///10; //
	if (derivate < 3)
		derivate = 0;
	int output = (p_constant_lab * current_error + d_constant_lab * derivate)/10;
	previous_errors[error_count] = current_error;

	if(++error_count >= number_of_errors_sc)
		error_count=0;
	
	if(output <= (-laby_left_speed))
		output = -laby_left_speed;
	
	else if(output >= laby_right_speed)
		output=laby_right_speed;
	
	if(output <= 0){
		motor_right = laby_right_speed;
		motor_left = laby_left_speed + output;
	}
	else{ 
		motor_left = laby_left_speed;
		motor_right = laby_right_speed - output;	
	}
}

void drive_forward_right(int distance, int distance_front){
	int current_error = distance_wall_desired - distance; //-sensor_left_tmp
	derivate = current_error - previous_errors[error_count];///10; //
	int output = (p_wall * current_error + d_wall * derivate)/10;
	previous_errors[error_count] = current_error;


	if(++error_count >= number_of_errors_sc)
		error_count = 0;
	
	if(output >= (laby_left_speed))
		output = laby_left_speed;
	
	else if(output <= -laby_right_speed)
		output = -laby_right_speed;
	
	if(output >= 0){
		motor_right = laby_right_speed;
		motor_left = laby_left_speed - output;
	}
	else{
		motor_left = laby_left_speed;
		motor_right = laby_right_speed + output;
	}
	/*
	if(distance_front <= 60 && distance < 20){
		turn_left();
		_delay_ms(50);	
		stand_still();
		for(int i = 0; i < number_of_errors_sc ; i++){
			previous_errors[i] = 0;
			
		}
	}
	else if (distance_front <= 60 && distance > 25){
		//turn_right;
		PORTD ^= (1 << PD0);
		PORTB |= (1 << motor_dir_right);
		PORTB &= ~(1 << motor_dir_left);
		motor_left = 57;//80;//57left;
		motor_right = 52;//52right;
		_delay_ms(80);	
		stand_still();
	}*/
}

//-------------------------------------
//Funktioner som kommer styra roboten vid specialfall som tillexempel t-korsning eller en vanlig sväng
//-------------------------------------
void waypoint_lab(int distance_left,int distance_right,int distance_front,int st_value){ //t-korsning 
	//PORTD ^= ( 1 << PD1 );
	if(!ongoing_turn && distance_front > min_distance_front){    //---kör fram i korsningen för att prepa sväng
		drive_forward();
		turn_done = false;
	}
	
	else if(!ongoing_turn && distance_front <= min_distance_front){			// redo att påbörja sväng
		ongoing_turn = true;
		start_turn = true;
		turn_done = false;
		stand_still();
	}
	
	else if(start_turn){					//startar svängen, kollar RGB, om vänster gira vänster annars gira höger(med eller utan rgb indikation).
		
		if(RGB_data == 3){					//RGB_data="blå" alltså sväng vänster vänster=blå
			req_gyro_turn();
			turn_left();
		}
		else {
			req_gyro_turn();
			turn_right();					//RGB_data="röd" alltså sväng höger
		}
		start_turn = false;					//Säger att svängen har inledits för att inte gå in i den här if-satsen igen
	}
	
	else if(ongoing_turn && !turning && (distance_left > st_value || distance_right > st_value)){	//Gyrot singnalerar att svängen är klar och det är klart att börja köra ur den
		turn_done = true;
		//count_waypoint = 0;
		drive_forward(); // Byt till att reglera mot vägg
	}
	
	else if(ongoing_turn && turn_done && distance_right < st_value && distance_left < st_value){	//sväng är klar och roboten har kört ur kurvan, återgå till vanlig PD-reglering
		waypoint = false;						
		ongoing_turn = false;
		prepare_special_case = false;
	}
}

void oneway_turn_lab(int distance_left,int distance_right,int distance_front,int st_value){//roboten kommer fram till en enkel kurva antingen till höger eller vänster
	PORTD ^= ( 1 << PD0 );
	if(!ongoing_turn && distance_front > min_distance_front){    //---kör fram i korsningen för att prepa sväng
		drive_forward();
		turn_done = false;
	}
	else if(!ongoing_turn && distance_front <= min_distance_front){			// redo att påbörja sväng
		turn_done = false;
		ongoing_turn = true;
		start_turn = true;
		stand_still();
	}
	else if(start_turn){	
		start_turn = false;									//Roboten är inne i kurvan och redo o svänga 
		if(distance_left > st_value){						//Kollar avstånd till höger och vänster för att -												//se vilket håll det ska svängas och inleder sedan sväng
			req_gyro_turn();
			turn_left();
			regulate_side = 1;
		}
		else if(distance_right > st_value){
			req_gyro_turn();
			turn_right();
			regulate_side = 2;
		}
												// Signalerar att sväng har	inledits för att inte gå in i denna if igen
		//previous_right = 0;
		//previous_left = 0;
		//return;
	}
	
	else if(ongoing_turn && !turn_done && (distance_left > st_value || distance_right > st_value)){					//Gyrot singnalerar att svängen är klar och det är klart att börja köra ur den
		//count_waypoint = 0; //oneway_turn
		//gyro_turn = 0;
		drive_forward();
		/*if(regulate_side == 1)
			drive_forward_right(distance_right, distance_front);
		else 
			drive_forward_left(distance_left);*/	
		//drive_forward(); // Byt till left eller right beroende på riktning
	}
	
	else if(!turn_done && distance_left < st_value && distance_right < st_value /*&& previous_left < st_value && previous_right < st_value*/){	//sväng är klar och roboten har kört ur kurvan, återgå till vanlig PD-reglering
		turn_done = true;
		oneway_turn = false;
		ongoing_turn = false;
		//PORTD ^= (1 << PD0);
		prepare_special_case = false;
	}
	previous_right = distance_right;
	previous_left = distance_left;
}


//Måste testas
void twoway_turn_left_lab(int distance_left,int distance_right,int distance_front,int st_value){// roboten kommer fram till ett vägskäl där den antingen kan svänga åt höger/vänster eller köra raktfram	
	PORTD |= (1 << PD1);
	if(!ongoing_turn /*&& ++count_waypoint <= 1*/){	// Roboten har kört ut i kurvan och signalerar att den är klar och svänga
		ongoing_turn = true;
		start_turn = true;
		stand_still();
		turn_done = false;
		first_time = false;
		//count_waypoint = 0;
	}
	else if(start_turn){				//Robot är inne i kurva och kontrollerar vilket håll den ska svänga
		start_turn = false;				// Signalerar att sväng har	inledits för att inte gå in i denna if igen
		if(RGB_data != 3){			//RGB säger inte att roboten ska svänga höger så den kör rakt fram
			drive_forward_right(distance_right, distance_front);
			turn_done = true;
			regler_against_wall = true;
			}
		else {						//RGB säger att roboten ska svänga vänster så högersväng inleds samt kall på Gyro för att kontrollera 90-grader
			req_gyro_turn();
			turn_left();
		}
			
		previous_right = 0;
		previous_left = 0;
		return;
	}
	
	// Rakt fram11
	else if(regler_against_wall && (distance_left > st_value )){
		drive_forward_right(distance_right, distance_front);
	}
	
	else if(RGB_data != 3 && distance_left <= st_value && distance_right <= st_value && previous_left < st_value && previous_right < st_value){// && distance_front > st_value){	//Roboten har kört igenom kurvan och återgår till vanlig reglering
		twoway_turn_left = false;
		first_time = true;
		ongoing_turn = false;
		RGB_data = 0;				// RGB värdet nollsätts för att kunna kontrollera för nästa kurva
		regler_against_wall = false;
		PORTD &= ~(1 << PD1);
		prepare_special_case = false;
	}
	
	// SVÄÄÄÄNG
	else if(ongoing_turn && RGB_data == 3 && !turn_done){ //RGB sensor visar på att vi ska svänga vänster och RGB säger att vi roterat 90-grader	
		turn_done = true;
		//count_waypoint = 0;			//counter som används för att roboten ska köra i 0.5 sek för att komma ur kurvor 
		drive_forward();			// motorerna kör framåt för att komma ur kurvan
	}
	
	else if(turn_done && distance_left < st_value && distance_right < st_value && previous_left < st_value && previous_right < st_value){// && distance_front > st_value){	//sväng är klar och roboten har kört ur kurvan, återgå till vanlig PD-reglering
		twoway_turn_left = false;
		PORTD &= ~(1 << PD1);
		ongoing_turn = false;
		first_time = true;
		prepare_special_case = false;
		RGB_data = 0;
	}
	previous_right = distance_right;
	previous_left = distance_left;	
}


//INTE KLAR + måste testas
void twoway_turn_right_lab(int distance_left,int distance_right,int distance_front,int st_value){// roboten kommer fram till ett vägskäl där den antingen kan svänga åt höger/vänster eller köra raktfram
	++count_waypoint;
	if(first_time){    //---kör fram i korsningen för att prepa sväng
		drive_forward();
		turn_done = false;
		first_time = false;
		count_waypoint = 0;
		ongoing_turn = false;
	}
	else if(!ongoing_turn && count_waypoint>= 1){	// Roboten har kört ut i kurvan och signalerar att den är klar och svänga
		ongoing_turn = true;
		start_turn = true;
		stand_still();
	}
	else if(start_turn){				//Robot är inne i kurva och kontrollerar vilket håll den ska svänga
		if(RGB_data == 2){			//RGB säger inte att roboten ska svänga höger så den kör rakt fram
			drive_forward_left(distance_left);
			turn_done = true;
			regler_against_wall = true;
		}
		else {						//RGB säger att roboten ska svänga höger så högersväng inleds samt kall på Gyro för att kontrollera 90-grader
			req_gyro_turn();
			turn_right();
		}	
		previous_right = 0;
		previous_left = 0;
		start_turn = false;				// Signalerar att sväng har	inledits för att inte gå in i denna if igen
		return;
	}
	
	// Rakt fram
	else if(regler_against_wall && (distance_right > st_value )){
		drive_forward_left(distance_left);
	}
	
	else if(RGB_data == 2 && distance_left < st_value && distance_right < st_value && distance_front > st_value){	//Roboten har kört igenom kurvan och återgår till vanlig reglering
		twoway_turn_right = false;
		first_time = true;
		ongoing_turn = false;
		RGB_data = 0;				// RGB värdet nollsätts för att kunna kontrollera för nästa kurva
		regler_against_wall = false;
		prepare_special_case = false;
	}
	
	// SVÄÄÄÄNG
	else if(ongoing_turn && RGB_data != 2 && !turn_done){ //RGB sensor visar på att vi ska svänga vänster och RGB säger att vi roterat 90-grader	
		turn_done = true;
		//count_waypoint = 0;			//counter som används för att roboten ska köra i 0.5 sek för att komma ur kurvor 
		drive_forward();			// motorerna kör framåt för att komma ur kurvan
	}
	
	else if(turn_done && distance_left < st_value && distance_right < st_value ){//&& distance_front > st_value){	//sväng är klar och roboten har kört ur kurvan, återgå till vanlig PD-reglering
		twoway_turn_right = false;
		ongoing_turn = false;
		first_time = true;
		prepare_special_case = false;
	}
	previous_right = distance_right;
	previous_left = distance_left;
}