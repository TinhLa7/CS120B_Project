#pragma once
#include "defines.h"

//task struct

typedef struct task {
	int state;
	unsl period;
	unsl elapsedTime;
	int (*TickFct)(int);
} task;

void TimerOn();
void TimerOff();
void TimerSet(unsl M);
void TimerISR();