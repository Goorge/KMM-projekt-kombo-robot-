
#include <asf.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <compat/twi.h>
#include <util/delay.h>

#include "i2c_slave.c"
#include "usart.c"
#include "bluetooth.c"
#include "LCD.c"

void run();
void initialize();

byte a;

//ISR(TWI_vect){
//	a=incomingData();
//}

int main (void)
{
	board_init();
	initialize();
	run();
}

void initialize(){
	i2c_setup(0x02);
	bluetooth_setup(115200);
	lcd_setup();
	DDRB = 0xff;
	//sei();
}

void run(){
	char test[] = {"test"};
	//lcd_write_string(test);
	lcd_write_char('A');
	while(true == true)	{
		//RequestToSend(0x02);
		
		if(a == 0xf0){
			DDRD = 1<<PD7;
			PORTD= 1<<PD7;
		}
	}
}