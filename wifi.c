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
    // ����ATָ���ʼ��WiFiģ��
    HAL_UART_Transmit(&huart1,(uint8_t*)"AT\r\n", 4, 100);
    HAL_Delay(1000);
    HAL_UART_Transmit(&huart1, (uint8_t*)"AT+CWMODE=1\r\n", 13, 100);
    HAL_Delay(1000);
    // ����WiFi����
    sprintf(wifiBuffer, "AT+CWJAP=\"%s\",\"%s\"\r\n", "ESP8266", "WIFI_PASSWORD");
    HAL_UART_Transmit(&huart1, (uint8_t*)wifiBuffer, strlen(wifiBuffer), 100);
    HAL_Delay(5000);
    // ���Ӱͷ���
    HAL_UART_Transmit(&huart1, (uint8_t*)"AT+CIPSTART=\"TCP\",\"bemfa.com\",8344\r\n", 36, 100);
    HAL_Delay(2000);
    wifiConnected = 1;
}

void WiFi_Send_Data(void) {
    if (!wifiConnected) return;
    
    // ���ʹ���������
    sprintf(wifiBuffer, "{\"temp\":%.d,\"humi\":%.d,\"light\":%d}\r\n", 
            system_state.temperature, system_state.humidity , system_state.adc_volt);
    HAL_UART_Transmit(&huart1, (uint8_t*)wifiBuffer, strlen(wifiBuffer), 100);
}

void Handle_WiFi_Communication(void) {
    static uint32_t lastSendTime = 0;
    uint32_t currentTime = HAL_GetTick();
    
    // ���ڷ�������
    if (currentTime - lastSendTime > 5000) { // 5�뷢��һ��
        WiFi_Send_Data();
        lastSendTime = currentTime;
    }
    
    // ���մ���С����ָ��
    uint8_t rxData;
    if (HAL_UART_Receive(&huart1, &rxData, 1, 10) == HAL_OK) {
        // ����С����ָ��
        // ʾ��: "mode:auto" �� "mode:manual"
        // ʾ��: "setTemp1:30.0"
        // ����ָ�����ϵͳ״̬����ֵ
    }
}
