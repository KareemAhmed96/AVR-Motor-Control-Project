/*
 * Timer_PWM.h
 *
 *  Created on: May 2, 2019
 *      Author: Kareem
 */

#ifndef TIMER_PWM_H_
#define TIMER_PWM_H_

#include "micro_config.h"
#include "std_types.h"
#include "common_macros.h"

#define PWM_PORT    DDRB
#define PWM_PIN     3

void PWM_init(void);

#endif /* TIMER_PWM_H_ */
