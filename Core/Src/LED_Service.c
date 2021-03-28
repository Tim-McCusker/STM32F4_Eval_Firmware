/*
 * LED_Service.c
 *
 *  Created on: Mar 28, 2021
 *      Author: mccuskert
 */

#include "eval.h"
#include "eval_timers.h"
#include "LED_Service.h"


#define BLINK_PERIOD    500U	//500ms

LED_Mode_t LED_Mode = MODE_OFF;

uint16_t blinkCnt = 0U;

void LED_Control(LED_Mode_t mode);


/* LED_Service()
 *
 * Runs LED_Service
 */
void LED_Service(void)
{
	LED_Control(LED_Mode);
}


/* LED_Set_Mode()
 *
 * Update LED mode
 * Arg: (LED_Mode_t) mode - LED mode defined in LED_Service.h
 */
void LED_Set_Mode(LED_Mode_t mode)
{
	LED_Mode = mode;
}


/* LED_Control()
 *
 * Handle LED given current LED mode
 * Arg: (LED_Mode_t) mode - LED mode defined in LED_Service.h
 */
void LED_Control(LED_Mode_t mode)
{
	static _Bool led_level = LEVEL_LOW;

	switch(mode)
	{
	case MODE_OFF:
		HAL_GPIO_WritePin(LED_Status_GPIO_Port, LED_Status_Pin, LEVEL_LOW);

	break;

	case MODE_SOLID:
		HAL_GPIO_WritePin(LED_Status_GPIO_Port, LED_Status_Pin, LEVEL_HIGH);

	break;

	case MODE_BLINK:
		if(BLINK_PERIOD <= timer[LED_BLINK].t)
		{
			RESET_TIMER((&timer[LED_BLINK]), TIMER_RUN);

			led_level = !led_level;

			HAL_GPIO_WritePin(LED_Status_GPIO_Port, LED_Status_Pin, led_level);

			if(LEVEL_HIGH == led_level)
			{
				blinkCnt++;
			}
		}

	break;

	default:
	break;
	}


}
