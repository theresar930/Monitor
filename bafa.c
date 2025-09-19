#include "bafa.h"
#include <string.h>
#include <stdlib.h>
#include "mqtt_client.h"
#include "system.h"


// �ͷ�������
#define BAFA_CLIENT_ID    "your_device_id"  // �Ӱͷ��ƿ���̨��ȡ
#define BAFA_SERVER      "bemfa.com"
#define BAFA_PORT        9501
#define BAFA_TOPIC_TEMP  "your_temp_topic004"  // 004Ϊ�������豸
#define BAFA_TOPIC_MOTOR "your_motor_topic006" // 006Ϊ�����豸

static esp_mqtt_client_handle_t client;

// ��ʼ���ͷ���MQTT����
void BafangYun_Init(void) {
    esp_mqtt_client_config_t mqtt_cfg = {
        .uri = "mqtt://" BAFA_SERVER ":" BAFA_PORT,
        .client_id = BAFA_CLIENT_ID,
        .keepalive = 60  // �������
    };

    client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(client);
}

// �ϴ�����������
void BafangYun_SendSensorData(uint8_t temp, uint8_t humi, uint16_t light) {
    char payload[128];
    
    // ��ʽ1��JSON��ʽ
    snprintf(payload, sizeof(payload), 
            "{\"temp\":%.1f,\"humi\":%.1f,\"light\":%d}", 
            system_state.temperature, system_state.humidity , system_state.adc_volt);
    esp_mqtt_client_publish(client, BAFA_TOPIC_TEMP, payload, 0, 1, 0);

    // ��ʽ2���׼Ҽ��ݸ�ʽ
    snprintf(payload, sizeof(payload), "#%.1f#%.1f#%d",
			system_state.temperature, system_state.humidity , system_state.adc_volt);
    esp_mqtt_client_publish(client, BAFA_TOPIC_TEMP, payload, 0, 1, 0);
}

// �ϱ����״̬
void BafangYun_SendMotorState(uint8_t motor_a_state, uint8_t motor_b_state) {
    char payload[64];
    snprintf(payload, sizeof(payload), 
            "{\"motor_a\":%d,\"motor_b\":%d}", motor_a_state, motor_b_state);
    esp_mqtt_client_publish(client, BAFA_TOPIC_MOTOR, payload, 0, 1, 0);
}

// ָ�����
uint8_t BafangYun_CheckCommand(char *command) {
    if(strstr(command, "cmd=1")) {  // ����ָ��
        return BAFA_CMD_SUBSCRIBE;
    }
    else if(strstr(command, "motor_on")) {
        return BAFA_CMD_MOTOR_ON;
    }
    else if(strstr(command, "motor_off")) {
        return BAFA_CMD_MOTOR_OFF;
    }
    return BAFA_CMD_UNKNOWN;
}

// MQTT�¼��ص�
static void mqtt_event_handler(void *handler_args, esp_event_base_t base, 
                              int32_t event_id, void *event_data) {
    esp_mqtt_event_handle_t event = event_data;
    
    switch(event->event_id) {
        case MQTT_EVENT_CONNECTED:
            esp_mqtt_client_subscribe(client, BAFA_TOPIC_TEMP, 1);
            esp_mqtt_client_subscribe(client, BAFA_TOPIC_MOTOR, 1);
            break;
            
        case MQTT_EVENT_DATA:
            if(strncmp(event->topic, BAFA_TOPIC_MOTOR, event->topic_len) == 0) {
                char cmd[64];
                strncpy(cmd, event->data, event->data_len);
                cmd[event->data_len] = '\0';
                BafangYun_CheckCommand(cmd);
            }
            break;
            
        case MQTT_EVENT_ERROR:
            // ʵ�������߼�
            break;
    }
}