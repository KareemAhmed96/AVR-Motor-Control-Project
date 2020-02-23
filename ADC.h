/*
 * ADC.h
 *
 *  Created on: Apr 20, 2019
 *      Author: Kareem
 */

#ifndef ADC_H_
#define ADC_H_

#include "common_macros.h"
#include "std_types.h"
#include "micro_config.h"

#define ON                   1
#define OFF                  0

#define ADC_ENABLE		 	 1
#define ADC_AUTO_TRIG_EN	 1
#define INTERRUPT_EN  		 ON

extern volatile uint16 g_result;


/*************************************Voltage References************************************
 *																						   *
 *         AREF PIN: External voltage source (PIN 32), Internal Voltage turned off         *
 *             AVCC: 5V with external capacitor at AREF pin                                *
 * Internal Voltage: Internal 2.56V Voltage reference with external capacitor at AREF pin  *
 *******************************************************************************************/
typedef enum {

	AREF_PIN, AVCC, RESERVERED, INTERNAL_VOLTAGE
} ADC_Voltage_Reference;

/*******************************************************************************************/


/*
 * Right of Left Adjustments for the result
 */
typedef enum {

	RIGHT_ADJUST, LEFT_ADJUST
} ADC_Result_Adjustment;

/*
 * ADC Pre-scaler Select "3-Bits" (Division Factor)
 */
typedef enum {

	DIV_BY_2 = 1,
	DIV_BY_4,
	DIV_BY_8,
	DIV_BY_16,
	DIV_BY_32,
	DIV_BY_64,
	DIV_BY_128

} ADC_Prescaler_Division_Factor;


/*
 *  ADC Auto Trigger Source Selection Bits
 *  --------------------------------------
 *  ADATE bit in ADCSRA register has to be written to one for these
 *  bits to have an effect
 */
typedef enum {

	FREE_RUNNING_MODE,
	ANALOG_COMPARATOR,
	EXTENAL_INTERRUPT_REQUEST_0,
	TIMER_COUNTER_0_COMPARE_MATCH,
	TIMER_COUNTER_0_OVERFLOW,
	TIMER_COUNTER_1_COMPARE_MATCH_B,
	TIMER_COUNTER_1_OVERFLOW,
	TIMER_COUNTER_1_CAPTURE_EVENT

} ADC_Auto_Trigger_Source;

typedef struct {

	ADC_Voltage_Reference voltageRef;
	ADC_Result_Adjustment resAdjust;
	//ADC_Analog_Channel_Type *channelType; //Union
	ADC_Prescaler_Division_Factor prescalerBits;
	ADC_Auto_Trigger_Source trigSource;

} ADC_Config_Type;

/*****************************************************************
 *           Analog Channel and Gain Selection Bits              *
 * 			----------------------------------------		     *
 * They are split into three enumerations, one for each set of   *
 * configurations. 												 *
 * From channel 1 to channel 7: Single Ended Input 		  	 	 *
 * From option 8 to option 15:  Double Ended Input with gain     *
 * From option 16 to option 32: Double Ended Input without gain  *
 * 																 *
 *****************************************************************/

// First enumeration (3-bits are needed)
typedef enum {

	ADC0, ADC1, ADC2, ADC3, ADC4, ADC5, ADC6, ADC7

} ADC_Single_Ended_Input;

//Second enumeration (4-bits are needed)
typedef enum {
	// (PositiveInputPin)_(NegativeInputPin)_(GainMultiplier)
	ADC0_ADC0_G10X = 8,
	ADC1_ADC0_G10X,
	ADC0_ADC0_G200X,
	ADC1_ADC0_G200X,
	ADC2_ADC2_G10X,
	ADC3_ADC2_G10X,
	ADC2_ADC2_G200X,
	ADC3_ADC2_G200X

} ADC_Double_Ended_Input_With_Gain;

//Third enumeration (5-bits are needed)
typedef enum {
	// (PositiveInputPin)_(NegativeInputPin)
	// (VBG) -> 1.22V, GND -> 0V
	ADC0_ADC1 = 16,
	ADC1_ADC1,
	ADC2_ADC1,
	ADC3_ADC1,
	ADC4_ADC1,
	ADC5_ADC1,
	ADC6_ADC1,
	ADC7_ADC1,
	ADC0_ADC2,
	ADC1_ADC2,
	ADC2_ADC2,
	ADC3_ADC2,
	ADC4_ADC2,
	ADC5_ADC2,
	CONST_VBG,
	GND

} ADC_Double_Ended_Input_Without_Gain;

// Union containing three enumerations of input type
typedef union {

	ADC_Single_Ended_Input singleEndedInput;
	ADC_Double_Ended_Input_With_Gain doubleEndedInputGain;
	ADC_Double_Ended_Input_Without_Gain doubleEndedInputNoGain;

} ADC_Analog_Channel_Type;

/****************************************************************/


/****************************************************************
 *                      Function Prototypes                     *
 ****************************************************************/

/*	1- ADC_init()
 *     Initializes the ADC Module
 *
 * 	   It's input parameter is a pointer to constant structure (Configuration Structure)
 *
 * 	   Also choosing interrupts or polling is implemented inside this function
 */
void ADC_init(const ADC_Config_Type *ADC_Config_Ptr);
/*
 * 2- ADC_readChannel()
 *	  Using Interrupts & Polling
 */
void ADC_readChannel_Interrupts(const ADC_Analog_Channel_Type *Channel_Ptr);
uint16 ADC_readChannel_Polling(const ADC_Analog_Channel_Type *Channel_Ptr);

#endif /* ADC_H_ */
