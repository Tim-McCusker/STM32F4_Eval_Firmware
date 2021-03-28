/*
 * LED_Service.h
 *
 *  Created on: Mar 28, 2021
 *      Author: mccuskert
 */

#ifndef INC_LED_SERVICE_H_
#define INC_LED_SERVICE_H_


typedef enum LED_Mode_e
{
	MODE_OFF = 0U,
	MODE_SOLID = 1U,
	MODE_BLINK = 2U
}LED_Mode_t;


void LED_Service();
void LED_Set_Mode(LED_Mode_t mode);

#endif /* INC_LED_SERVICE_H_ */
