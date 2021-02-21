#include <stdio.h>
#include <string.h>

#include "main.h"
#include "eval.h"

//Private defines
#define UART_PERIOD_1S		1000
#define BLINK_PERIOD_500MS	500
#define BLINK_PERIOD_250MS	250

#define TEMP_ADC_GAIN		0.0342
#define TEMP_ADC_OFFSET		14

#define MESSAGE_LENGTH		128

// Function Prototypes
void Update_Timers(void);
void ADC_Service(void);
void UART_Service(void);
void Blink_LED(uint16_t period);

// HAL Structs
extern ADC_HandleTypeDef hadc1;

extern I2C_HandleTypeDef hi2c1;

extern TIM_HandleTypeDef htim11;

extern UART_HandleTypeDef huart3;

// Private global variables
_Bool tim11_OvrFlo_Flag = FALSE;

uint32_t blinkCounter_1ms = 0U;

uint32_t UART_Counter_1ms = 0U;

char msg[MESSAGE_LENGTH] = {0};

uint16_t blinkCnt = 0U;

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

	while(INFINITY)
	{
		Update_Timers();
		ADC_Service();
		UART_Service();
		Blink_LED(BLINK_PERIOD_250MS);

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

		UART_Counter_1ms++;
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
	if(UART_PERIOD_1S <= UART_Counter_1ms)
	{
		UART_Counter_1ms = 0U;

		sprintf(msg, "Temp raw: %hu\n\r", temp_ADC);

		HAL_UART_Transmit(&huart3, (uint8_t*)msg, sizeof(msg), 100);
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

		if(LEVEL_HIGH == led_level)
		{
			blinkCnt++;
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
	tim11_OvrFlo_Flag = TRUE;
}
