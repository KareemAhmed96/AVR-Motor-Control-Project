/*
 * ADC.c
 *
 *  Created on: Apr 20, 2019
 *      Author: Kareem
 */

#include "ADC.h"

volatile uint16 g_result;

void ADC_init(const ADC_Config_Type *ADC_Config_Ptr){

	ADMUX = (ADMUX & 0x3F) | ((ADC_Config_Ptr -> voltageRef) << REFS0); 	//Two Bits for Voltage Reference
	ADMUX = (ADMUX & 0xDF) | ((ADC_Config_Ptr -> resAdjust)  << ADLAR); 	//Left or Right Adjustment

	ADCSRA = (ADCSRA & 0xF8) | (ADC_Config_Ptr -> prescalerBits); 		//Pre-scaler Division factor 3-Bits

	ADCSRA = (ADCSRA & 0xDF) | (ADC_AUTO_TRIG_EN << ADATE);               	//Auto Trigger Enable
	SFIOR  = (SFIOR & 0x1F)  | ((ADC_Config_Ptr -> trigSource) << ADTS0); 	//Auto Trigger Source

#if (INTERRUPT_EN == ON)

	SET_BIT(ADCSRA, ADIE); //Interrupt Enable

#elif (INTERRUPT_EN == OFF)

	CLEAR_BIT(ADCSRA, ADIE);

#endif

	ADCSRA |= (ADC_ENABLE<<ADEN); //ADC Enable
}

/*
 * The address of the union will be passed to this function,
 * after defining the configurations of it's members
 */
#if (INTERRUPT_EN == ON)

void ADC_readChannel_Interrupts(const ADC_Analog_Channel_Type *Channel_Ptr){

	//Insert channel number to register
	ADMUX = (ADMUX & 0xF8) | (Channel_Ptr -> singleEndedInput);       //First 3 bits (Single Ended Input)
	ADMUX = (ADMUX & 0xF0) | (Channel_Ptr -> doubleEndedInputGain);   //First 4 bits (Double with gain)
	ADMUX = (ADMUX & 0xE0) | (Channel_Ptr -> doubleEndedInputNoGain); //First 5 bits (Double without gain)

	// Start conversion
	SET_BIT(ADCSRA, ADSC);
}
#elif (INTERRUPT_EN == OFF)

uint16 ADC_readChannel_Polling(const ADC_Analog_Channel_Type *Channel_Ptr){

	//Insert channel number to register
	ADMUX = (ADMUX & 0xF8) | (Channel_Ptr -> singleEndedInput);       //First 3 bits (Single Ended Input)
	ADMUX = (ADMUX & 0xF0) | (Channel_Ptr -> doubleEndedInputGain);   //First 4 bits (Double with gain)
	ADMUX = (ADMUX & 0xE0) | (Channel_Ptr -> doubleEndedInputNoGain); //First 5 bits (Double without gain)

	// Start conversion
	SET_BIT(ADCSRA, ADSC);

	// Polling until ADIF (flag) = 1
	while(BIT_IS_CLEAR(ADCSRA, ADIF));

	// Clear the flag for next time by writing one the same bit
	SET_BIT(ADCSRA, ADIF);

	return ADC;
	//return (ADCL) | (ADCH << 8);
}
#endif

ISR(ADC_vect){

	g_result = ADC;
	SET_BIT(ADCSRA, ADIF); //Clearing the Interrupt flag
}
