#ifndef __MOTOR_H
#define __MOTOR_H

#include "stm32f1xx_hal.h"

extern TIM_HandleTypeDef htim1;  // ºŸ…Ë π”√TIM1

void Motor_Init(void);
void Motor_A_On(void);
void Motor_A_Off(void);
void Motor_B_On(void);
void Motor_B_Off(void);

#endif

