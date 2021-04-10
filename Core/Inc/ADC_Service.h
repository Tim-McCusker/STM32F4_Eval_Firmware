/*
 * ADC_Service.h
 *
 *  Created on: Mar 28, 2021
 *      Author: mccuskert
 */

#ifndef INC_ADC_SERVICE_H_
#define INC_ADC_SERVICE_H_


// HAL Struct
extern ADC_HandleTypeDef hadc1;


// -- Public Variables
extern double SysTemp;


// -- Public Functions
extern void ADC_Service_Init(void);
extern void ADC_Service(void);
extern double ADC_Get_SysTemp(void);

#endif /* INC_ADC_SERVICE_H_ */
