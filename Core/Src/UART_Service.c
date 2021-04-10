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


// -- Private globals

// Character buffer to transmit over UART
static char tx_msg[MESSAGE_LENGTH] = {0};


// -- Private Functions
static void UART_Tx(void);


/* UART_Service_Init()
 *
 * Initializes UART_Service
 * Args: N/A
 * Returns: N/A
 */
void UART_Service_Init(void)
{
	memset(tx_msg, 0, MESSAGE_LENGTH);

	RESET_TIMER((&timer[UART_TX]), TIMER_RUN);
}


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


/* UART_Set_Tx_Msg()
 *
 * Set message to transmit over UART
 * Args: *mess - char pointer
 * Returns: N/A
 */
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
static void UART_Tx()
{
	if(true == timer[UART_TX].flag)
	{
		RESET_TIMER((&timer[UART_TX]), TIMER_RUN);

		HAL_UART_Transmit(&huart3, (uint8_t*)tx_msg, sizeof(tx_msg), 100);
	}
}
