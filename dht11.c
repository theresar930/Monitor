#include "dht11.h"
#include "adc.h"

	extern TIM_HandleTypeDef htim1;
	
	uint8_t *temp=&temperature;
	uint8_t *humi=&humidity;
  uint8_t temperature=0;
	uint8_t humidity=0;
void DHT11_Init(void)
{
	 // 使能GPIOA时钟
    __HAL_RCC_GPIOA_CLK_ENABLE();
	
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    // 配置引脚为输出模式
    GPIO_InitStruct.Pin = DHT11_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(DHT11_PORT, &GPIO_InitStruct);
//		HAL_Delay(100); 
		HAL_GPIO_WritePin(DHT11_PORT, DHT11_GPIO_PIN, GPIO_PIN_SET); // 初始状态为高电平
}

void Delay_us(uint16_t us) {
    uint16_t differ = 0xffff - us - 5;
    __HAL_TIM_SET_COUNTER(&htim1, differ);
    HAL_TIM_Base_Start(&htim1);
    while(__HAL_TIM_GET_COUNTER(&htim1) < 0xffff - 5);
    HAL_TIM_Base_Stop(&htim1);
}

  HAL_StatusTypeDef DHT11_ReadData(uint8_t *temp, uint8_t *humi)
	{
		uint8_t data[5] = {0};
//	  uint8_t index;
//	  uint8_t temperature;
//	  uint8_t humidity;
		// DHT11读取数据函数实现
    HAL_StatusTypeDef status = HAL_OK;
    uint8_t i, j;
    // 主机发送起始信号
    HAL_GPIO_WritePin(DHT11_PORT, DHT11_GPIO_PIN, GPIO_PIN_RESET);
    HAL_Delay(20);  // 拉低至少18ms
    HAL_GPIO_WritePin(DHT11_PORT, DHT11_GPIO_PIN, GPIO_PIN_SET);
    Delay_us(30);  // 拉高等待20 - 40us

    // 设置引脚为输入模式，准备接收数据
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = DHT11_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    HAL_GPIO_Init(DHT11_PORT, &GPIO_InitStruct);

    // 等待DHT11响应
    if (HAL_GPIO_ReadPin(DHT11_PORT, DHT11_GPIO_PIN) == GPIO_PIN_RESET)
    {
        // 等待DHT11拉高
        while (HAL_GPIO_ReadPin(DHT11_PORT, DHT11_GPIO_PIN) == GPIO_PIN_RESET);
        // 等待DHT11拉低
        while (HAL_GPIO_ReadPin(DHT11_PORT, DHT11_GPIO_PIN) == GPIO_PIN_SET);

        // 读取5个字节数据
        for (i = 0; i < 5; i++)
        {
            for (j = 0; j < 8; j++)
            {
                // 等待引脚拉高
                while (HAL_GPIO_ReadPin(DHT11_PORT, DHT11_GPIO_PIN) == GPIO_PIN_RESET);
                Delay_us(30);
                if (HAL_GPIO_ReadPin(DHT11_PORT, DHT11_GPIO_PIN) == GPIO_PIN_SET)
                {
                    data[i] |= (1 << (7 - j));
                    while (HAL_GPIO_ReadPin(DHT11_PORT, DHT11_GPIO_PIN) == GPIO_PIN_SET);
                }
            }
        }

        // 校验数据
        if (data[0] + data[1] + data[2] + data[3] == data[4])
        {
            *humi = data[0];
            *temp = data[2];
        }
        else
        {
            status = HAL_ERROR;
        }
    }
    else
    {
        status = HAL_ERROR;
    }

    // 重新配置引脚为输出模式
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(DHT11_PORT, &GPIO_InitStruct);

    return status;
} 


