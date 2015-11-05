
#include <asf.h> //?????
#include <avr/io.h>
#include <avr/interrupt.h>
#include <compat/twi.h>

//typedef int bool;
#define true 1
#define false 0

typedef unsigned char byte;
#include "i2c_slave.c"

void run();
void initialize();

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */

	//board_init();
	initialize();
	run();
	/* Insert application code here, after the board has been initialized. */
}

void initialize(){
	
}

void run(){
	while(true)	{
		
	}
}