#include <stdio.h>
#include <string.h>

#include "eval.h"
#include "ADC_Service.h"
#include "UART_Service.h"
#include "LED_Service.h"


//Private defines


// Function Prototypes
static void initTimers(void);
static void Update_Timers(void);


// HAL Structs
extern TIM_HandleTypeDef htim11;
extern I2C_HandleTypeDef hi2c1;


// Private global variables
static uint8_t tim11_OvrFlo_Flag = 0U;


// Public global variables
const uint16_t system_timer_overflow[NUM_SYS_TIMERS] =
{
		TIME1MS_1S,     // 0 - SysTimer
		TIME1MS_1MS,	// 1 - ADC_Sample
		TIME1MS_500MS,	// 2 - LED_BLINK
		TIME1MS_1S	    // 3 - UART_TX
};

uint32_t SysTimeInSeconds = 0U;


/* System_Init()
 *
 * Initialize the system
 * Args: N/A
 * Returns: N/A
 */
void System_Init()
{
	initTimers();
	LED_Set_Mode(MODE_BLINK);

	SysTimeInSeconds = 0U;
}


/* System_Control()
 *
 * Runs the system application
 * Args: N/A
 * Returns: N/A
 */
void System_Control()
{
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
	if(1U == tim11_OvrFlo_Flag)
	{
		tim11_OvrFlo_Flag = 0U;

		for(uint8_t idx = 0U; idx < NUM_SYS_TIMERS; idx++)
		{
			if(0U == timer[idx].lock)
			{
				++timer[idx].t;

				if(system_timer_overflow[idx] <= timer[idx].t)
				{
				    timer[idx].flag = 1U;
				}
			}
			else
			{
				timer[idx].t = 0U;
			}
		}
	}

	if(1U == timer[SYS_TIME].flag)
	{
		RESET_TIMER((&timer[SYS_TIME]), TIMER_RUN);

		++SysTimeInSeconds;
	}
}


/* initTimers()
 *
 * Initialize system timers
 * Args: N/A
 * Returns: N/A
 */
void initTimers(void)
{
	for(uint8_t idx = 0U; idx < NUM_SYS_TIMERS; idx++)
	{
		timer[idx].t = 0U;
		timer[idx].flag = 0U;
		timer[idx].lock = 0U;    // TODO: Should init to TRUE
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
	tim11_OvrFlo_Flag = 1U;
}
