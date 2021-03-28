/*
 * UART_Service.h
 *
 *  Created on: Mar 28, 2021
 *      Author: mccuskert
 */

#ifndef INC_UART_SERVICE_H_
#define INC_UART_SERVICE_H_

#define MESSAGE_LENGTH		128

void UART_Service(void);
void UART_Set_Tx_Msg(char *mess);

#endif /* INC_UART_SERVICE_H_ */
