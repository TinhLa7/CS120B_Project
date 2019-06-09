#pragma once
#include <avr/eeprom.h>
#include "defines.h"
#include "pwm.c"
#include "not_player.h"

uint16_t EEMEM ADDRESS;

unsigned char playerChar[] = {
  0x0F,
  0x0F,
  0x0F,
  0x14,
  0x0F,
  0x04,
  0x0A,
  0x11
};

unsigned char customChar1[] = {
  0x00,
  0x00,
  0x00,
  0x11,
  0x0A,
  0x04,
  0x00,
  0x00
};

unsigned char customChar2[] = {
  0x04,
  0x0E,
  0x1F,
  0x0E,
  0x0E,
  0x1F,
  0x0E,
  0x0E
};

void LCD_Custom_Char (unsigned char *msg, const char loc)
{
	unsigned char i;
	if(loc<8)
	{
		LCD_WriteCommand (0x40 + (loc*8));	/* Command 0x40 and onwards forces the device to point CGRAM address */
		for(i=0;i<8;i++)	/* Write 8 byte for generation of 1 character */
		LCD_WriteData(msg[i]);
	}
}

unsch player, en_move_count, enMoveMult = 5, lTime, mTime = 0, spawnTopLimit = 0, spawnBottomLimit = 0;
unssh sTime, gTime = 0;
not_player ob[total_en];

void drawPlayer() {
	LCD_Cursor(player);
	LCD_WriteData(5);
}

void titleScreen() {
	LCD_ClearScreen();
	LCD_Cursor(1);
	LCD_WriteData(5);
	LCD_DisplayStringNoClear(2, (const unsch *)("***** LCD *****"));
	LCD_DisplayStringNoClear(17, (const unsch *)("***** RACER ****"));
	player = player_start;
	drawPlayer();
}

void menuScreen() {
	unsch temp_array[4];
	LCD_ClearScreen();
	LCD_Cursor(1);
	LCD_WriteData(5);
	LCD_DisplayStringNoClear(2, (const unsch *)("~Start to Begin"));
	LCD_DisplayStringNoClear(17, (const unsch *)("Hi Score: "));
	LCD_DisplayStringNoClear(26, To_String(eeprom_read_word(&ADDRESS), temp_array, 4));
	player = player_start;
	drawPlayer();
}

void refreshEnemies(){
	for(unsch i = 0; i < total_en; i++) {
		if(i % 2 == 0){ ob[i].type = 1; }
		else { ob[i].type = 2; }
	}
}

void drawEnemies() {
	refreshEnemies();
	for (unsch i = 0; i < total_en; i++) {
		LCD_Cursor(ob[i].drawPosition);
		if(ob[i].type == 2){ LCD_WriteData(6); }
		else if(ob[i].type == 1){ LCD_WriteData(7); }
	}
}

void refreshScreen() {
	LCD_ClearScreen();
	LCD_DisplayStringNoClear(32, (const unsch *)(" "));	
	drawPlayer();
	drawEnemies();	
}

void gameOverScreen1() {
	unsch array[4];
	
	LCD_ClearScreen();
	LCD_DisplayStringNoClear(1, (const unsch *)(" **GAME OVER**"));
	LCD_DisplayStringNoClear(17, (const unsch *)("~~~SCORE = "));
	LCD_DisplayStringNoClear(28, To_String(sTime, array, 4));
	set_PWM(100.00);
}

void gameOverScreen2() {
	unsch array[4];
	
	LCD_ClearScreen();
	LCD_DisplayStringNoClear(1, (const unsch *)("Congratulations!"));
	LCD_DisplayStringNoClear(17, (const unsch *)("~Hi SCORE: "));
	LCD_DisplayStringNoClear(28, To_String(sTime, array, 4));
	set_PWM(261.63);
}

void gameOverScreen3() {
	LCD_ClearScreen();
	LCD_DisplayStringNoClear(1, (const unsch *)("Congratulations!"));
	LCD_DisplayStringNoClear(17, (const unsch *)("U won the race!"));
}

void initEnemies() {
	for(unsch i = 0; i < total_en; i++) {
		if(i % 2 == 0){ ob[i].type = 1; }
		else { ob[i].type = 2; }
		ob[i].drawPosition = 0;
	}
}

int JoystickActions(int state);
int projectile_fct(int state);
int TickFct_LCD_Output(int state);
int enemy_fct(int state);