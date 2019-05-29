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

