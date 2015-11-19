#include "definitions.h"
#include "LCD.h"

#define RS PD6
#define E PD7
#define DataPort PORTA;
#define NELEMS(x)  (sizeof(x) / sizeof((x)[0]))

bool directionLeft = true;

//LCD Setup, inits all the variables
void lcd_setup(void)	{
	DDRA = 0xFF;
	DDRD |= (1 << E) | (1 << RS);
			
	_delay_ms(100); // wait for vdd to rise to 4.5V 
	//Function reset
	lcd_write_instruction(lcd_FunctionReset); //b3 antal rader, b2 antal punkter, 8 eller 11  // Kanske inte beh�vs
	//Function set
	lcd_write_instruction(lcd_FunctionSet); //b3 antal rader, b2 antal punkter, 8 eller 11  
	//Display on
	lcd_write_instruction(lcd_DisplayOn); //b2 display, b1 cursor, b0 blink
	//Display clear
	lcd_write_instruction(lcd_Clear);
	//Entry Mode Set
	lcd_write_instruction(lcd_EntryMode);//b1 inc/dec, b0, Entire shift
}

// writes a char on current possition
void lcd_write_char(char letter){
/*
	PORTA = 0x00;
	PORTA |= (letter & (1 << 7));
	PORTA |= (letter & (1 << 6));
	PORTA |= (letter & (1 << 5));
	PORTA |= (letter & (1 << 4));
	PORTA |= (letter & (1 << 3));
	PORTA |= (letter & (1 << 2));
	PORTA |= (letter & (1 << 1));
	PORTA |= (letter & (1 << 0));
*/
	PORTA = letter;	// L�gg �n data p� pinnarna in data
	PORTD &= ~(1 << E);	// F�rs�kring om att E �r l�g innan vi f�rs�ker skriva
	
	PORTD |= (1 << RS); // s�tt RS
	PORTD |= (1 << E); // S�tt Enable
	
	_delay_us(100); //TSU2	// V�nta p� att data ska stabiliseras
	PORTD &= ~(1 << E); // Nolst�ll E och RS s� att dom �r noll vid n�sta instruktion/charl�sning
	PORTD &= ~(1 << RS); //
}

void lcd_write_instruction(uint8_t instruction){
	PORTD &= ~(1 << E);
	PORTD &= ~(1 << RS);
	
	/*
	PORTA = 0x00;
	PORTA |= (instruction & (1 << 7));
	PORTA |= (instruction & (1 << 6));
	PORTA |= (instruction & (1 << 5));
	PORTA |= (instruction & (1 << 4));
	PORTA |= (instruction & (1 << 3));
	PORTA |= (instruction & (1 << 2));
	PORTA |= (instruction & (1 << 1));
	PORTA |= (instruction & (1 << 0));
	*/
	
	PORTA = instruction; // funkar inte detta prova bit f�r bit. G�ller isf �ven utskrift.

	if(instruction == lcd_Home || instruction == lcd_Clear)
		_delay_ms(3);
	else
		_delay_us(100);
}

//Erases data on LCD and write new one.
void lcd_write_string(char text[], uint8_t row_instrucion){
	lcd_write_instruction(row_instrucion);
	
	for(int i = 0; i < NELEMS(text); i++){
		lcd_write_char(text[i]); 
	}
}



