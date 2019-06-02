#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "joystick.h"

void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
}

void wait(long numOP) {
	for( long i = 0; i < numOP; i++){
		asm("nop");
	}
}

void updateJoyStick() {
	unssh x;
	unssh y;
	
	ADMUX = REF_AVCC | 0x00;
	wait(300);
	x = ADC;
	ADMUX = REF_AVCC | 0x01;
	wait(300);
	y = ADC;
	
	coords[0] = x;	
	coords[1] = y;	
}