#ifndef __EC11_H
#define __EC11_H

#include "stm32f1xx_hal.h"

typedef struct {
    int8_t  direction;  // 0:无旋转, 1:顺时针, -1:逆时针
    uint8_t pressed; // 1:按钮按下, 0:未按下
} EC11_State;

void EC11_Init(void);
EC11_State EC11_Get_State(void);

#endif

