#ifndef __EC11_H
#define __EC11_H

#include "stm32f1xx_hal.h"

typedef struct {
    int8_t  direction;  // 0:����ת, 1:˳ʱ��, -1:��ʱ��
    uint8_t pressed; // 1:��ť����, 0:δ����
} EC11_State;

void EC11_Init(void);
EC11_State EC11_Get_State(void);

#endif

