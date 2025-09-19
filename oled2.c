#include "oled2.h"
#include <stdlib.h>
#include "fonts.h"  // 字库数据（需自行添加，后文说明）
//#include "hzk16.h"
//#include "chinese.h"

static I2C_HandleTypeDef *hi2c_oled;
static uint8_t oled_buffer[OLED_WIDTH * OLED_PAGE_NUM];
static uint8_t need_refresh = 0;

/* 私有函数 */
static void OLED_WriteCmd(uint8_t cmd) {
    uint8_t buf[2] = {0x00, cmd};
    HAL_I2C_Master_Transmit(hi2c_oled, OLED_I2C_ADDR, buf, 2, 100);
}

static void OLED_WriteData(uint8_t data) {
    uint8_t buf[2] = {0x40, data};
    HAL_I2C_Master_Transmit(hi2c_oled, OLED_I2C_ADDR, buf, 2, 100);
}

/* 初始化序列 */
const uint8_t oled_init_cmds[] = {
    0xAE, 0xD5, 0x80, 0xA8, 0x3F, 0xD3, 0x00, 0x40,
    0x8D, 0x14, 0x20, 0x00, 0xA1, 0xC8, 0xDA, 0x12,
    0x81, 0xCF, 0xD9, 0xF1, 0xDB, 0x40, 0xA4, 0xA6,
    0x2E, 0xAF
};

void OLED_Init(I2C_HandleTypeDef *hi2c) {
    hi2c_oled = hi2c;
    HAL_Delay(100);
    
    for (uint8_t i = 0; i < sizeof(oled_init_cmds); i++) {
        OLED_WriteCmd(oled_init_cmds[i]);
    }
    
    OLED_Clear();
    OLED_Refresh();
}

void OLED_Clear(void) {
    memset(oled_buffer, 0, sizeof(oled_buffer));
    need_refresh = 1;
}

void OLED_Refresh(void) {
    for (uint8_t page = 0; page < OLED_PAGE_NUM; page++) {
        OLED_WriteCmd(0xB0 | page);    // 设置页地址
        OLED_WriteCmd(0x00);           // 列低地址
        OLED_WriteCmd(0x10);           // 列高地址
        
        HAL_I2C_Mem_Write(hi2c_oled, OLED_I2C_ADDR, 0x40, 1, 
                         &oled_buffer[page*OLED_WIDTH], OLED_WIDTH, 100);
    }
    need_refresh = 0;
}

void OLED_Power(uint8_t on) {
    OLED_WriteCmd(on ? 0xAF : 0xAE);
}

/* 绘图函数 */
void OLED_DrawPixel(uint8_t x, uint8_t y, OLED_ColorMode color) {
    if (x >= OLED_WIDTH || y >= OLED_HEIGHT) return;
    
    uint16_t index = x + (y / 8) * OLED_WIDTH;
    switch (color) {
        case OLED_COLOR_NORMAL: oled_buffer[index] |=  (1 << (y % 8)); break;
        case OLED_COLOR_INVERT: oled_buffer[index] &= ~(1 << (y % 8)); break;
        case OLED_COLOR_ALL:    oled_buffer[index] =  0xFF; break;
    }
    need_refresh = 1;
}

void OLED_DrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) {
    int16_t dx = abs(x2 - x1), sx = x1 < x2 ? 1 : -1;
    int16_t dy = -abs(y2 - y1), sy = y1 < y2 ? 1 : -1;
    int16_t err = dx + dy, e2;
    
    while (1) {
        OLED_DrawPixel(x1, y1, OLED_COLOR_NORMAL);
        if (x1 == x2 && y1 == y2) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x1 += sx; }
        if (e2 <= dx) { err += dx; y1 += sy; }
    }
}

/* 文本显示 */
void OLED_PrintChar(uint8_t x, uint8_t y, char ch, uint8_t fontSize) {
    if (ch < 32 || ch > 126) ch = ' ';
    const uint8_t *font = ascii_1608x[ch - 32];
    
    for (uint8_t i = 0; i < fontSize; i++) {
        for (uint8_t j = 0; j < fontSize; j++) {
            if (font[i] & (1 << j)) {
                OLED_DrawPixel(x + j, y + i, OLED_COLOR_NORMAL);
            }
        }
    }
}

void OLED_PrintString(uint8_t x, uint8_t y, const char *str, uint8_t fontSize) {
    while (*str) {
        OLED_PrintChar(x, y, *str++, fontSize);
        x += fontSize;
        if (x > OLED_WIDTH - fontSize) {
            x = 0;
            y += fontSize;
        }
    }
}

/* 汉字显示 */
void OLED_PrintChinese(uint8_t x, uint8_t y, uint16_t gb_code) {
    int16_t index = GetGB2312Index(gb_code);
    if (index < 0) return;
    
    const uint8_t *font = tfont16[index];
    for (uint8_t i = 0; i < 16; i++) {
        for (uint8_t j = 0; j < 8; j++) {
            if (font[i*2] & (0x80 >> j)) 
                OLED_DrawPixel(x+j, y+i, OLED_COLOR_NORMAL);
            if (font[i*2+1] & (0x80 >> j)) 
                OLED_DrawPixel(x+j+8, y+i, OLED_COLOR_NORMAL);
        }
    }
}

void OLED_PrintStringCN(uint8_t x, uint8_t y, const char *str, uint8_t fontSize) {
    while (*str) {
        if (isChineseChar((uint8_t*)str)) {
            uint16_t gb_code = UTF8_TO_GB2312((uint8_t*)str);
            OLED_PrintChinese(x, y, gb_code);
            str += 3;
            x += 16;
        } else {
            OLED_PrintChar(x, y, *str++, fontSize);
            x += fontSize;
        }
        
        if (x > OLED_WIDTH - 16) {
            x = 0;
            y += 16;
        }
    }
}