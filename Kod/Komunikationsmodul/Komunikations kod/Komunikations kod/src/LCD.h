#ifndef LCD
#define LCD

#include <asf.h>

void lcd_setup(void);
void lcd_write_char(char letter);
void lcd_write_instruction(uint8_t instruction);
void lcd_write_string(char string[2][16]);

#endif