#include "definitions.h"
#include "LCD.h"
#include  "bluetooth.h"

//LCD Setup, inits all the variables
void lcd_setup(void){
	DDRA = 0xFF;
	DDRD |= (1 << E) | (1 << RS);
		
	PORTD &= ~((1 << E) | (1 << RS));		
	_delay_ms(100); // wait for vdd to rise to 4.5V 
	// Function reset Beh�vs nog inte
	//lcd_write_instruction(lcd_FunctionReset); //b3 antal rader, b2 antal punkter, 8 eller 11  // Kanske inte beh�vs
	// Function set
	lcd_write_instruction(lcd_FunctionSet); //b3 antal rader, b2 antal punkter, 8 eller 11  
	// Display on
	lcd_write_instruction(lcd_DisplayOn); //b2 display, b1 cursor, b0 blink
	// Display clear
	lcd_write_instruction(lcd_Clear);
	// Entry Mode Set
	lcd_write_instruction(lcd_EntryMode);//b1 inc/dec, b0, Entire shift
	// Set cursor home and on ddrd 
	lcd_write_instruction(lcd_SetCursor);
}

// writes a char on current possition
void lcd_write_char(uint8_t letter){
	PORTD &= ~(1 << E);	// F�rs�kring om att E �r l�g innan vi f�rs�ker skriva
	PORTA = letter;	// L�gg �n data p� pinnarna in data
	
	PORTD |= (1 << RS); // s�tt RS
	_delay_us(1); //TSU1
	PORTD |= (1 << E); // S�tt Enable
	_delay_us(1); // TW
	PORTD &= ~(1 << E);
	_delay_us(1); // TH1
	PORTD &= ~(1 << RS); // Nolst�ll E och RS s� att dom �r noll vid n�sta instruktion/charl�sning
	_delay_us(100); //TSU2	// V�nta p� att data ska stabiliseras
}

// Writes instruction 
void lcd_write_instruction(uint8_t instruction){
	PORTD &= ~(1 << E);// Se till att E �r l�g fr�n b�rjan
	PORTA = instruction; // funkar inte detta prova bit f�r bit. G�ller isf �ven utskrift.
	
	
	PORTD &= ~(1 << RS); // s�tt RS
	_delay_us(1); //TSU1
	PORTD |= (1 << E); // S�tt Enable
	_delay_us(1); // TW
	PORTD &= ~(1 << E);
	_delay_us(1); // TH1
	PORTD &= ~(1 << RS); // Nolst�ll E och RS s� att dom �r noll vid n�sta instruktion/charl�sning	

	// L�gg in beh�vd delay ( 1.53 ms f�r clear och home & 39 us f�r de andra )
	if(instruction == lcd_Home || instruction == lcd_Clear)
		_delay_ms(3);
	else
		_delay_us(100);
}


//Erases data on LCD and write new one.
void lcd_write_string(uint8_t text[], uint8_t row_instrucion){
	lcd_write_instruction(row_instrucion);
	for(int i = 0; i < NELEMS(text); i++)
		lcd_write_char(text[i]);
}