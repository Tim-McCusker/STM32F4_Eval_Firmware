/*
 * UART_Service.h
 *
 *  Created on: Mar 28, 2021
 *      Author: mccuskert
 */

#ifndef INC_UART_SERVICE_H_
#define INC_UART_SERVICE_H_


// HAL Struct
extern UART_HandleTypeDef huart3;


// -- Public Defines
#define MESSAGE_LENGTH		128


// -- Public Functions
extern void UART_Service_Init(void);
extern void UART_Service(void);
extern void UART_Set_Tx_Msg(char *mess);

#endif /* INC_UART_SERVICE_H_ */
