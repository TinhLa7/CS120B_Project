/*
 * LCD_Racer - CS120B
 * UCR Spring 2019
 *
 * Created: 5/21/2019 7:30:45 PM
 * Author : tinhl
 */ 

#include <avr/io.h>
#include <avr/eeprom.h>
#include "defines.h"
#include "io.c"
#include "not_player.h"
#include "stateMachines.h"
#include "joystick.c"

//sprites
const unssh INVICIBLE_ENEMY = 248;
const unssh DESTROYABLE_ENEMY = 244;
const unssh PROJECTILE_SPRITE = 45;

unsch player;
unsch en_move_count, enMoveMult = 2;
unsch lTime, mTime = 0;
unssh sTime, gTime;
unsch spawnTopLimit = 0;
unsch spawnBottomLimit = 0;
unsch projectileExists = 0;

not_player en[total_en];
not_player projectileObject;

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
	LCD_DisplayString_NoClear(26, LCD_To_String(H_SCORE, temp_array, 4));
}

void drawPlayer(){
	LCD_Cursor(player);
	LCD_WriteData('}');
}

void drawProjectile(){
	LCD_Cursor(projectileObject.drawPosition);
	LCD_WriteData(projectileObject.image);
}

void drawEnemies(){
	for (unsch i = 0; i < total_en; i++) {
		LCD_Cursor(en[i].drawPosition);
		LCD_WriteData(en[i].image);
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
}

void gameOverScreen2() {
	unsch temp_array[6];
	
	LCD_ClearScreen();
	LCD_DisplayString_NoClear(1, (const unsch *)("Congratulations!"));
	LCD_DisplayString_NoClear(17, (const unsch *)("Hi SCORE: "));
	LCD_DisplayString_NoClear(27, LCD_To_String(sTime, temp_array, 6));
}

int main(void)
{
    DDRA = 0x00; PORTA = 0xFF; 
	DDRB = 0xFF; PORTB = 0x40;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00; 
    while (1) 
    {
    }
}

