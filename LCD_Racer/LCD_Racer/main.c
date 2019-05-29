/*
 * LCD_Racer - CS120B
 * UCR Spring 2019
 *
 * Created: 5/21/2019 7:30:45 PM
 * Author : tinhl
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "io.c"
#include "joystick.c"
#include "pwm.c"
#include "timer.c"
#include "not_player.h"
#include "defines.h"
#include "main.h"
#include "scheduler.h"
#include "stateMachines.h"

int main() {
	DDRA = 0x00; PORTA = 0xFF; 
	DDRB = 0xFF; PORTB = 0x40; 
	DDRC = 0xFF; PORTC = 0x00; 
	DDRD = 0xFF; PORTD = 0x00; 
	
	LCD_init();
	ADC_init();
	PWM_on();
	
	gTime = 0;

	Joystick = Init; 
	screen_state = Screen_Init;
	enemy = en_spawn;
	projectile_states = projectile_Init;
	
	PORTB = PORTC = 0; 
	
	unsch i=0;
	tasks[i].state = Init;
	tasks[i].period = periodJoystick;
	tasks[i].elapsedTime = tasks[i].period;
	tasks[i].TickFct = &JoystickActions;

	++i;
	tasks[i].state = en_spawn;
	tasks[i].period = periodEnemy_Generator;
	tasks[i].elapsedTime = tasks[i].period;
	tasks[i].TickFct = &enemy_fct;
	
	++i;
	tasks[i].state = projectile_Init;
	tasks[i].period = periodShoot;
	tasks[i].elapsedTime = tasks[i].period;
	tasks[i].TickFct = &projectile_fct;

	++i;
	tasks[i].state = Screen_Init;
	tasks[i].period = periodLCD_Output;
	tasks[i].elapsedTime = tasks[i].period;
	tasks[i].TickFct = &TickFct_LCD_Output;
	
	TimerSet(tasksPeriodGCD);
	TimerOn();

	while(1) {} 
	
	return 0;
}
