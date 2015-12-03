#include <avr/interrupt.h>
#include <avr/io.h>
#include "asf.h"

void manual_drive(){
	if(counter < 10){
		if(manual_function == 0){              //stand still
			PORTB = (0 << PB5) | (0 << PB6);
			OCR0A = 0;
			OCR0B = 0;
		}
		else if(manual_function == 1){              //forward
			PORTB &= (0 << PB5) | (0 << PB6);
			OCR0A = 100;
			OCR0B = 100;
		}
		else if(manual_function == 2){          //backward
			PORTB |= (1 << PB5) | (1 << PB6);
			OCR0A = 100;
			OCR0B = 100;
		}
		else if(manual_function == 3){          //rightforward
			PORTB &= (0 << PB5) | (0 << PB6);
			OCR0A = 100;
			OCR0B = 50;
		}
		else if(manual_function == 4){          //leftforward
			PORTB |= (0 << PB5) | (0 << PB6);
			OCR0A = 50;
			OCR0B = 100;
		}
		else if(manual_function == 5){          //spin right
			PORTB &= (0 << PB5);
			PORTB |= (1 << PB6);
			OCR0A = 50;
			OCR0B = 50;
		}
		else if(manual_function == 6){          //spin left
			PORTB &= ~(1 << PB6);
			PORTB |= (1 << PB5);
			OCR0A = 50;
			OCR0B = 50;
		}
		else{
			manual_function=0;
		}
	}
	else{
		PORTB = (0 << PB5) | (0 << PB6);
		OCR0A = 0;
		OCR0B = 0;
	}
	
}