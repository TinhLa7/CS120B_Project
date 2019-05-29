#pragma once
#include <avr/eeprom.h>
#include "defines.h"
#include "pwm.c"

uint16_t EEMEM ADDRESS;

const unssh INVINCIBLE = 248;
const unssh DESTROYABLE_ENEMY = 244;
const unssh PROJECTILE_SPRITE = 45;

unsch player;
unsch en_move_count, enMoveMult = 7;
unsch lTime, mTime = 0;
unssh sTime, gTime;
unsch spawnTopLimit = 0; 
unsch spawnBottomLimit = 0;
unsch projectileExists = 0;

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

void drawProjectile() {
	LCD_Cursor(projectileObject.drawPosition);
	LCD_WriteData(projectileObject.type);
}

void drawEnemies() {
	for (unsch i = 0; i < total_en; i++) {
		LCD_Cursor(en[i].drawPosition);
		LCD_WriteData(en[i].type);
	}
}

void refreshScreen() {
	LCD_ClearScreen();
	LCD_DisplayString_NoClear(32, (const unsch *)(" "));	
	drawPlayer();
	drawProjectile();
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
		if(i % 2 == 0)
			en[i].type = INVINCIBLE;
		else
			en[i].type = DESTROYABLE_ENEMY;
		en[i].drawPosition = 0;
	}
}

void initProjectiles(){
	projectileObject.drawPosition = 0;
	projectileObject.type = PROJECTILE_SPRITE;
}

int JoystickActions(int state);
int projectile_fct(int state);
int TickFct_LCD_Output(int state);
int enemy_fct(int state);