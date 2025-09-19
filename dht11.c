#include "dht11.h"
#include "adc.h"

	extern TIM_HandleTypeDef htim1;
	
	uint8_t *temp=&temperature;
	uint8_t *humi=&humidity;
  uint8_t temperature=0;
	uint8_t humidity=0;
void DHT11_Init(void)
{
	 // ʹ��GPIOAʱ��
    __HAL_RCC_GPIOA_CLK_ENABLE();
	
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    // ��������Ϊ���ģʽ
    GPIO_InitStruct.Pin = DHT11_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(DHT11_PORT, &GPIO_InitStruct);
//		HAL_Delay(100); 
		HAL_GPIO_WritePin(DHT11_PORT, DHT11_GPIO_PIN, GPIO_PIN_SET); // ��ʼ״̬Ϊ�ߵ�ƽ
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
		// DHT11��ȡ���ݺ���ʵ��
    HAL_StatusTypeDef status = HAL_OK;
    uint8_t i, j;
    // ����������ʼ�ź�
    HAL_GPIO_WritePin(DHT11_PORT, DHT11_GPIO_PIN, GPIO_PIN_RESET);
    HAL_Delay(20);  // ��������18ms
    HAL_GPIO_WritePin(DHT11_PORT, DHT11_GPIO_PIN, GPIO_PIN_SET);
    Delay_us(30);  // ���ߵȴ�20 - 40us

    // ��������Ϊ����ģʽ��׼����������
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = DHT11_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    HAL_GPIO_Init(DHT11_PORT, &GPIO_InitStruct);

    // �ȴ�DHT11��Ӧ
    if (HAL_GPIO_ReadPin(DHT11_PORT, DHT11_GPIO_PIN) == GPIO_PIN_RESET)
    {
        // �ȴ�DHT11����
        while (HAL_GPIO_ReadPin(DHT11_PORT, DHT11_GPIO_PIN) == GPIO_PIN_RESET);
        // �ȴ�DHT11����
        while (HAL_GPIO_ReadPin(DHT11_PORT, DHT11_GPIO_PIN) == GPIO_PIN_SET);

        // ��ȡ5���ֽ�����
        for (i = 0; i < 5; i++)
        {
            for (j = 0; j < 8; j++)
            {
                // �ȴ���������
                while (HAL_GPIO_ReadPin(DHT11_PORT, DHT11_GPIO_PIN) == GPIO_PIN_RESET);
                Delay_us(30);
                if (HAL_GPIO_ReadPin(DHT11_PORT, DHT11_GPIO_PIN) == GPIO_PIN_SET)
                {
                    data[i] |= (1 << (7 - j));
                    while (HAL_GPIO_ReadPin(DHT11_PORT, DHT11_GPIO_PIN) == GPIO_PIN_SET);
                }
            }
        }

        // У������
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

    // ������������Ϊ���ģʽ
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(DHT11_PORT, &GPIO_InitStruct);

    return status;
} 


