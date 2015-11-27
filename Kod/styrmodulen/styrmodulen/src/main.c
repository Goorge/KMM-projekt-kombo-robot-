#include <avr/io.h>
#include <avr/interrupt.h>
#include <asf.h>
#include <compat/twi.h>
#include "PDreglering.c"
#include "Manual_drive.c"
#include "labyrintreg.c"
#include "Linje.c"
#include "Spec_cases_lab.c"
#include "Init_styr.c"
typedef unsigned char byte;
#include "i2c_master.c"
#include "linjealg.c"


// Global variables
int drive_mode=0;			// Manual or auto mode
int start=0;				// Start and stop
int left=0;					// Set speed for left motor
int right=0;				// Set speed for right motor
int regler_ready=0;			// Ready for new values from sensors if 1
int gyro_turn=0;			// When 1 the gyro has turned 90 degree
int count_waypoint=0;		// Time to move away from waypoint
int counter_timer=0;				// Counter for how long time a manual function going to run

//*********************************for test only
int count_arre=0;
int arre[] = {	-5,	-5, -5, -5, -3,
				-3, -5,  -3,  -5, -3,
				-2,  -1,  0,  0, 0,
				0,  0,  0,  0, 0,
				0,  0, 0,  0, 0,
				5,  4,  3,  2, 0 };
//*********************************for test only

int main(void)
{
	interrupt_init();
	board_init();
	initengine();
	timerinit();
	i2c_setup(1);
	linje_setup();
	sei();
	//***********Set motors speed value here *****************
	left=100;				// kanske r�cker med en variable f�r b�da om b�da k�r exakt lika fort
	right=93;
	//*********************************************************
	motor_left=left;      
	motor_right=right;
	while(1){
		i2c_handel_data();  //test av fregulito	
			if(drive_mode == 1){						// Drivemode is auto    // (drive_mode == 1 && start == 1)
				PORTB &= (0 << motor_dir_left) | (0 << motor_dir_right);		// Set motor direction to forward
				current_position = arre[count_arre];	// anv�nds f�r att simulera linjef�ljning, arrayen �ndras i globala
				if(regler_ready==1 && start == 1){
					regulator();
					regler_ready=0;	
				}
				else if(start == 0){					// if start is zero then turn off the auto, stops motors
					motor_left=0;
					motor_right=0;
				}
			}
			else if(drive_mode==0){										// Drivemode is manual
				manual_drive();
			}
	}
}