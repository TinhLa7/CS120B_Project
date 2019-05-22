/*
 * LCD_Racer - CS120B
 * UCR Spring 2019
 *
 * Created: 5/21/2019 7:30:45 PM
 * Author : tinhl
 */ 

#define unsch unsigned char
#define unssh unsigned short
#define unsl unsigned long

#include <avr/io.h>


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

