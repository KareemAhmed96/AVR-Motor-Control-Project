/*
 * Application.c
 *
 *  Created on: May 2, 2019
 *      Author: Kareem
 */

#include "Timer_PWM.h"
#include "ADC.h"
#include "LCD.h"

#define ADC_PRECISION 1023

uint8 g_Current_State = 1;
uint8 g_Temp;
uint8 g_Previous_State = 0;

void INT0_init(void)
{
	DDRD &= (~(1<<2));    //External Interrupt pin as input
	GICR  |= (1<<INT0);   // Enable external interrupt pin INT0

	// Trigger INT0 with the falling edge
	MCUCR |= (1<<ISC01);
	MCUCR &= ~(1<<ISC00);

	// Global Interrupts Enable
	SREG  |= (1<<7);
}

int main(void)
{
	uint16 motorPercentage;

	//ADC Configuration Structure Definition
	ADC_Config_Type Config_Ptr = {AVCC, RIGHT_ADJUST, DIV_BY_8, EXTENAL_INTERRUPT_REQUEST_0};
	
	//ADC Channel Select Union Definition
	ADC_Analog_Channel_Type Channel_Config_Ptr = {ADC1};

	ADC_init(&Config_Ptr);
	LCD_init();
	PWM_init();
	INT0_init();

	LCD_sendCommand(0x01); //Clear Command
	LCD_displayString("SPEED = ");

	DDRB |= (1<<0) | (1<<1); //Output pins connected to motor driver

	for( ; ; )
	{
		if(g_Current_State == 1)
		{  	
			// Rotation direction controlled by external interrupt connected to a push button
			PORTB |= (1<<0);
			PORTB &= (~(1<<1));
		}
		else
		{
			PORTB |= (1<<1);
			PORTB &= (~(1<<0));
		}

		ADC_readChannel_Interrupts(&Channel_Config_Ptr);

		motorPercentage =  ((float)(g_result)/(float)(ADC_PRECISION)) * (100); // 0% -> 100%

		OCR0 = (g_result) / (4UL); // 0-1023 -> 0-255

		//Spliting each digit
		if(motorPercentage == 100)
		{
			LCD_goToRowColumn(0,8);
			LCD_intgerToString(motorPercentage / 100 ); //First Digit
			LCD_goToRowColumn(0,9);
			LCD_intgerToString((motorPercentage % 100) / 10); //Second Digit
			LCD_goToRowColumn(0,10);
			LCD_intgerToString(motorPercentage % 10); //Third Digit
			LCD_goToRowColumn(0,11);
			LCD_displayCharacter('%'); //Percentage Sign
		}
		else if( (motorPercentage < 100) && (motorPercentage > 10) )
		{
			LCD_goToRowColumn(0,8);
			LCD_intgerToString((motorPercentage % 100) / 10);
			LCD_goToRowColumn(0,9);
			LCD_intgerToString(motorPercentage % 10); //Second Digit
			LCD_goToRowColumn(0,10);
			LCD_displayCharacter('%'); //Percentage Sign
			LCD_goToRowColumn(0,11);
			LCD_displayCharacter(' '); //Blank Space
		}
		else if(motorPercentage < 10)
		{
			LCD_goToRowColumn(0,8);
			LCD_intgerToString(motorPercentage % 10); //First Digit
			LCD_goToRowColumn(0,9);
			LCD_displayCharacter('%'); //Percentage Sign
			LCD_goToRowColumn(0,10);
			LCD_displayCharacter(' '); //Blank Space
			LCD_goToRowColumn(0,11);
			LCD_displayCharacter(' '); //Blank Space
		}
	}
	return 0;
}

ISR(INT0_vect)
{
	// Swapping
	g_Temp = g_Current_State;
	g_Current_State = g_Previous_State;
	g_Previous_State = g_Temp;
}
