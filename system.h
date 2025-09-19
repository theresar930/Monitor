#ifndef __SYSTEM_H
#define __SYSTEM_H

#include "stm32f1xx_hal.h"
#include "main.h"

//#include "main.h"
// 系统模式定义
typedef enum {
    AUTO_MODE = 0,
    MANUAL_MODE
} SystemMode;

// 手动模式页面定义
typedef enum {
    MANUAL_PAGE_LED = 0,
    MANUAL_PAGE_MOTOR_A,
    MANUAL_PAGE_MOTOR_B
} ManualPage;

// 系统状态结构体
typedef struct {
    SystemMode current_mode;
    ManualPage manual_page;
    uint8_t led_state;
    uint8_t motor_a_state;
    uint8_t motor_b_state;
    uint8_t temperature;
    uint8_t humidity;
    uint16_t adc_volt;
    uint16_t light_threshold;
    uint8_t temp_threshold1;
    uint8_t temp_threshold2;
    uint8_t humi_threshold;
} SystemState;

// 函数声明
void System_Init(void);
void System_Update(void);
void System_HandleEncoderAction(void);
void System_SwitchMode(SystemMode new_mode);
void System_SwitchManualPage(ManualPage new_page);
void System_UpdateSensorData(void);
void System_HandleAutoMode(void);
void System_SendDataToCloud(void);

extern SystemState system_state;

#endif

