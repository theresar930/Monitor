#include "light_sensor.h"
#include "adc.h"

uint16_t adc_val=0 ;      
uint16_t adc_volt=0;

void LightSensor_Init(void)
{
	  GPIO_InitTypeDef GPIO_InitStruct ;

    // 使能GPIOA时钟
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitStruct.Pin = LIGHT_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LIGHT_GPIO_Port, &GPIO_InitStruct);
	
}

uint8_t LightSenso_ADC_Read(void)
{
	
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1,60);
  adc_val = HAL_ADC_GetValue(&hadc1);
  adc_volt = adc_val/4096.0f * 3.3f;
	//设置指定ADC的规则组通道，采样时间
	return adc_volt;
}
