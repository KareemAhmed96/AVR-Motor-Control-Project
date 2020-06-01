/*
 * LCD.h
 *
 *  Created on: Apr 19, 2019
 *      Author: Kareem
 */

#ifndef LCD_H_
#define LCD_H_

#include "micro_config.h"
#include "std_types.h"
#include "common_macros.h"


#define RS

#define CONTROL_REG   PORTD
#define CONNTROL_DIR  DDRD

#define DATA_REG		  PORTC
#define DATA_DIR		  DDRC

void LCD_init(void);
void LCD_sendCommand(uint8 command);
void LCD_displayCharacter(uint8 data);
void LCD_displayString(const uint8* str);
void LCD_intgerToString(int data);
void LCD_goToRowColumn(uint8 row,uint8 col);

#endif /* LCD_H_ */
