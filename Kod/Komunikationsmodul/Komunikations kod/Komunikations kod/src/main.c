#include <asf.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <compat/twi.h>

#include "definitions.h"
//#include "i2c_slave.c"
#include "bluetooth.h"
#include "LCD.h"


void run(void);
void initialize(void);

byte a;

int main (void)
{
	board_init();
	initialize();
	run();
}

void initialize(void){
	//i2c_setup(0x02);
	DDRB = 0xff;
	bluetooth_setup((long)115200);
	
	lcd_setup();
	
	sei();
	_delay_ms(100);
}

void run(void){
	//char test[2][16] = {"test", "Mer Test"};
	//lcd_write_string(test);
	//lcd_write_instruction(lcd_Clear);
	//_delay_us(80);
	//lcd_write_char('A');

	while(true)	{		
		if(bluetooth_get_new_data() == true){	
			if((PIND & (1<<CTS)) == 0){ // Vi har tillåtelse att skicka data
				byte data = bluetooth_fetch_new_data();
				bluetooth_clear_to_send(); // Säg att du vill ha mer BT data
				//sei(); //slå på avbrott igen
				PORTB = data;
				bluetooth_send_byte(0x04);//0b00000100
			}
		} 
		//PORTB = 0x00;
	}
}