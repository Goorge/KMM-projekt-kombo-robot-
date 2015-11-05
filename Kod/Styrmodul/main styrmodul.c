#include <avr/io.h>
#include <avr/interrupt.h>
#include <TWI_Master.h>

typedef unsigned char byte;
#include <"i2c master.c">

 //avbrotsrutiner
 ISR(INT0_vect)
 {
	if (PC6){	//komunikation vill skicka
	
	}
	else if(PC7){ //sensor vill skicka
		
	}
	 
 }
   
int main(){
    //init
	i2c_setup();
TWI_Master_Initialise();

 
	GICR = 1<<INT0;					// Enable INT0
	MCUCR = 1<<ISC01 | 1<<ISC00;	// Trigger INT0 on rising edge
	sei();				//Enable Global Interrupt 
    while(1){
       //FUNKTIONSLOP



    };
    RETURN (0);


};
