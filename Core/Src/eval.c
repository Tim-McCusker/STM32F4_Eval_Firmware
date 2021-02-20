#include <stdio.h>

#include "main.h"
#include "eval.h"

//Private defines
#define UART_PERIOD_1S		1000
#define BLINK_PERIOD_500MS	500
#define BLINK_PERIOD_250MS	250

// Function Prototypes
void Update_Timers(void);
void Blink_LED(uint16_t period);

// HAL Structs
ADC_HandleTypeDef hadc1;

I2C_HandleTypeDef hi2c1;

TIM_HandleTypeDef htim11;

UART_HandleTypeDef huart3;

// Private global variables
_Bool tim11_OvrFlo_Flag = FALSE;

uint32_t blinkCounter_1ms = 0U;

uint32_t UART_Counter_1ms = 0U;


/* System_Control()
 *
 * Runs the system infinite loop
 * Args: N/A
 * Returns: N/A
 */
void System_Control()
{

	while(INFINITY)
	{
		Update_Timers();
		Blink_LED(BLINK_PERIOD_500MS);
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

		blinkCounter_1ms++;
	}
}



/* Blink_LED()
 *
 * Blinks R2D2's led at given period
 * Args: period (unsigned 16 bit)
 * Returns: N/A
 */
void Blink_LED(uint16_t period)
{
	static _Bool led_level = LEVEL_LOW;

	if(period <= blinkCounter_1ms)
	{
		blinkCounter_1ms = 0U;

		led_level = !led_level;

		HAL_GPIO_WritePin(LED_Status_GPIO_Port, LED_Status_Pin, led_level);
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
