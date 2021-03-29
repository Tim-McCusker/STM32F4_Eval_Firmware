/*
 * ADC_Service.c
 *
 *  Created on: Mar 28, 2021
 *      Author: mccuskert
 */

#include <stdio.h>

#include "eval.h"
#include "eval_timers.h"
#include "UART_Service.h"
#include "ADC_Service.h"


// Private defines
#define TEMP_ADC_GAIN		0.0342
#define TEMP_ADC_OFFSET		14

#define TEMP_ADC_GAIN_2		-0.0021
#define TEMP_ADC_OFFSET_2	100

#define TEMP_NUM_SAMPLES    128
#define TEMP_SHIFT          7


// Private globals
char adc_msg[MESSAGE_LENGTH] = {0};
double SysTemp = 0;


void ADC_Sample(void);


void ADC_Service(void)
{
	ADC_Sample();
}


/* ADC_Service()
 *
 * Polls ADC1 for System Temp. Calculates real deg C.
 * Args: N/A
 * Returns: N/A
 */
void ADC_Sample(void)
{
	static uint16_t sample_count = 0U;
	static uint16_t ADC_Sample = 0U;
    static uint32_t ADC_Avg = 0U;


	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
	ADC_Sample = HAL_ADC_GetValue(&hadc1);
	ADC_Avg += ADC_Sample;

	++sample_count;

	if(TEMP_NUM_SAMPLES <= sample_count)
	{
		SysTemp = TEMP_ADC_GAIN * (ADC_Avg >> TEMP_SHIFT) + TEMP_ADC_OFFSET;

		sprintf(adc_msg, "Temp ADC: %lu\n\rTemp C: %f\n\r", ADC_Avg, SysTemp);

		UART_Set_Tx_Msg(adc_msg);

		ADC_Avg = 0U;
		sample_count = 0U;
	}
}


double ADC_Get_SysTemp(void)
{
	return SysTemp;
}
