#include "ec11.h"
#include "tim.h"
#include "gpio.h"

static uint8_t last_clk_state = 1;
static uint8_t last_pressed_state = 1;
static uint32_t last_pressed_time = 0;

void EC11_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    __HAL_RCC_GPIOA_CLK_ENABLE();
    
    // ����CLK��DT����Ϊ����
    GPIO_InitStruct.Pin = EC11_A_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(EC11_A_GPIO_Port, &GPIO_InitStruct);
		
		GPIO_InitStruct.Pin = EC11_B_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(EC11_B_GPIO_Port, &GPIO_InitStruct);
    
    // ����SW����Ϊ����
    GPIO_InitStruct.Pin = SW_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(SW_GPIO_Port, &GPIO_InitStruct);
}

 EC11_State EC11_Get_State(void) {
    EC11_State state = {0};
    uint8_t current_clk_state = HAL_GPIO_ReadPin(EC11_A_GPIO_Port, EC11_A_Pin);
    uint8_t current_pressed_state = HAL_GPIO_ReadPin(SW_GPIO_Port, SW_Pin);
    
   // ��ⰴť���£���������
    if (current_pressed_state == 0 && last_pressed_state == 1) {
        if (HAL_GetTick() - last_pressed_time > 50) { // 50ms����
            state.pressed = 1;
            last_pressed_time = HAL_GetTick();
        }
    }
    last_pressed_state = current_pressed_state;

    // �����ת����
    if (current_clk_state != last_clk_state) {
        if (HAL_GPIO_ReadPin(EC11_B_GPIO_Port, EC11_B_Pin) != current_clk_state) {
            state.direction = 1; // ˳ʱ��
        } else {
            state.direction = -1; // ��ʱ��
        }
        last_clk_state = current_clk_state;
    }
    
    return state;
}
 
