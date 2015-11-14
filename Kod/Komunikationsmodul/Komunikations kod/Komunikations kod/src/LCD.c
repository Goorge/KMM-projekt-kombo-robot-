#include <asf.h>

#define RS PD6
#define E PD7
#define DataPort PORTA;
#define NELEMS(x)  (sizeof(x) / sizeof((x)[0]))

bool directionLeft = true;

void lcd_move_cursour_home(){
	PORTA = 0x02;
	PORTD |= 1 << RS;
	_delay_ms(1.53);
}

void lcd_display_on(){
	PORTD &= 0 << RS;
	PORTA = 0x0E;
	_delay_us(39);
}

void lcd_set_ram_adress(){
	PORTD &= 0 << RS;
	PORTA = 0x40;
	_delay_us(39);
}

void lcd_set_mode(){
	PORTD &= 0 << RS;
	PORTA = 0x06;
	_delay_us(0.039);
}
//LCD Setup, inits all the variables
void lcd_setup(){
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
	_delay_ms(1);
	
	//lcd_display_on();
	lcd_set_mode();
	lcd_move_cursour_home();
	lcd_set_ram_adress();
	
}

// writes a char on current possition
void lcd_write_char(char letter){
	PORTD |= 1 << RS; // sätt RS
	_delay_us(0.04);//TSU1
	PORTD |= 1 << E; // Sätt Enable
	_delay_us(0.130);//Tw - Tsu2
	PORTA = letter; // Skicka in data
	_delay_us(0.08);//TSU2	// Vänta på att data ska stabiliseras
	PORTD &= 0 << E;
	_delay_us(0.01);
	PORTD &= 0 << RS;
}

void lcd_clear_display(){
	PORTD &= 0 << RS;
	PORTA = 0x01;
	_delay_ms(1.53);
}

void lcd_write_string(char text[]){
	//lcd_clear_display();
	lcd_move_cursour_home();
	for(int i = 0; i < NELEMS(text); i++)
		lcd_write_char(text[i]);
}



