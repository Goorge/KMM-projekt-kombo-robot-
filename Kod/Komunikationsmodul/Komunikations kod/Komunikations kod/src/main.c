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
	char test[2][16] = {"test", "Mer Test"};
	lcd_write_string(test);
	//lcd_write_instruction(lcd_Clear);
	_delay_us(80);
	//lcd_write_char('A');

	short number_of_bytes = 0;
	bool new_data = false;

	while(true)	{		

		

		// Ta emot data fr�n BT
		if(bluetooth_get_new_data() == true){ // Om vi har f�tt data s�tts newData till true i BT och vi kollar p� den h�r
			byte data = bluetooth_fetch_new_data(); // H�mta ut data � s�tt newData false
			bluetooth_clear_to_send(); // S�g att du vill ha mer BT data
		} 
		
		// Skicka data via BT
		if((PIND & (1<<CTS)) == 0){ // Vi har till�telse att skicka data
				bluetooth_send_byte(0x04);//0b00000100
		}
	}
}