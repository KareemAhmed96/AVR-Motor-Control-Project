/*
 * Timer_PWM.c
 *
 *  Created on: May 2, 2019
 *      Author: Kareem
 */
#include "Timer_PWM.h"

void PWM_init(void){
			//Fast PWM
	TCCR0 = (1<<WGM00) | (1<<WGM01) \
			//Clear OC0 on compare match, set OC0 at BOTTOM (Non-inverting mode)
			| (1<<COM01) \
			//Clock select
			| (1<<CS01);

	//Timer-Counter Register
	TCNT0 = 0;

	//Output Pin that will produce PWM signal
	(PWM_PORT) |= 1 << (PWM_PIN);
}


