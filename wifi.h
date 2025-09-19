#ifndef __WIFI_H
#define __WIFI_H

#include "stm32f1xx_hal.h"

void WIFI_Init(void);
void WIFI_SendData(const char *data);
void Handle_WiFi_Communication(void);
uint16_t WIFI_ReceiveData(char *buffer, uint16_t buffer_size);

#endif

