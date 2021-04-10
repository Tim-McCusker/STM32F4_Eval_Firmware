/*
 * LED_Service.c
 *
 *  Created on: Mar 28, 2021
 *      Author: mccuskert
 */

#include "eval.h"
#include "LED_Service.h"

// -- Private defines
#define BLINK_PERIOD    500U	//500ms


// -- Private globals

// Mode of operation for LED_STATUS
static LED_Mode_t LED_Mode = MODE_OFF;

// Number of LED_STATUS blinks since boot
static uint32_t blinkCnt = 0U;


// -- Private functions
static void LED_Control(LED_Mode_t mode);


/* LED_Service_Init()
 *
 * Initialize LED_Service
 * Args: N/A
 * Returns: N/A
 */
void LED_Service_Init(void)
{
	blinkCnt = 0U;
	LED_Set_Mode(MODE_BLINK);

	RESET_TIMER((&timer[LED_BLINK]), TIMER_RUN);
}


/* LED_Service()
 *
 * Runs LED_Service
 * Args: N/A
 * Returns: N/A
 */
void LED_Service(void)
{
	LED_Control(LED_Mode);
}


/* LED_Set_Mode()
 *
 * Set mode of operation for LED_STATUS
 * Arg: (LED_Mode_t) mode - Mode of operation
 * Returns: N/A
 */
void LED_Set_Mode(LED_Mode_t mode)
{
	LED_Mode = mode;
}


/* LED_Control()
 *
 * Handle LED given current LED mode
 * Arg: (LED_Mode_t) mode - Mode of operation
 * Returns: N/A
 */
static void LED_Control(LED_Mode_t mode)
{
	static uint8_t led_level = LEVEL_LOW;

	switch(mode)
	{
	case MODE_OFF:
		output[LED_STATUS].cmd = LEVEL_LOW;

	break;

	case MODE_SOLID:
		output[LED_STATUS].cmd = LEVEL_HIGH;

	break;

	case MODE_BLINK:
		if(BLINK_PERIOD <= timer[LED_BLINK].t)
		{
			RESET_TIMER((&timer[LED_BLINK]), TIMER_RUN);

			led_level = !led_level;

			output[LED_STATUS].cmd = led_level;

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
