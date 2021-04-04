/*
 * eval_GPIO.h
 *
 *  Created on: Apr 4, 2021
 *      Author: mccuskert
 */

#ifndef INC_EVAL_GPIO_H_
#define INC_EVAL_GPIO_H_

#define LEVEL_LOW		0
#define LEVEL_HIGH		1

#define NUM_OUTPUTS    (1U)

enum outputs
{
    LED_STATUS
};

typedef struct system_gpio_out
{

	GPIO_TypeDef *port;
	uint16_t pin;
    bool cmd;
    bool cmd_last;
}system_gpio_out_t;

volatile system_gpio_out_t output[NUM_OUTPUTS];

#endif /* INC_EVAL_GPIO_H_ */
