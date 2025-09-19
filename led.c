#include "led.h"
#include "gpio.h"


/**
  * @brief  LED初始化函数
  * @param  无
  * @retval 无
  */
void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    // 启用GPIOB时钟
    __HAL_RCC_GPIOA_CLK_ENABLE();
    
    // 配置GPIOB的第0号引脚为输出模式
    GPIO_InitStruct.Pin = LED_R_Pin|LED_G_Pin|LED_B_Pin|SLED_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/**
  * @brief  点亮指定LED
  * @param  GPIO_Pin: 要控制的LED引脚号
  * @retval 无
  */
void LED_On(uint16_t GPIO_Pin)
{
    HAL_GPIO_WritePin(GPIOA, GPIO_Pin, GPIO_PIN_RESET);
}

/**
  * @brief  熄灭指定LED
  * @param  GPIO_Pin: 要控制的LED引脚号
  * @retval 无
  */
void LED_Off(uint16_t GPIO_Pin)
{
    HAL_GPIO_WritePin(GPIOA, GPIO_Pin, GPIO_PIN_SET);
}

/**  * @brief  切换指定LED状态
  * @param  GPIO_Pin: 要控制的LED引脚号
  * @retval 无
  */
void LED_Toggle(uint16_t GPIO_Pin)
{
    HAL_GPIO_TogglePin(GPIOA, GPIO_Pin);
}

/**
  * @brief  LED不间断闪烁函数
  * @param  GPIO_Pin: 要控制的LED引脚号
  * @param  delay: 闪烁间隔(ms)
  * @retval 无
  */
void LED_Blink1(uint16_t GPIO_Pin, uint32_t delay)
{
		LED_Toggle(GPIO_Pin);
		HAL_Delay(delay);
}

/**
  * @brief  LED一次闪烁函数
  * @param  GPIO_Pin: 要控制的LED引脚号
  * @param  delay: 闪烁间隔(ms)
  * @retval 无
  */
void LED_Blink2(uint16_t GPIO_Pin, uint32_t delay)
{
			
		LED_Toggle(GPIO_Pin);
		HAL_Delay(delay);
		LED_Toggle(GPIO_Pin);
		HAL_Delay(delay);	
}
