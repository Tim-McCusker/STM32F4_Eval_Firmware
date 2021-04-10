/*
 * eval_timers.h
 *
 *  Created on: Mar 28, 2021
 *      Author: mccuskert
 */

#ifndef INC_EVAL_TIMERS_H_
#define INC_EVAL_TIMERS_H_

// -- Public Defines

// Time constants in 1ms resolution
#define TIME1MS_1MS		1U
#define TIME1MS_10MS	10U
#define TIME1MS_25MS	25U
#define TIME1MS_50MS	50U
#define TIME1MS_75MS	75U
#define TIME1MS_100MS	100U
#define TIME1MS_200MS	200U
#define TIME1MS_250MS	250U
#define TIME1MS_300MS	300U
#define TIME1MS_400MS	400U
#define TIME1MS_500MS	500U
#define TIME1MS_600MS	600U
#define TIME1MS_700MS	700U
#define TIME1MS_750MS	750U
#define TIME1MS_800MS	800U
#define TIME1MS_900MS	900U
#define TIME1MS_1S	    1000U
#define TIME1MS_2S		2000U
#define TIME1MS_3S		3000U
#define TIME1MS_4S		4000U

#define TIMER_STOP	((_Bool) 1)
#define TIMER_RUN	((_Bool) 0)

// -- Public Macros
#define RESET_TIMER(timer, stop_go) ({ 	\
	    timer->lock = stop_go;			\
		timer->flag = 0U;			    \
		timer->t = 0U;					\
    	})


// -- Public Types
typedef struct SYS_Timer
{
	uint32_t t;
	bool flag;
	bool lock;
}system_timer_t;


const enum system_timers
{
	SYS_TIME,
	ADC_SAMPLE,
	LED_BLINK,
	UART_TX,
	NUM_TIMERS    //XXX: Always leave as last element in enum
}system_timers;


// -- Public globals

// Array of SYS_Timer structs - corresponds with system_timers enum
volatile system_timer_t timer[NUM_TIMERS];

extern uint32_t SysTimeInSeconds;


#endif /* INC_EVAL_TIMERS_H_ */
