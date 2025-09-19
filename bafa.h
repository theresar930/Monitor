#ifndef __BAFA_H
#define __BAFA_H

#include "stm32f1xx_hal.h"
#include "usart.h"

typedef enum {
    BAFA_CMD_UNKNOWN = 0,
    BAFA_CMD_SUBSCRIBE,
    BAFA_CMD_MOTOR_ON,
    BAFA_CMD_MOTOR_OFF
} BafaCommandType;

void BafangYun_Init(void);
void BafangYun_SendSensorData(uint8_t temp, uint8_t humi, uint16_t light);
void BafangYun_SendMotorState(uint8_t motor_a_state, uint8_t motor_b_state);
uint8_t BafangYun_CheckCommand(char *command);

#endif

