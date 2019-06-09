/*
 * LCD_Racer_Version2.c
 *
 * Created: 6/1/2019 12:33:58 PM
 * Author : tinhl
 */ 

#include <avr/io.h>
#include <io.c>
#include <joystick.c>
#include <defines.h>
#include <stdlib.h>
#include "pwm.c"
#include "not_player.h"
#include "main.h"
#include "timer.c"
#include "scheduler.h"

unsch player;
unsch up = 0;
unsch up_cnt = 0;
unsch start = 1;
unsch playerJumpLimit = 10;
unsch speedDecrementer = 0;

enum Joystick { Init, check_b1, J_wait, J_up, J_down, J_left, J_right, J_end } Joystick;
enum Screen { ScreenInit, TitleScreen, TitleScreenWait, GameMenu, GameMenuWait, screenButton, screenRefresh, game_over1, game_over1Wait, game_over2, game_over2Wait, reset_score, reset_game, beatHScore, beatHScoreWait } screen_state;
enum obstacle { obstacle_spawn, obstacle_nothing, obstacle_refresh, obstacle_move, obstacle_kill_player } enemy;
	
void restartGame(){
	start = 1;
	player = player_start;
	playerJumpLimit = 10;
	en_move_count = 8, enMoveMult = 5;
	lTime = mTime = sTime = gTime = 0;
	spawnTopLimit = 0;
	spawnBottomLimit = 0;
	for(unsch j = 0; j < total_en; j++)	{ ob[j].drawPosition = 0; }
}	
	
int JoystickActions(int state) {
	switch(state) { // Transitions
		case Init:
			if(player != player_start){
				player = player_start;
				drawPlayer();
			}
			if(b1) { state = J_wait; }
			else { state = Init; }
		break;
		case check_b1:
			if(b1) { restartGame(); }
			else 
			{ state = Init; }
		break;
		case J_wait:
			if(playerJumpLimit > 0){ 
				if(speedDecrementer) { playerJumpLimit -= speedDecrementer; }
				speedDecrementer = 0;
			}
			if (position[player_start] < JOYSTICK_INIT - SHIFT) {
				if (player >= player_above_limit) {
					player = player - player_limits;
					state = J_up;
					up = 1;
				}
				else { state = J_wait; }
			}
			else if (position[0] < JOYSTICK_INIT - SHIFT) {
				if (player > player_start and player != player_above_limit) {
					player--;
					state = J_left;
				}
				else { state = J_wait; }
			}
			else if (position[0] > JOYSTICK_INIT + SHIFT) {
				if (player < player_limits or (player > player_limits and player < player_upper_limit)) {
					player++;
					state = J_right;
				}
				else { state = J_wait; }
			}
			else if (up_cnt == playerJumpLimit) {
				if (player <= player_limits) {
					player = player + player_limits;
					state = J_down;
					up = 0;
				}
				else { state = J_wait; }
				up_cnt = 0;
			}
			else if(up) {
				state = J_wait;
				up_cnt++;
			}
			else if(start == 1) {
				if (player <= player_limits) {
					player = player + player_limits;
					state = J_down;
					up = 0;
					start = 0;
				}
			}
			else { state = J_wait; }
			for (unsch i = 0; i < total_en; i++) {
				if (player == ob[i].drawPosition) {
					state = J_end;
					break;
				}
				}
		break;
		case J_up: state = J_wait; break;
		case J_down: state = J_wait; break;
		case J_left: state = J_wait; break;
		case J_right: state = J_wait; break;
		case J_end:
			if(b1) { state = check_b1; }
			else { state = J_end; }
		break;
		default: state = J_wait; break;
	} // Transitions

	switch(state) { // State actions
		case Init: set_PWM(0); break;
		case J_wait: set_PWM(0); break;
		case check_b1: set_PWM(400.00); break;
		case J_up: set_PWM(523.25); break;
		case J_down: set_PWM(261.63); break;
		case J_left: set_PWM(329.63); break;
		case J_right: set_PWM(392.00); break;
		case J_end:	break;
		default: break;
	} // State actions
	return state;
}

int TickFct_LCD_Output(int state) {
	switch(state) { // Transitions
		case ScreenInit: state = TitleScreen; break;
		case TitleScreen:
			sTime = 0;
			state = TitleScreenWait;
		break;
		case TitleScreenWait:
			if(b1){state = GameMenu;}
			else {state = TitleScreenWait;}	
		break;
		case GameMenu: state = GameMenuWait; break;
		case GameMenuWait:
			if (b3 and mTime < MENU_REFRESH_TIME) { state = reset_score; }
			else if (!b1 and mTime < MENU_REFRESH_TIME) { state = GameMenuWait; }
			else if (!b1 and mTime == MENU_REFRESH_TIME) {
				mTime = 0;
				state = TitleScreen;
			}
			else if(b1 and mTime < MENU_REFRESH_TIME) {
				mTime = 0;
				state = screenRefresh;
			}
		break;
		case screenButton:
			if(b1) { state = screenButton; }
			else { state = TitleScreen; }
		break;
		case screenRefresh:
			for (unsch i = 0; i < total_en; i++) {
				if(player == ob[i].drawPosition) {
					if(HighScore < (uint16_t)sTime) { state = game_over2; }
					else { state = game_over1; }
				}
			}
			if(sTime > 999) { state = beatHScore; }
		break;
		case game_over1: state = game_over1Wait; break;
		case game_over1Wait:
			if(b1) { state = reset_game; } 
			else { state = game_over1Wait; }
		break;
		case game_over2: state = game_over2Wait; break;
		case game_over2Wait:
			if(b1) { state = reset_game; }
			else { state = game_over2Wait; }
		break;
		case reset_score: state = ScreenInit; break;
		case reset_game: state = screenButton; break;
		case beatHScore: state = beatHScoreWait; break;
		case beatHScoreWait:
		if(b1) { state = reset_game; }
		else { state = beatHScoreWait; }
		default: state = TitleScreen; break;
	} // Transitions

	switch(state) { // State actions
		case ScreenInit: break;
		case TitleScreen:
			gameTime;
			titleScreen();
		break;
		case TitleScreenWait: break;
		case GameMenu:
			gameTime;
			menuScreen();
		break;
		case GameMenuWait: 
			++mTime;
		break;
		case screenButton: break;
		case screenRefresh:
			score1;
			gTime++;
			JoyStick();
			refreshScreen();
		break;
		case game_over1: gameOverScreen1(); break;
		case game_over1Wait: break;
		case game_over2: 
			eeprom_update_word (&ADDRESS , (uint16_t)sTime );
			gameOverScreen2();
		break;
		case game_over2Wait: break;
		case reset_score:
			eeprom_update_word (&ADDRESS , (uint16_t)sTime );
		break;
		case reset_game: restartGame(); break;
		case beatHScore: gameOverScreen3(); 
			eeprom_update_word (&ADDRESS , (uint16_t)999 );
		break;
		default: break;
	} // State actions
	screen_state = state;
	return state;
}

int enemy_fct(int state) {
	switch(state) { // Transitions
		case obstacle_spawn:
			enemyTime = 0;
			en_move_count = 8; 
			if(b1) {
				initEnemies();
				srand(gTime);
				state = obstacle_refresh;
			}
			else state = obstacle_spawn; break;
		case obstacle_nothing:
			if(b1) { state = obstacle_nothing; }
			else { state = obstacle_spawn; }
		break;
		case obstacle_refresh: 
			if (lTime < en_move_count) { state = obstacle_refresh; }
			else if (lTime == en_move_count) {
				enemyTime = 0;
				state = obstacle_move;
				}
			for (unsch i = 0; i < total_en; i++) {
				if (player == ob[i].drawPosition) {
					state = obstacle_kill_player;
					break;
				}
			}
		break;
		case obstacle_move: state = obstacle_refresh; break;
		case obstacle_kill_player:
			if(b1) { state = obstacle_nothing; }
			else { state = obstacle_kill_player; }
		break;
		default: state = obstacle_spawn; break;
	} // Transitions

	switch(state) { // State actions
		case obstacle_refresh:
			lTime++;
			refreshEnemies();
		break;
		case obstacle_nothing: break;
		case obstacle_move:
			if (speedUp) 
			{
				en_move_count--;
				speedDecrementer = 1;
			}
			
			for(unsch i = 0; i < total_en; i++) {
				if ((ob[i].drawPosition > 1 and ob[i].drawPosition < 18 and ob[i].type == 2) or (ob[i].drawPosition >  17 and ob[i].drawPosition <= 35 and ob[i].type == 1)) ob[i].drawPosition--;
				else ob[i].drawPosition = 0;
				if (!spawnBottomLimit and (ob[i].drawPosition == 31 or ob[i].drawPosition == 32 or ob[i].drawPosition == 35 or ob[i].drawPosition == 16 or ob[i].drawPosition == 17)) { spawnBottomLimit = 1;}
				if (!spawnTopLimit and (ob[i].drawPosition == 15 or ob[i].drawPosition == 16 or ob[i].drawPosition == 17 or ob[i].drawPosition == 32 or ob[i].drawPosition == 35)) {spawnTopLimit = 1;}
			
				if(rand() % 2) {
					if (ob[i].drawPosition == 0 and spawnBottomLimit == 0 and ob[i].type == 1 and i % 2 == 0) { ob[i].drawPosition = 35; }
					if (ob[i].drawPosition == 0 and spawnTopLimit == 0 and ob[i].type == 2) { ob[i].drawPosition = 16; }					
				}
			}
			
			for(unsch i = 0; i < total_en; i += 2){
				for(unsch j = 1; j < total_en; j += 2){
					if( i != j){
						if(ob[i].drawPosition - player_limits == ob[j].drawPosition){ ob[j].drawPosition = 0; }
						if((ob[i].drawPosition - player_limits) - 1 == ob[j].drawPosition){ ob[j].drawPosition = 0; }
						if((ob[i].drawPosition - player_limits) + 1 == ob[j].drawPosition){ ob[j].drawPosition = 0; }
					}
				}
			}
			spawnBottomLimit = spawnTopLimit = 0;
		break;
		case obstacle_kill_player: break;
		default:break;
	} // State actions
	enemy = state;
	return state;
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x40;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
    LCD_init();
	LCD_WriteCommand(0x0C);
    ADC_init();
	PWM_on();
	LCD_Custom_Char(playerChar,5);
	LCD_Custom_Char(customChar1,6);
	LCD_Custom_Char(customChar2,7);
	gTime = 0;
	Joystick = Init;
	screen_state = ScreenInit;
	PORTB = PORTC = 0;
	unsch i=0;
	tasks[i].state = Init;
	tasks[i].period = periodJoystick;
	tasks[i].elapsedTime = tasks[i].period;
	tasks[i].TickFct = &JoystickActions;
	
	++i;
	tasks[i].state = obstacle_spawn;
	tasks[i].period = periodEnemy_Generator;
	tasks[i].elapsedTime = tasks[i].period;
	tasks[i].TickFct = &enemy_fct;

	++i;
	tasks[i].state = ScreenInit;
	tasks[i].period = periodLCD_Output;
	tasks[i].elapsedTime = tasks[i].period;
	tasks[i].TickFct = &TickFct_LCD_Output;
	
	TimerSet(tasksPeriodGCD);
	TimerOn();
    while (1) {}
}

