#ifndef __OLED2_H
#define __OLED2_H

#include "stm32f1xx_hal.h"
#include <stdint.h>
#include <string.h>

/* OLED配置 */
#define OLED_I2C_ADDR    0x78
#define OLED_WIDTH       128
#define OLED_HEIGHT      64
#define OLED_PAGE_NUM    (OLED_HEIGHT/8)

/* 颜色模式 */
typedef enum {
    OLED_COLOR_NORMAL = 0,  // 正常显示
    OLED_COLOR_INVERT = 1,  // 反色显示
    OLED_COLOR_ALL    = 2   // 全亮（测试用）
} OLED_ColorMode;

/* 初始化与基础函数 */
void OLED_Init(I2C_HandleTypeDef *hi2c);
void OLED_Clear(void);
void OLED_Refresh(void);
void OLED_Power(uint8_t on);

/* 绘图API */
void OLED_DrawPixel(uint8_t x, uint8_t y, OLED_ColorMode color);
void OLED_DrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
void OLED_DrawRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t fill);

/* 文本显示 */
void OLED_PrintChar(uint8_t x, uint8_t y, char ch, uint8_t fontSize);
void OLED_PrintString(uint8_t x, uint8_t y, const char *str, uint8_t fontSize);
void OLED_PrintChinese(uint8_t x, uint8_t y, uint16_t gb_code);
void OLED_PrintStringCN(uint8_t x, uint8_t y, const char *str, uint8_t fontSize);

#endif