//#include "usart.h"
#include "wifi.h"
//#include "bafa.h"
#include "main.h"
#include "system.h"
#include "stdio.h"
#include <string.h>

extern UART_HandleTypeDef huart1;
char wifiBuffer[128];
uint8_t wifiConnected = 0;

void WIFI_Init(void) {
    // 发送AT指令初始化WiFi模块
    HAL_UART_Transmit(&huart1,(uint8_t*)"AT\r\n", 4, 100);
    HAL_Delay(1000);
    HAL_UART_Transmit(&huart1, (uint8_t*)"AT+CWMODE=1\r\n", 13, 100);
    HAL_Delay(1000);
    // 连接WiFi网络
    sprintf(wifiBuffer, "AT+CWJAP=\"%s\",\"%s\"\r\n", "ESP8266", "WIFI_PASSWORD");
    HAL_UART_Transmit(&huart1, (uint8_t*)wifiBuffer, strlen(wifiBuffer), 100);
    HAL_Delay(5000);
    // 连接巴法云
    HAL_UART_Transmit(&huart1, (uint8_t*)"AT+CIPSTART=\"TCP\",\"bemfa.com\",8344\r\n", 36, 100);
    HAL_Delay(2000);
    wifiConnected = 1;
}

void WiFi_Send_Data(void) {
    if (!wifiConnected) return;
    
    // 发送传感器数据
    sprintf(wifiBuffer, "{\"temp\":%.d,\"humi\":%.d,\"light\":%d}\r\n", 
            system_state.temperature, system_state.humidity , system_state.adc_volt);
    HAL_UART_Transmit(&huart1, (uint8_t*)wifiBuffer, strlen(wifiBuffer), 100);
}

void Handle_WiFi_Communication(void) {
    static uint32_t lastSendTime = 0;
    uint32_t currentTime = HAL_GetTick();
    
    // 定期发送数据
    if (currentTime - lastSendTime > 5000) { // 5秒发送一次
        WiFi_Send_Data();
        lastSendTime = currentTime;
    }
    
    // 接收处理小程序指令
    uint8_t rxData;
    if (HAL_UART_Receive(&huart1, &rxData, 1, 10) == HAL_OK) {
        // 解析小程序指令
        // 示例: "mode:auto" 或 "mode:manual"
        // 示例: "setTemp1:30.0"
        // 根据指令更新系统状态和阈值
    }
}
