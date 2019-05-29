#pragma once
#include "defines.h"

void LCD_init();
void LCD_ClearScreen(void);
void LCD_WriteData(unsch Data);
void LCD_WriteCommand (unsch Command);
void LCD_Cursor (unsch column);
void LCD_DisplayString(unsch column, const unsch *string);
void LCD_DisplayString_NoClear(unsch column, const unsch *string);
void delay_ms(int miliSec);

unsch * LCD_To_String(uint16_t num, unsch * string, unsch size);

