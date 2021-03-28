#include <stdio.h>
#include <string.h>

#include "eval.h"
#include "eval_timers.h"
#include "ADC_Service.h"
#include "UART_Service.h"
#include "LED_Service.h"

//Private defines


// Function Prototypes
void Update_Timers(void);
void initTimers(void);

// HAL Structs
extern TIM_HandleTypeDef htim11;

// Private global variables
_Bool tim11_OvrFlo_Flag = FALSE;

const uint16_t system_timer_overflow[NUM_SYS_TIMERS] =
{
		TIME1MS_1MS,	// 0 - ADC_Sample
		TIME1MS_500MS,	// 1 - LED_BLINK
		TIME1MS_1S	    // 2 - UART_TX
};


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
 * Update system timers
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
