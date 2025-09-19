#ifndef __DHT11_H__
#define __DHT11_H__

#include "stm32f1xx_hal.h"

#define DHT11_PORT GPIOA
#define DHT11_GPIO_PIN GPIO_PIN_0

extern uint8_t temperature;
extern uint8_t humidity;

void DHT11_Init(void);
HAL_StatusTypeDef DHT11_ReadData(uint8_t *temp, uint8_t *humi);

#endif
