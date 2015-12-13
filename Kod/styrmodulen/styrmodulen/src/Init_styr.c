#include "asf.h"
#include <avr/interrupt.h>
#include <avr/io.h>

// ------------------------------------------------------------------------------------------------------
// -- Init first time for engine/ motors
// ------------------------------------------------------------------------------------------------------
void initengine(void){
	TCCR0A |= (1 << COM0A1) | (1 << COM0B1) | (1 << WGM01) | (1 << WGM00);  //0xA3; // Initiate fast PWM so that it goes high longer for bigger values on OCR0A and OCR0B
	motor_left=0;      // Initiate speed=0 on both engines
	motor_right=0;
	TCCR0B |= (1 << CS00) | (1 << CS01) ;       //0x0B;// test differnt values shouldnt matter what it is imo (clk freq for the PWM 1/8 main clk)
	
	DDRB = 0x78;								//set I/0 ports
	PORTB &=  (0 << motor_dir_left) | (0 << motor_dir_right);			//set motors to move forward as default
}
// ------------------------------------------------------------------------------------------------------
// -- Timer init with interrupt, first time
// ------------------------------------------------------------------------------------------------------
void timerinit(void){
	DDRD = 0x03; // set dport 0&1 to output
	TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10); // enable timer compare and prescale 1/1024
	OCR1A = 1440; // compare if timer count up to 1440 (0,1*14745600/1024=1440)
	TIMSK1 |= (1 << OCIE1A);
	TIFR1 |= (1 << OCF1A); // enable timer compare interrupt
}
// -- Interrupt
ISR(TIMER1_COMPA_vect){
	//regler_ready=1;
	if(turn_delay < 2)
		turn_delay++;
	
	updaterad_labyrint = true;
	counter_timer_line_lab++;
	if(counter_timer_line_lab >= 50){
		counter_timer_line_lab = 50;			// för att undvika overflow
	}
	counter_timer_line_RGB++;
	if(counter_timer_line_RGB >= 50){			
		counter_timer_line_RGB = 50;			// för att undvika overflow
	}
	counter_timer_line_goal++;
	if(counter_timer_line_goal >= 50)
		counter_timer_line_goal = 50;
		
	//count_waypoint++;			//räknare för att köra ur svängar i labyrinter
}
// ------------------------------------------------------------------------------------------------------
// -- External interrupt for INT1 and INT2
// ------------------------------------------------------------------------------------------------------
void interrupt_init(void){
	EIMSK |= (1 << INT1) | (1 << INT2);			//Enable INT1 and INT2
	EICRA |= (1 << ISC11) | (1 << ISC21);		//The falling edge of INTn generates asynchronously an interrupt request.
}
// -- External interrupt for INT1
ISR(INT1_vect){						// INT1 is Manual/ auto button
	if(drive_mode==1){
		drive_mode=0;
		PORTD |= (1 << PD1);
		PORTD &= ~(1 << PD0);
	}
	else{
		start=0;
		drive_mode=1;
		PORTD |= (1 << PD0);
		PORTD &= ~(1 << PD1);
	}
	EIFR |= (1<< INTF1);
}
// -- External interrupt for INT2
ISR(INT2_vect){						// INT2 is start and turn off
	if(start==0){
		start=1;
	}
	else{
		start=0;
	}
	EIFR |= (1<< INTF2);
}
// ------------------------------------------------------------------------------------------------------
