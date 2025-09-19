#include "led.h"
#include "gpio.h"


/**
  * @brief  LED��ʼ������
  * @param  ��
  * @retval ��
  */
void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    // ����GPIOBʱ��
    __HAL_RCC_GPIOA_CLK_ENABLE();
    
    // ����GPIOB�ĵ�0������Ϊ���ģʽ
    GPIO_InitStruct.Pin = LED_R_Pin|LED_G_Pin|LED_B_Pin|SLED_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/**
  * @brief  ����ָ��LED
  * @param  GPIO_Pin: Ҫ���Ƶ�LED���ź�
  * @retval ��
  */
void LED_On(uint16_t GPIO_Pin)
{
    HAL_GPIO_WritePin(GPIOA, GPIO_Pin, GPIO_PIN_RESET);
}

/**
  * @brief  Ϩ��ָ��LED
  * @param  GPIO_Pin: Ҫ���Ƶ�LED���ź�
  * @retval ��
  */
void LED_Off(uint16_t GPIO_Pin)
{
    HAL_GPIO_WritePin(GPIOA, GPIO_Pin, GPIO_PIN_SET);
}

/**  * @brief  �л�ָ��LED״̬
  * @param  GPIO_Pin: Ҫ���Ƶ�LED���ź�
  * @retval ��
  */
void LED_Toggle(uint16_t GPIO_Pin)
{
    HAL_GPIO_TogglePin(GPIOA, GPIO_Pin);
}

/**
  * @brief  LED�������˸����
  * @param  GPIO_Pin: Ҫ���Ƶ�LED���ź�
  * @param  delay: ��˸���(ms)
  * @retval ��
  */
void LED_Blink1(uint16_t GPIO_Pin, uint32_t delay)
{
		LED_Toggle(GPIO_Pin);
		HAL_Delay(delay);
}

/**
  * @brief  LEDһ����˸����
  * @param  GPIO_Pin: Ҫ���Ƶ�LED���ź�
  * @param  delay: ��˸���(ms)
  * @retval ��
  */
void LED_Blink2(uint16_t GPIO_Pin, uint32_t delay)
{
			
		LED_Toggle(GPIO_Pin);
		HAL_Delay(delay);
		LED_Toggle(GPIO_Pin);
		HAL_Delay(delay);	
}
