/*
 * LCD.c
 *
 *  Created on: Apr 19, 2019
 *      Author: Kareem
 */
#include "LCD.h"

void LCD_init(void){

	DDRD |= (1<<4) | (1<<5) | (1<<6);
	DDRC = 0xFF;

	LCD_sendCommand(0x38);
	LCD_sendCommand(0x0C);	//Cursor OFF
	LCD_sendCommand(0x01);  //Clear Screen

}
void LCD_sendCommand(uint8 command){

	CLEAR_BIT(CONTROL_REG, 4); 		// RS = 0
	CLEAR_BIT(CONTROL_REG, 5); 		// R/W = 0
	_delay_ms(1); 					// t(as) 50 n seconds

	SET_BIT(CONTROL_REG, 6); 		// Enable = 1
	_delay_ms(1);					//190 n seconds

	DATA_REG = command;
	_delay_ms(1);

	CLEAR_BIT(CONTROL_REG, 6);		// Enable = 0
	_delay_ms(1);

}

void LCD_displayCharacter(uint8 data){

	SET_BIT(CONTROL_REG, 4); 		// RS = 1
	CLEAR_BIT(CONTROL_REG, 5); 		// R/W = 0
	_delay_ms(1); 					// t(as) 50 n seconds

	SET_BIT(CONTROL_REG, 6); 		// Enable = 1
	_delay_ms(1);					//190 n seconds

	DATA_REG = data;
	_delay_ms(1);

	CLEAR_BIT(CONTROL_REG, 6);		// Enable = 0
	_delay_ms(1);
}

void LCD_displayString(const uint8* str){  //pointer to constant data

	uint8 i = 0;

	while (str[i] != '\0') {

		LCD_displayCharacter(str[i]);
		i++;
	}

	 /*
	 while(*str != '\0'){

	 LCD_displayCharacter(*str);
	 str++;
	 }
	 */
}

void LCD_intgerToString(int data)
{
   uint8 buff[16]; /* String to hold the ascii result */
   itoa(data,buff,10); /* 10 for decimal */
   LCD_displayString(buff);
}

void LCD_goToRowColumn(uint8 row,uint8 col)
{
	uint8 Address;

	/* first of all calculate the required address */
	switch(row)
	{
		case 0:
				Address=col;
				break;
		case 1:
				Address=col+0x40;
				break;
		case 2:
				Address=col+0x10;
				break;
		case 3:
				Address=col+0x50;
				break;
	}
	/* to write to a specific address in the LCD
	 * we need to apply the corresponding command 0b10000000+Address */
	LCD_sendCommand(Address | 0x80);
}
