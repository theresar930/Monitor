#ifndef __LIGHT_SENSOR_H
#define __LIGHT_SENSOR_H

#include "stm32f1xx_hal.h"

#define LIGHT_Pin GPIO_PIN_4
#define LIGHT_GPIO_Port GPIOA

extern uint16_t adc_val ;//ADC      
extern uint16_t adc_volt;//3.3V 5V    3.3V - 4095  5V - 4095

void LightSensor_Init(void);
uint8_t LightSenso_ADC_Read(void);

#endif

