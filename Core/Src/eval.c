/*
 * eval.c
 *
 *  Created on: Mar 28, 2021
 *      Author: mccuskert
 */

#include <stdio.h>
#include <string.h>

#include "eval.h"
#include "ADC_Service.h"
#include "UART_Service.h"
#include "LED_Service.h"


// -- Private defines


// HAL Structs
extern TIM_HandleTypeDef htim11;
extern I2C_HandleTypeDef hi2c1;


// -- Public global variables

// Time in seconds since application boot
uint32_t SysTimeInSeconds = 0U;


// -- Private global variables

// Set true when timer 11 overflows, set false after timers are updated
static bool tim11_OvrFlo_Flag = false;

// Overflow counts in 1ms resolution
static const uint16_t system_timer_overflow[NUM_TIMERS] =
{
		TIME1MS_1S,     // 0 - SYS_TIME
		TIME1MS_1MS,	// 1 - ADC_SAMPLE
		TIME1MS_500MS,	// 2 - LED_BLINK
		TIME1MS_1S	    // 3 - UART_TX
};

// Active logic levels for each output GPIO pin
static const bool output_active[NUM_OUTPUTS] =
{
		LEVEL_HIGH      // 0 - LED_STATUS
};


// -- Private functions
static void initTimers(void);
static void initGPIO(void);
static void Update_Timers(void);
static void Monitor_Inputs(void);
static void System_Services_Init(void);
static void System_Services(void);
static void Set_Outputs(void);



/* System_Init()
 *
 * Initialize the system
 * Args: N/A
 * Returns: N/A
 */
void System_Init()
{
	initTimers();
	RESET_TIMER((&timer[SYS_TIME]), TIMER_RUN);

	initGPIO();
	System_Services_Init();

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
		Monitor_Inputs();
		System_Services();
		Set_Outputs();
	}
}


/* System_Services_Init()
 *
 * Initializes system services
 * Args: N/A
 * Returns: N/A
 */
static void System_Services_Init()
{
    ADC_Service_Init();
    UART_Service_Init();
    LED_Service_Init();
}


/* System_Services()
 *
 * Runs the application services
 * Args: N/A
 * Returns: N/A
 */
static void System_Services()
{
	ADC_Service();
	UART_Service();
	LED_Service();
}


/* initTimers()
 *
 * Initialize system timers
 * Args: N/A
 * Returns: N/A
 */
static void initTimers(void)
{
	for(uint8_t idx = 0U; idx < NUM_TIMERS; idx++)
	{
		timer[idx].t = 0U;
		timer[idx].flag = false;
		timer[idx].lock = true;
	}
}


/* Update_Timers()
 *
 * Update system timers
 * Args: N/A
 * Returns: N/A
 */
static void Update_Timers(void)
{
	if(true == tim11_OvrFlo_Flag)
	{
		tim11_OvrFlo_Flag = false;

		for(uint8_t idx = 0U; idx < NUM_TIMERS; idx++)
		{
			if(false == timer[idx].lock)
			{
				++timer[idx].t;

				if(system_timer_overflow[idx] <= timer[idx].t)
				{
				    timer[idx].flag = true;
				}
			}
			else
			{
				timer[idx].t = 0U;
			}
		}
	}

	if(true == timer[SYS_TIME].flag)
	{
		RESET_TIMER((&timer[SYS_TIME]), TIMER_RUN);

		++SysTimeInSeconds;
	}
}


/* initGPIO()
 *
 * Initialize system GPIO
 * Args: N/A
 * Returns: N/A
 */
static void initGPIO(void)
{
    output[LED_STATUS].port = LED_Status_GPIO_Port;
    output[LED_STATUS].pin = LED_Status_Pin;
    output[LED_STATUS].cmd = LEVEL_LOW;
    output[LED_STATUS].cmd_last = LEVEL_HIGH;
}


/* Monitor_Inputs()
 *
 * Monitor system inputs
 * Args: N/A
 * Returns: N/A
 */
static void Monitor_Inputs()
{
	//XXX: No inputs to monitor
}


/* Set_Outputs()
 *
 * Update system output states
 * Args: N/A
 * Returns: N/A
 */
static void Set_Outputs()
{
	for(uint8_t i = 0; i < NUM_OUTPUTS; i++)
	{
		if(output[i].cmd != output[i].cmd_last)
		{
			//XXX: Abstraction from physical logic level, all FW GPIO logic is active high
			bool level = output[i].cmd ? output_active[i] : !output_active[i];

			HAL_GPIO_WritePin(output[i].port, output[i].pin, level);

			output[i].cmd_last = output[i].cmd;
		}
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
	tim11_OvrFlo_Flag = true;
}
