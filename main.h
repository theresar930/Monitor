/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED_R_Pin GPIO_PIN_1
#define LED_R_GPIO_Port GPIOA
#define LED_G_Pin GPIO_PIN_2
#define LED_G_GPIO_Port GPIOA
#define LED_B_Pin GPIO_PIN_3
#define LED_B_GPIO_Port GPIOA
#define LIGHT_Pin GPIO_PIN_4
#define LIGHT_GPIO_Port GPIOA
#define DHT11_Pin GPIO_PIN_5
#define DHT11_GPIO_Port GPIOA
#define EC11_A_Pin GPIO_PIN_6
#define EC11_A_GPIO_Port GPIOA
#define EC11_B_Pin GPIO_PIN_7
#define EC11_B_GPIO_Port GPIOA
#define BEEP_Pin GPIO_PIN_1
#define BEEP_GPIO_Port GPIOB
#define DO_Pin GPIO_PIN_11
#define DO_GPIO_Port GPIOB
#define SW_Pin GPIO_PIN_12
#define SW_GPIO_Port GPIOB
#define AIN1_Pin GPIO_PIN_13
#define AIN1_GPIO_Port GPIOB
#define PWMB_Pin GPIO_PIN_14
#define PWMB_GPIO_Port GPIOB
#define STBY_Pin GPIO_PIN_15
#define STBY_GPIO_Port GPIOB
#define PWMA_Pin GPIO_PIN_8
#define PWMA_GPIO_Port GPIOA
#define BIN1_Pin GPIO_PIN_11
#define BIN1_GPIO_Port GPIOA
#define BIN2_Pin GPIO_PIN_12
#define BIN2_GPIO_Port GPIOA
#define SLED_Pin GPIO_PIN_15
#define SLED_GPIO_Port GPIOA
#define AIN2_Pin GPIO_PIN_3
#define AIN2_GPIO_Port GPIOB
#define ESP_RET_Pin GPIO_PIN_4
#define ESP_RET_GPIO_Port GPIOB
#define ESP_IO_Pin GPIO_PIN_5
#define ESP_IO_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

#define PWMA_TIM TIM1
#define PWMA_CHANNEL TIM_CHANNEL_1
#define PWMB_TIM TIM1
#define PWMB_CHANNEL TIM_CHANNEL_2

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
