#include "motor.h"
#include "tim.h"
#include "gpio.h"

void Motor_Init(void){
	
	HAL_TIM_PWM_Start(&htim1, PWMA_CHANNEL);
	HAL_TIM_PWM_Start(&htim1, PWMB_CHANNEL);
	
	Motor_A_Off();
	Motor_B_Off();

}
void Motor_A_On(void){
	HAL_GPIO_WritePin(GPIOB, AIN1_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, AIN2_Pin,GPIO_PIN_RESET);
	__HAL_TIM_SET_COMPARE(&htim1,PWMA_CHANNEL, 700); // 70%占空比
}
void Motor_A_Off(void){
	HAL_GPIO_WritePin(GPIOB, AIN1_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, AIN2_Pin,GPIO_PIN_RESET);
}
void Motor_B_On(void){
	HAL_GPIO_WritePin(GPIOA, BIN1_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, BIN2_Pin,GPIO_PIN_RESET);
	__HAL_TIM_SET_COMPARE(&htim1,PWMB_CHANNEL, 700); // 70%占空比
}
void Motor_B_Off(void){
	HAL_GPIO_WritePin(GPIOA, BIN1_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, BIN2_Pin,GPIO_PIN_RESET);
}

