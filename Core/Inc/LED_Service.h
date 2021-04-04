/*
 * LED_Service.h
 *
 *  Created on: Mar 28, 2021
 *      Author: mccuskert
 */

#ifndef INC_LED_SERVICE_H_
#define INC_LED_SERVICE_H_


// Public Types
typedef enum LED_Mode_e
{
	MODE_OFF,
	MODE_SOLID,
	MODE_BLINK
}LED_Mode_t;


// Public Variables


// Public Functions
void LED_Service_Init(void);
void LED_Service(void);
void LED_Set_Mode(LED_Mode_t mode);

#endif /* INC_LED_SERVICE_H_ */
