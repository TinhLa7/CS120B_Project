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
unsch up;

not_player obstacles[total_en];
enum Joystick_States { Init, check_b1, J_wait, J_up, J_down, J_left, J_right, J_end } Joystick;
void resetGame(){
	en_move_count = 10, enMoveMult = 5;
	lTime = mTime = 0;
	sTime = gTime = 0;
	spawnTopLimit = 0;
	spawnBottomLimit = 0;
	Joystick = Init;
}
	
int JoystickActions(int state) {
	switch(state) { // Transitions
		case Init:
		player = player_start;
		if(b1) state = J_wait;
		else state = Init;
		break;
		case check_b1:
		if(b1) state = check_b1;
		else state = Init;
		break;
		case J_wait:
		if (coords[player_start] < JOYSTICK_INIT - SHIFT) {
			if (player >= player_above_limit) {
				player = player - player_limits;
				state = J_up;
			}
			else state = J_wait;
		}
		else if (coords[0] < JOYSTICK_INIT - SHIFT) {
			if (player > player_start and player != player_above_limit) {
				player--;
				state = J_left;
			}
			else state = J_wait;
		}
		else if (coords[0] > JOYSTICK_INIT + SHIFT) {
			if (player < player_limits or (player > player_limits and player < player_upper_limit)) {
				player++;
				state = J_right;
			}
			else state = J_wait;
		}
		else if (up == 3) {
			if (player <= player_limits) {
				player = player + player_limits;
				state = J_down;
				up = 0;
			}
			else state = J_wait;
		}
		else {
			state = J_wait;
			up++;
		} 
		for (unsch i = 0; i < total_en; i++) {
			if (player == obstacles[i].drawPosition) {
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
		if(b1) state = check_b1;
		else state = J_end;
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
		case J_end:
			/*if(eeprom_read_word( &ADDRESS) < sTime){
				//eeprom_update_word (&ADDRESS , (uint16_t)sTime );
				special_song();
			} 
			else { set_PWM(100.00); } 
				
			//test
			if(sTime >= 50)
				{
					special_song();
				}*/
		break;
		default: break;
	} // State actions
	return state;
}

enum Screen { ScreenInit, TitleScreen, TitleScreenWait, GameMenu, GameMenuWait, screenButton, screenRefresh, game_over1, game_over1Wait, game_over2, game_over2Wait, reset_score } screen_state;

int TickFct_LCD_Output(int state) {
	switch(state) { // Transitions
		case ScreenInit:
			state = TitleScreen;
		break;
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
			if(b1) state = screenButton;
			else state = TitleScreen;
		break;
		case screenRefresh:
			for (unsch i = 0; i < total_en; i++) {
				if(player == en[i].drawPosition) {
					if(eeprom_read_word( &ADDRESS) < (uint16_t)sTime) {state = game_over2;}
					else {state = game_over1;}
				}
			}
		break;
		case game_over1: state = game_over1Wait; break;
		case game_over1Wait:
			if(b1) 
			{ 
				resetGame();
				state = screenButton; 
			} 
			else { state = game_over1Wait; }
		break;
		case game_over2: state = game_over2Wait; break;
		case game_over2Wait:
			if(b1) 
			{ 
				resetGame();
				state = screenButton; 
			}
			else { state = game_over2Wait; }
		break;
		case reset_score: state = TitleScreen; break;
		default: state = TitleScreen; break;
	} // Transitions

	switch(state) { // State actions
		case ScreenInit: break;
		case TitleScreen:
			gTime++;
			mTime++;
			titleScreen();
		break;
		case TitleScreenWait: break;
		case GameMenu:
			gTime++;
			mTime++;
			menuScreen();
		break;
		case GameMenuWait: 
			++mTime;
		break;
		case screenButton: break;
		case screenRefresh:
			sTime++;
			gTime++;
			updateJoyStick();
			refreshScreen();
		break;
		case game_over1: gameOverScreen1(); break;
		case game_over1Wait: break;
		case game_over2: 
			//eeprom_update_word (&ADDRESS , (uint16_t)sTime );
			gameOverScreen2();
		break;
		case game_over2Wait: break;
		case reset_score:
			//eeprom_update_word (&ADDRESS , (uint16_t)sTime );
		break;
		default: break;
	} // State actions
	screen_state = state;
	return state;
}

enum en_SM { en_spawn, en_nothing, en_refresh, en_move, en_dead } enemy;

int enemy_fct(int state) {
	switch(state) { // Transitions
		case en_spawn:
			lTime = 0;
			en_move_count = 9; //10
			if(b1) {
				initEnemies();
				srand(gTime);
				state = en_refresh;
			}
			else state = en_spawn; break;
		case en_nothing:
			if(b1) state = en_nothing;
			else state = en_spawn;
		break;
		case en_refresh: 
			/*if(sTime != 0 and sTime % 500 == 0)
			{
				for(unsch i = 0 ; i < total_en ; i++)
				{
					en[i].drawPosition = 0;
				}
				for(unsch i = 0; i < 100; i++)
				{
					
				}
				state = en_spawn;
			}
			else */
			if (lTime < en_move_count) state = en_refresh;
			else if (lTime == en_move_count) {
				lTime = 0;
				state = en_move;
				}
			for (unsch i = 0; i < total_en; i++) {
				if (player == en[i].drawPosition) {
					state = en_dead;
					break;
				}
			}
		break;
		case en_move: state = en_refresh; break;
		case en_dead:
			if(b1) state = en_nothing;
			else state = en_dead;
		break;
		default: state = en_spawn; break;
	} // Transitions

	switch(state) { // State actions
		case en_refresh:
			lTime++;
			refreshEnemies();
		break;
		case en_nothing: break;
		case en_move:
			if (sTime % enMoveMult == 0 and en_move_count > 1) {en_move_count--;}
			
			for(unsch i = 0; i < total_en; i++) {
				if ((en[i].drawPosition > 1 and en[i].drawPosition < 18) or (en[i].drawPosition >  17 and en[i].drawPosition <= 33)) en[i].drawPosition--;
				else en[i].drawPosition = 0;
			}
			
			for(unsch i = 0; i < total_en; i++) {
				if (!spawnBottomLimit and (en[i].drawPosition == 31 or en[i].drawPosition == 32 or en[i].drawPosition == 33 or en[i].drawPosition == 16 or en[i].drawPosition == 17)) { spawnBottomLimit = 1;}
				if (!spawnTopLimit and (en[i].drawPosition == 15 or en[i].drawPosition == 16 or en[i].drawPosition == 17 or en[i].drawPosition == 32 or en[i].drawPosition == 33)) {spawnTopLimit = 1;}
			}
			for(unsch i = 0; i < total_en and rand() % 2; i++) {
				if (en[i].drawPosition == 0 and spawnBottomLimit == 0) {
					en[i].drawPosition = 33;
				}
				if (en[i].drawPosition == 0 and spawnTopLimit == 0) {
					en[i].drawPosition = 17;
				}
				for(unsch j = 0; j < total_en; j++){
					if( i != j){
						if(en[i].drawPosition == en[j].drawPosition){
							if(en[j].drawPosition <= 2)
							en[j].drawPosition = 0;
						}
					}
				}
			}
			spawnBottomLimit = spawnTopLimit = 0;
		break;
		case en_dead: break;
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
    ADC_init();
	PWM_on();
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
	tasks[i].state = ScreenInit;/*TitleScreen*/;
	tasks[i].period = periodLCD_Output;
	tasks[i].elapsedTime = tasks[i].period;
	tasks[i].TickFct = &TickFct_LCD_Output;
	
	TimerSet(tasksPeriodGCD);
	TimerOn();
    while (1) 
    {
		//TimerISR();
    }
}

