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
void update_lcd(void);

int main (void){
	board_init();
	initialize();
	run();
}

void initialize(void){
	bluetooth_setup((long)115200);
	lcd_setup();
	//i2c_setup(0x02);
	
	sei(); // Enable interrupt
	_delay_ms(100);
}

void run(void){
	uint8_t rad1[] = "detta ar text   ";
	uint8_t rad2[] = "mer text        ";
	byte data[7] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07};
	lcd_write_instruction(lcd_DisplayOff);
	//lcd_write_char('A');
	//_delay_us(80);
	
	//How many bytes is there during current read
	short number_of_bytes_from_bt = 0;
	//short number_of_bytes_from_i2c = 0;
	
	// how many bytes is left to read=
	short bytes_left_from_bt = -1;
	//short bytes_left_from_i2c = 0;
	
	// the byte we are currently reading
	byte data_from_bt[16];
	//byte data_from_i2c[16];
	
	//How many bytes is there during current read
	short number_of_bytes_to_bt = 0;
	//short number_of_bytes_to_i2c = 0;
	
	// how many bytes is left to read=
	short bytes_left_to_bt = -1;
	//short bytes_left_to_i2c = 0;
		
	// the byte we are currently reading
	byte data_to_bt[16];
	//byte data_to_i2c[16];
	bool dataSend = false;
	while(true)	{		
		// Hantera eventuell ny data ifrån BT
		if(bluetooth_get_new_data() == true){ // Om vi har fått data sätts newData till true i BT och vi kollar på den här
			byte data = bluetooth_fetch_new_data(); // Hämta ut data å sätt newData false
			
			// Om vi inte har någon kö för stunden så kör
			if (bytes_left_from_bt == -1){ 
				number_of_bytes_from_bt = (data >> 4); 
				bytes_left_from_bt = number_of_bytes_from_bt;
			}
			
			// Lägg in datan i vektorn
			data_from_bt[number_of_bytes_from_bt - bytes_left_from_bt] = data;
			--bytes_left_from_bt;
			
			// Vill vi ha mer data från samma sändning? 				
			if(bytes_left_from_bt != -1)// Om rts är 0 lagara blåtandseneheten data å skickar via usart när usarten är redo
				bluetooth_clear_to_send(); // Säg att du vill ha mer BT data
			else dataSend = true;	
		} 
		
		if( dataSend ) { //bytes_left_from_bt == -1 && bytes_left_to_bt == -1){
			for(int i = 0; i < 7; i++)
				data_to_bt[i] = data[i];
			dataSend = false;
			number_of_bytes_to_bt = 7;//number_of_bytes_from_bt;
			bytes_left_to_bt = 7;//number_of_bytes_from_bt;
			bluetooth_clear_to_send();
			// Då det bara kommer skickas styrdata via bt är det dags att skicka data till styrmodulen
			// i2csendstuff.
		}
		
		//Skriv skräp till lcd
		lcd_write_instruction(lcd_Clear);
		lcd_write_string(rad1, lcd_SetCursor | lcd_LineOne);
		lcd_write_string(rad2, lcd_SetCursor | lcd_LineTwo);
		
		// Skicka data via BT
		if(((PIND & (1<<CTS)) == 0) && bytes_left_to_bt != -1){ // Vi har tillåtelse att skicka data & data att skicka
			bluetooth_send_byte(data_to_bt[number_of_bytes_to_bt - bytes_left_to_bt]);
			--bytes_left_to_bt;
		}
	}
}
