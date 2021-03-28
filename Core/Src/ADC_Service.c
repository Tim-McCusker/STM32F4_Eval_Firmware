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


// Private globals
char adc_msg[MESSAGE_LENGTH] = {0};
uint16_t temp_ADC = 0U;
float SysTemp = 0;


/* ADC_Service()
 *
 * Polls ADC1 for System Temp. Calculates real deg C.
 * Args: N/A
 * Returns: N/A
 */
void ADC_Service()
{
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
	temp_ADC = HAL_ADC_GetValue(&hadc1);

	SysTemp = TEMP_ADC_GAIN * temp_ADC + TEMP_ADC_OFFSET;

	sprintf(adc_msg, "Temp ADC: %hu\n\rTemp C: %f\n\r", temp_ADC, SysTemp);

	UART_Set_Tx_Msg(adc_msg);
}
