/*
 * UART_Service.c
 *
 *  Created on: Mar 28, 2021
 *      Author: mccuskert
 */

#include <stdio.h>
#include <string.h>

#include "eval.h"
#include "UART_Service.h"


static char tx_msg[MESSAGE_LENGTH] = {0};


void UART_Tx(void);


/* UART_Service()
 *
 * Runs UART_Service
 * Args: N/A
 * Returns: N/A
 */
void UART_Service(void)
{
	UART_Tx();
}


void UART_Set_Tx_Msg(char *mess)
{
	memcpy(&tx_msg, mess, MESSAGE_LENGTH);
}


/* UART_Service()
 *
 * Transmits 128 bit UART message at defined period
 * Args: N/A
 * Returns: N/A
 */
void UART_Tx()
{
	if(1U == timer[UART_TX].flag)
	{
		RESET_TIMER((&timer[UART_TX]), TIMER_RUN);

		HAL_UART_Transmit(&huart3, (uint8_t*)tx_msg, sizeof(tx_msg), 100);
	}
}
