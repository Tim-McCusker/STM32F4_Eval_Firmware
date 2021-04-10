/*
 * eval_GPIO.h
 *
 *  Created on: Apr 4, 2021
 *      Author: mccuskert
 */

#ifndef INC_EVAL_GPIO_H_
#define INC_EVAL_GPIO_H_

// -- Public Defines
#define LEVEL_LOW		0
#define LEVEL_HIGH		1


// -- Public Types
const enum outputs
{
    LED_STATUS,
	NUM_OUTPUTS    //XXX: Always leave as last element in enum
}outputs;

typedef struct system_gpio_out
{

	GPIO_TypeDef *port;
	uint16_t pin;
    bool cmd;
    bool cmd_last;
}system_gpio_out_t;


// -- Public Globals

// Array of output system_gpio_out struct, corresponds with outputs enum
volatile system_gpio_out_t output[NUM_OUTPUTS];

#endif /* INC_EVAL_GPIO_H_ */
