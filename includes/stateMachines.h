#pragma once
#include "defines.h"
#include "main.h"

enum Joystick_States { Init, check_b1, J_wait, J_up, J_down, J_left, J_right, J_end } Joystick;
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
		else if (coords[player_start] > JOYSTICK_INIT + SHIFT) {
			if (player <= player_limits) {
				player = player + player_limits;
				state = J_down;
			}
			else state = J_wait;
		}
		else if (coords[0] < JOYSTICK_INIT - SHIFT) {
			if (player > player_start && player != player_above_limit) {
				player--;
				state = J_left;
			}
			else state = J_wait;
		}
		else if (coords[0] > JOYSTICK_INIT + SHIFT) {
			if (player < player_limits || (player > player_limits && player < player_upper_limit)) {
				player++;
				state = J_right;
			}
			else state = J_wait;
		}
		else state = J_wait;
		for (unsch i = 0; i < total_en; i++) {
			if (player == en[i].drawPosition) {
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
			if(eeprom_read_word( &ADDRESS) < sTime){
				eeprom_update_word (&ADDRESS , (uint16_t)sTime );
				special_song();
			} 
			else { set_PWM(100.00); } 
				
			//test
			/*if(sTime >= 50)
				{
					special_song();
				}*/
		break;
		default: break;
	} // State actions
	return state;
}

enum projectile { projectile_Init, projectile_release, projectile_exist, projectile_move, projectile_fade } projectile_states;
int projectile_fct(int state) {
	switch(state) { // Transitions
		case projectile_Init:
		if(b1) state = projectile_exist;
		else state = projectile_Init;
		break;
		case projectile_release:
		if(b1) state = projectile_release;
		else state = projectile_Init;
		break;
		case projectile_exist:
		if (b2 || projectileExists) state = projectile_move;
		else state = projectile_exist;
		break;
		case projectile_move: state = projectile_exist; break;
		case projectile_fade:
		if(b1) state = check_b1;
		else state = J_end;
		break;
		default: break;
	} // Transitions
	
	switch(state) { // State actions
		case projectile_Init: break;
		case projectile_release: break;
		case projectile_exist: break;
		case projectile_move:
		if(projectileObject.drawPosition == 0) {
			set_PWM(500.00);
			projectileExists = 1;
			projectileObject.drawPosition = player + 1;
		}
		else if (projectileObject.drawPosition != 0) {
			if (projectileObject.drawPosition != 16 && projectileObject.drawPosition != 32 && projectileExists)
			projectileObject.drawPosition++;
			else {
				projectileObject.drawPosition = 0;
				projectileExists = 0;
			}
			for (unsch i = 0; i < total_en; i++) {
				if (projectileObject.drawPosition == en[i].drawPosition && en[i].image == INVICIBLE_ENEMY) {
					projectileObject.drawPosition = 0;
					projectileExists = 0;
					break;
				}
				if (projectileObject.drawPosition == en[i].drawPosition && en[i].image == DESTROYABLE_ENEMY) {
					en[i].drawPosition = 0;
					projectileObject.drawPosition = 0;
					projectileExists = 0;
					break;
				}
			}
		}
		break;
		case projectile_fade: break;
		default: break;
	} // State actions
	projectile_states = state;
	return state;
}

enum Screen { TitleScreen, TitleScreenWait, GameMenu, GameMenuWait, screenRefresh, game_over1, game_over2, reset_score } screen_state;
int TickFct_LCD_Output(int state) {
	switch(state) { // Transitions
		case TitleScreen:
		sTime = 0;
		if(b1 && mTime < MENU_REFRESH_TIME) {
			mTime = 0;
			state = screenRefresh;
		}
		else if (!b1 && mTime < MENU_REFRESH_TIME)
		state = TitleScreen;
		else if (b3 && mTime < MENU_REFRESH_TIME)
		state = reset_score;
		else if (!b1 && mTime == MENU_REFRESH_TIME) {
			mTime = 0;
			state = GameMenu;
		}
		break;
		case GameMenu:
		if(b1 && mTime < MENU_REFRESH_TIME) {
			mTime = 0;
			state = screenRefresh;
		}
		else if (b3 && mTime < MENU_REFRESH_TIME)
		state = reset_score;
		else if (!b1 && mTime < MENU_REFRESH_TIME)
		state = GameMenu;
		else if (!b1 && mTime == MENU_REFRESH_TIME) {
			mTime = 0;
			state = TitleScreen;
		}
		break;
		case screenButton:
		if(b1) state = screenButton;
		else state = TitleScreen;
		break;
		case screenRefresh:
		for (unsch i = 0; i < total_en; i++) {
			if(player == en[i].drawPosition) {
				if(eeprom_read_word( &ADDRESS) < (uint16_t)sTime)
				// test
				/*if(sTime >= 50)*/ {state = game_over2;}
				else {state = game_over1;}
			}
		}
		break;
		case game_over1:
		if(b1) state = screenButton;
		else state = game_over1;
		break;
		case game_over2:
		if(b1) state = screenButton;
		else state = game_over2;
		break;
		case reset_score:
		state = TitleScreen;
		break;
		default: state = TitleScreen; break;
	} // Transitions

	switch(state) { // State actions
		case TitleScreen:
		gTime++;
		mTime++;
		titleScreen();
		break;
		case GameMenu:
		gTime++;
		mTime++;
		menuScreen();
		break;
		case screenButton:
		break;
		case screenRefresh:
		sTime++;
		gTime++;
		updateJoyStick();
		refreshScreen();
		break;
		case game_over1:
		gameOverScreen1();
		break;
		case game_over2:
			eeprom_update_word (&ADDRESS , (uint16_t)sTime );
			gameOverScreen2();
		break;
		case reset_score:
			eeprom_update_word (&ADDRESS , (uint16_t)sTime );
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
		en_move_count = 10;
		if(b1) {
			initEnemies();
			initProjectiles();
			srand(gTime);
			state = en_refresh;
		}
		else state = en_spawn; break;
		case en_nothing:
		if(b1) state = en_nothing;
		else state = en_spawn;
		break;
		case en_refresh:
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
		break;
		case en_nothing: break;
		case en_move:
		if (sTime % enMoveMult == 0 && en_move_count > 1) {en_move_count--;}
		
		for(unsch i = 0; i < total_en; i++) {
			if ((en[i].drawPosition > 1 && en[i].drawPosition < 18) || (en[i].drawPosition >  17 && en[i].drawPosition <= 33)) en[i].drawPosition--;
			else en[i].drawPosition = 0;
		}
		
		for(unsch i = 0; i < total_en; i++) {
			if (!spawnBottomLimit && (en[i].drawPosition == 31 || en[i].drawPosition == 32 || en[i].drawPosition == 33 || en[i].drawPosition == 16 || en[i].drawPosition == 17)) { spawnBottomLimit = 1;}
			if (!spawnTopLimit && (en[i].drawPosition == 15 || en[i].drawPosition == 16 || en[i].drawPosition == 17 || en[i].drawPosition == 32 || en[i].drawPosition == 33)) {spawnTopLimit = 1;}
		}
		for(unsch i = 0; i < total_en && rand() % 2; i++) {
			if (en[i].drawPosition == 0 && spawnBottomLimit == 0) {en[i].drawPosition = 33;}
			if (en[i].drawPosition == 0 && spawnTopLimit == 0) {en[i].drawPosition = 17;}
		}
		spawnBottomLimit = spawnTopLimit = 0;
		break;
		case en_dead: break;
		default:break;
	} // State actions
	enemy = state;
	return state;
}