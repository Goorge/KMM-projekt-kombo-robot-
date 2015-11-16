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
	DDRD = (1 << E) | (1 << RS);
			
	_delay_ms(100); // wait for vdd to rise to 4.5V 
	//Function reset
	lcd_write_instruction(lcd_FunctionReset); //b3 antal rader, b2 antal punkter, 8 eller 11  
	_delay_us(80);
	//Function set
	lcd_write_instruction(lcd_FunctionSet); //b3 antal rader, b2 antal punkter, 8 eller 11  
	_delay_us(80);
	//Display on
	lcd_write_instruction(lcd_DisplayOn); //b2 display, b1 cursor, b0 blink
	_delay_us(80);
	//Display clear
	lcd_write_instruction(lcd_Clear);
	_delay_ms(4);
	//Entry Mode Set
	lcd_write_instruction(lcd_EntryMode);//b1 inc/dec, b0, Entire shift
	_delay_ms(1);
	
	lcd_write_instruction(lcd_SetCursor);
	_delay_us(80);
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
	PORTA = letter;	// Lägg ín data på pinnarna in data
	PORTD &= ~(1 << E);	// Försäkring om att E är låg innan vi försöker skriva
	
	PORTD |= (1 << RS); // sätt RS
	PORTD |= (1 << E); // Sätt Enable
	
	_delay_us(1); //TSU2	// Vänta på att data ska stabiliseras
	PORTD &= ~(1 << E);
	PORTD &= ~(1 << RS);
	_delay_us(1);
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
	
	PORTA = instruction; // funkar inte detta prova bit för bit. Gäller isf även utskrift.
}

//Erases data on LCD and write new one.
void lcd_write_string(char string[2][16]){
	lcd_write_instruction(lcd_Clear);
	_delay_us(80);
	lcd_write_instruction(lcd_SetCursor | lcd_LineOne);
	_delay_us(80);	// Behövs troligtvis inte då clear borde 0, om jag inte fattar databladet fel
	
	for(int line = 0; line < 2; line++){
		for(int i = 0; i < NELEMS(string[line]); i++){
			lcd_write_char(string[line][i]); 
			_delay_us(80);
		}
		lcd_write_instruction(lcd_SetCursor | lcd_LineTwo);
		_delay_us(80); 
	}
}



