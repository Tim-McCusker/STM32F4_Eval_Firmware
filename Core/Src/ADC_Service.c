/*
 * ADC_Service.c
 *
 *  Created on: Mar 28, 2021
 *      Author: mccuskert
 */

#include <stdio.h>
#include <string.h>

#include "eval.h"
#include "UART_Service.h"
#include "ADC_Service.h"


// -- Private defines
#define TEMP_ADC_GAIN		(float)0.0342
#define TEMP_ADC_OFFSET		14

#define TEMP_NUM_SAMPLES    128
#define TEMP_SHIFT          7


// -- Public globals


// -- Private globals

// Value for system temperature in degC
double SysTemp = 0.0;

// Character buffer for message from ADC to UART
static char adc_msg[MESSAGE_LENGTH] = {0};


// -- Private Functions
static void ADC_Sample(void);


/* ADC_Service_Init()
 *
 * Initializes ADC service
 * Args: N/A
 * Returns: N/A
 */
void ADC_Service_Init()
{
	SysTemp = 0.0;
	memset(adc_msg, 0, MESSAGE_LENGTH);

	RESET_TIMER((&timer[ADC_SAMPLE]), TIMER_RUN);
}


/* ADC_Service()
 *
 * Runs ADC service
 * Args: N/A
 * Returns: N/A
 */
void ADC_Service(void)
{
	ADC_Sample();
}


/* ADC_Get_SysTemp()
 *
 * Retrieve system temperature
 * Args: N/A
 * Returns: SysTemp - System temperature
 */
double ADC_Get_SysTemp(void)
{
	return SysTemp;
}


/* ADC_Sample()
 *
 * Polls ADC1 for System Temp. Calculates real deg C.
 * Args: N/A
 * Returns: N/A
 */
static void ADC_Sample(void)
{
	static uint8_t sample_count = 0U;
	static uint16_t ADC_Sample = 0U;
    static uint32_t ADC_Avg = 0U;

    if(timer[ADC_SAMPLE].flag)
    {
	    HAL_ADC_Start(&hadc1);
	    HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
	    ADC_Sample = HAL_ADC_GetValue(&hadc1);
	    ADC_Avg += ADC_Sample;

	    ++sample_count;

	    RESET_TIMER((&timer[ADC_SAMPLE]), TIMER_RUN);

	    if(TEMP_NUM_SAMPLES <= sample_count)
	    {
		    SysTemp = TEMP_ADC_GAIN * (ADC_Avg >> TEMP_SHIFT) + TEMP_ADC_OFFSET;

		    snprintf(adc_msg, MESSAGE_LENGTH, "Temp ADC: %lu\n\rTemp C: %f\n\r", ADC_Avg, SysTemp);

		    UART_Set_Tx_Msg(adc_msg);

		    ADC_Avg = 0U;
		    sample_count = 0U;
	    }

    }
}
