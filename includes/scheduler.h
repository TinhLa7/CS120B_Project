#pragma once
#include "defines.h"

task tasks[TASKS_NUM];
const unsl periodJoystick = 50;
const unsl periodShoot = 25;
const unsl periodLCD_Output = 50;
const unsl periodEnemy_Generator = 50;
const unsl tasksPeriodGCD = 25;

void TimerISR() {
	unsch i;
	for (i = 0; i < TASKS_NUM; ++i) { 
		if ( tasks[i].elapsedTime >= tasks[i].period ) { 
			tasks[i].state = tasks[i].TickFct(tasks[i].state);
			tasks[i].elapsedTime = 0;
		}
		tasks[i].elapsedTime += tasksPeriodGCD;
	}
}