#ifndef __LED_H
#define __LED_H

#include "stm32f1xx_hal.h"

// LED控制函数声明
void LED_Init(void);                      // LED初始化
void LED_On(uint16_t GPIO_Pin);           // 点亮指定LED
void LED_Off(uint16_t GPIO_Pin);          // 熄灭指定LED
void LED_Toggle(uint16_t GPIO_Pin);       // 切换指定LED状态
void LED_Blink1(uint16_t GPIO_Pin, uint32_t delay); // LED闪烁
void LED_Blink2(uint16_t GPIO_Pin, uint32_t delay); // LED闪烁

#endif
