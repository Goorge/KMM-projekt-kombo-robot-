#include <asf.h>

#define RS PD6
#define E PD7
#define DataPort PORTA;
#define NELEMS(x)  (sizeof(x) / sizeof((x)[0]))

bool directionLeft = true;

//LCD Setup, inits all the variables
void lcd_setup(){
	DDRD |= (1<<RS)|(1<<E);
	_delay_ms(80); // wait for vdd to rise to 4.5V 
	//Function Set
	PORTD = (0<<RS)|(0<<E);
	PORTA = lcd_FunctionReset; //b3 antal rader, b2 antal punkter, 8 eller 11  
	_delay_us(80);
	//Display on
	PORTD = (0<<RS)|(0<<E);
	PORTA = lcd_DisplayOn; //b2 display, b1 cursor, b0 blink
	_delay_us(80);
	//Display clear
	PORTD = (0<<RS)|(0<<E);
	PORTA = lcd_Clear;
	_delay_ms(4);
	//Entry Mode Set
	PORTD = (0<<RS)|(0<<E);
	PORTA = lcd_EntryMode;//b1 inc/dec, b0, Entire shift
	_delay_ms(1);
}

// writes a char on current possition
void lcd_write_char(char letter){
	PORTA = letter;  // Lägg ín data på pinnarna in data
	
	PORTD &= 0<<E; // Försäkring om att E är låg innan vi försöker skriva
	
	PORTD |= 1 << RS; // sätt RS
	PORTD |= 1 << E; // Sätt Enable
	
	_delay_us(1); //TSU2	// Vänta på att data ska stabiliseras
	PORTD &= 0 << E;
	PORTD &= 0 << RS;
	_delay_us(1);
}

lcd_write_instruction(uint8_t instruction){
	PORTD &= 0 << E;
	PORTD &= 0 << RS;
	PORTA = instruction; // funkar inte detta prova bit för bit. Gäller isf även utskrift.
}

//Erases data on LCD and write new one.
void lcd_write_string(char string[2][16]){
	lcd_write_instruction(lcd_Clear);
	_delay_us(80);
	
	//lcd_write_instruction(lcd_SetCursor | lcd_LineOne);
	//_delay_us(80);	// Behövs troligtvis inte då clear borde 0, om jag inte fattar databladet fel
	
	for(int line = 0; line < 2; line++){
		for(int i = 0; i < NELEMS(string[line]); i++){
			lcd_write_char(string[line][i]); 
			_delay_us(80);
		}
		lcd_write_instruction(lcd_SetCursor | lcd_LineTwo);
		_delay_us(80); 
	}
}



