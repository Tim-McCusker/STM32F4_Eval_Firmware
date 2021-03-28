#include <stdio.h>
#include <string.h>

#include "eval.h"
#include "eval_timers.h"
#include "LED_Service.h"

//Private defines
#define TEMP_ADC_GAIN		0.0342
#define TEMP_ADC_OFFSET		14

#define MESSAGE_LENGTH		128

// Function Prototypes
void Update_Timers(void);
void ADC_Service(void);
void UART_Service(void);
void LED_Service();
void initTimers(void);

// HAL Structs
extern TIM_HandleTypeDef htim11;

// Private global variables
_Bool tim11_OvrFlo_Flag = FALSE;

const uint16_t system_timer_overflow[NUM_SYS_TIMERS] =
{
		TIME1MS_1MS,	// ADC_Sample
		TIME1MS_500MS,	// LED_BLINK
		TIME1MS_1S	    // UART_TX
};

char msg[MESSAGE_LENGTH] = {0};

uint16_t temp_ADC = 0U;

float SysTemp = 0;


/* System_Control()
 *
 * Runs the system infinite loop
 * Args: N/A
 * Returns: N/A
 */
void System_Control()
{
	initTimers();
	LED_Set_Mode(MODE_BLINK);

	while(INFINITY)
	{
		Update_Timers();
		ADC_Service();
		UART_Service();
		LED_Service();

	}
}


/* Update_Timers()
 *
 * Updates global 1ms counters attached to hwtim11
 * Args: N/A
 * Returns: N/A
 */
void Update_Timers(void)
{
	if(tim11_OvrFlo_Flag)
	{
		tim11_OvrFlo_Flag = FALSE;

		for(uint8_t idx = 0U; idx < NUM_SYS_TIMERS; idx++)
		{
			if(!timer[idx].lock)
			{
				++timer[idx].t;

				if(system_timer_overflow[idx] <= timer[idx].t)
				{
				    timer[idx].flag = TRUE;
				}
			}
			else
			{
				timer[idx].t = 0U;
			}
		}
	}
}


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
}


/* UART_Service()
 *
 * Transmits 128 bit UART message at defined period
 * Args: N/A
 * Returns: N/A
 */
void UART_Service()
{
	if(timer[UART_TX].flag)
	{
		timer[UART_TX].flag = FALSE;
		timer[UART_TX].t = 0U;

		sprintf(msg, "Temp raw: %hu\n\r", temp_ADC);

		HAL_UART_Transmit(&huart3, (uint8_t*)msg, sizeof(msg), 100);
	}
}


void initTimers(void)
{
	for(uint8_t idx = 0U; idx < NUM_SYS_TIMERS; idx++)
	{
		timer[idx].t = 0U;
		timer[idx].flag = FALSE;
		timer[idx].lock = FALSE;    // TODO: Should init to TRUE
	}
}


/*HAL_TIM_PeriodElapsedCallback()
 *
 * General Timer Overflow ISR
 * Args: htim (TIM_HandleTypeDef)
 * Returns: N/A
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	tim11_OvrFlo_Flag = TRUE;
}
