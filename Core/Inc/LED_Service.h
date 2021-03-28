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
	MODE_ON,
	MODE_BLINK,
	MODE_OFF
}LED_Mode_t;


void LED_Service();
void LED_Set_Mode(LED_Mode_t mode);

#endif /* INC_LED_SERVICE_H_ */
