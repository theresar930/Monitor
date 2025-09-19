#ifndef __LED_H
#define __LED_H

#include "stm32f1xx_hal.h"

// LED���ƺ�������
void LED_Init(void);                      // LED��ʼ��
void LED_On(uint16_t GPIO_Pin);           // ����ָ��LED
void LED_Off(uint16_t GPIO_Pin);          // Ϩ��ָ��LED
void LED_Toggle(uint16_t GPIO_Pin);       // �л�ָ��LED״̬
void LED_Blink1(uint16_t GPIO_Pin, uint32_t delay); // LED��˸
void LED_Blink2(uint16_t GPIO_Pin, uint32_t delay); // LED��˸

#endif
