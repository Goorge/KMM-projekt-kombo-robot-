#include <asf.h>
#include "usart.c"

#define RS PD6
#define E PD7
#define DataPort PORTA;
#define NELEMS(x)  (sizeof(x) / sizeof((x)[0]))

bool directionLeft = true;

//LCD Setup, inits all the variables
lcd_setup(){
	DDRD |= (1<<RS)|(1<<E);
	_delay_ms(30); // wait for vdd to rise to 4.5V 
	//Function Set
	PORTD = (0<<RS)|(0<<E);
	PORTA = 0b00111000; //b3 antal rader, b2 antal punkter, 8 eller 11  
	_delay_us(39);
	//Display on
	PORTD = (0<<RS)|(0<<E);
	PORTA = 0b00001110; //b2 display, b1 cursor, b0 blink
	_delay_us(39);
	//Display clear
	PORTD = (0<<RS)|(0<<E);
	PORTA = 0b00000001;
	_delay_us(1530);
	//Entry Mode Set
	PORTD = (0<<RS)|(0<<E);
	PORTA = 0b00000111;//b1 inc/dec, b0, Entire shift
}

// writes a char on current possition
void lcd_write_char(char letter){
	PORTD |= 1 << RS; // sätt RS
	_delay_us(0.4);
	PORTD |= 1 << E; // Sätt Enable
	_delay_us(0.2);
	PORTA = letter; // Skicka in data
	_delay_us(1);	// Vänta på att data ska stabiliseras
}

void lcd_clear_display(){
	PORTA = 0x01;
	PORTD |= 1 << RS;
	_delay_us(0.2);
}

void lcd_write_string(char text[]){
	lcd_clear_display();
	for(int i = 0; i < NELEMS(text); i++)
		lcd_write_char(text[i]);
}

void lcd_move_cursour_home(){
	PORTA = 0x02;
	PORTD |= 1 << RS;
	_delay_us(0.2);
}

