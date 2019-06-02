#pragma once
#include <avr/eeprom.h>
#include "defines.h"
#include "pwm.c"
#include "not_player.h"

uint16_t EEMEM ADDRESS;

const unssh TREE = 200;
const unssh BIRD = 252;

unsch player;
unsch en_move_count, enMoveMult = 5; // enMoveMult = 5;
unsch lTime, mTime = 0;
unssh sTime, gTime = 0;
unsch spawnTopLimit = 0; 
unsch spawnBottomLimit = 0;

not_player en[total_en];
not_player projectileObject;

void special_song(){
	static unsch i = 0;
	set_PWM(261.63);
	while(i < 100){
		++i;
	}
	i = 0;
	set_PWM(329.63);
	while(i < 100){
		++i;
	}
	i = 0;
	set_PWM(392.00);
	while(i < 100){
		++i;
	}
	i = 0;
	set_PWM(523.25);
	while(i < 100){
		++i;
	}
	i = 0;	
	set_PWM(0);	
}

void titleScreen() {
	LCD_ClearScreen();
	LCD_DisplayString_NoClear(1, (const unsch *)("****** LCD *****"));
	LCD_DisplayString_NoClear(17, (const unsch *)("***** RACER ****"));
}

void menuScreen() {
	unsch temp_array[4];
	LCD_ClearScreen();
	LCD_DisplayString_NoClear(1, (const unsch *)("Middle to Start"));
	LCD_DisplayString_NoClear(17, (const unsch *)("Hi Score: "));
	//LCD_DisplayString_NoClear(17, (const unsch *)("Hi Score: TEST"));
	LCD_DisplayString_NoClear(26, LCD_To_String(eeprom_read_word(&ADDRESS), temp_array, 4));
}

void drawPlayer() {
	LCD_Cursor(player);
	LCD_WriteData('}');
}

void refreshEnemies(){
	for(unsch i = 0; i < total_en; i++) {
		if(i % 2 == 0){
			en[i].image = TREE;
			en[i].type = 1;
		}
		else {
			en[i].image = BIRD;
			en[i].type = 2;
		}
	}
}

void drawEnemies() {
	refreshEnemies();
	for (unsch i = 0; i < total_en; i++) {
		LCD_Cursor(en[i].drawPosition);
		LCD_WriteData(en[i].image);
	}
}

void refreshScreen() {
	LCD_ClearScreen();
	LCD_DisplayString_NoClear(32, (const unsch *)(" "));	
	drawPlayer();
	drawEnemies();	
}

void gameOverScreen1() {
	unsch temp_array[6];
	
	LCD_ClearScreen();
	LCD_DisplayString_NoClear(1, (const unsch *)("**GAME OVER**"));
	LCD_DisplayString_NoClear(17, (const unsch *)("SCORE: "));
	LCD_DisplayString_NoClear(24, LCD_To_String(sTime, temp_array, 6));
	set_PWM(100.00);
}

void gameOverScreen2() {
	unsch temp_array[6];
	
	LCD_ClearScreen();
	LCD_DisplayString_NoClear(1, (const unsch *)("Congratulations!"));
	LCD_DisplayString_NoClear(17, (const unsch *)("Hi SCORE: "));
	LCD_DisplayString_NoClear(27, LCD_To_String(sTime, temp_array, 6));
	set_PWM(261.63);
}

void initEnemies() {
	for(unsch i = 0; i < total_en; i++) {
		if(i % 2 == 0){
			en[i].image = TREE;
			en[i].type = 1;
		}
		else {
			en[i].image = BIRD;
			en[i].type = 2;
		}
		en[i].drawPosition = 0;
	}
}

int JoystickActions(int state);
int projectile_fct(int state);
int TickFct_LCD_Output(int state);
int enemy_fct(int state);