#include <avr/io.h>
#include <avr/interrupt.h>
#include <asf.h>
#include <compat/twi.h>

// Global variables
int drive_mode = 0;			// Manual or auto mode
int start = 0;				// Start and stop
int left = 0;					// Set speed for left motor
int right = 0;				// Set speed for right motor
int regler_ready = 0;			// Ready for new values from sensors if 1
int gyro_turn = 0;			// When 1 the gyro has turned 90 degree
int count_waypoint = 0;		// Time to move away from waypoint
int counter_timer = 0;		// Counter for how long time a manual function going to run
int counter_timer_line=0;
int counter_timer_line_lab = 0;

bool gyro_90;				//om gyro har sv�ngt 90 grader
byte batteri;				//batteri niv�
byte distans_fram;			// data avst�nd fram
byte distans_right;			// data avst�nd h�ger
byte distans_left;			// data avst�nd v�nster
byte RGB_data;				// vilken f�rg som �r detekterad
int Reflex_data;			// reflexsensor data
int Reflex_data2;
int regulator_mode = 1;		//1=linje, 0=labyrint
int regler_ready_linje=0;

bool updaterad_labyrint = false;
bool turning = false;

#include <util/delay.h>
#include "linjealg.c"
#include "PDreglering.c"
#include "Manual_drive.c"
#include "NormalPD_lab.c"
#include "Spec_cases_lab.c"
#include "labyrintreg.c"
#include "Linje.c"
#include "Init_styr.c"
#include "i2c_master.c"


int main(void)
{
	interrupt_init();
	board_init();
	initengine();
	timerinit();
	i2c_setup(1);
	sei();
	//***********Set motors speed value here *****************
	left=130*0;				// kanske r�cker med en variable f�r b�da om b�da k�r exakt lika fort
	right=120*0;
	//********************************************************
	motor_left=left;
	motor_right=right;
	while(1){
		i2c_handel_data();  //test av fregulito
		if(drive_mode == 1){						// Drivemode is auto    // (drive_mode == 1 && start == 1)
			//current_position = arre[count_arre];	// anv�nds f�r att simulera linjef�ljning, arrayen �ndras i globala
			if(start == 1){
				regulator();	
			}
			else if(start == 0){					// if start is zero then turn off the auto, stops motors
				motor_left=0;
				motor_right=0;
				}
			}
		else if(drive_mode==0){										// Drivemode is manual
			PORTD |= (1 << PD1);
			manual_drive();
		}
	}
}