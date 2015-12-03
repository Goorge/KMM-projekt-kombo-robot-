#ifndef LCD
#define LCD

#include <asf.h>

void lcd_setup(void);
void lcd_write_char(uint8_t letter);
void lcd_write_instruction(uint8_t instruction);
void lcd_write_string(uint8_t text[], uint8_t row_instrucion);

#endif