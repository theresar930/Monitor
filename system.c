#include "system.h"
#include "dht11.h"
#include "led.h"
#include "oled1.h"
#include "ec11.h"
#include "wifi.h"
#include "light_sensor.h"
#include "motor.h"
//#include "bafa.h"


SystemState system_state = {
    .current_mode = AUTO_MODE,
    .manual_page = MANUAL_PAGE_LED,
    .led_state = 0,
    .motor_a_state = 0,
    .motor_b_state = 0,
    .temperature = 0,
    .humidity = 0,
    .adc_volt = 0,
    .light_threshold = 500,
    .temp_threshold1 = 30,
    .temp_threshold2 = 36,
    .humi_threshold = 60
};

void System_Init(void) {
    // 初始化各模块
    LED_Init();
    DHT11_Init();
    OLED_Init();
    EC11_Init();
    WIFI_Init();
    LightSensor_Init();
    Motor_Init();
//    BafangYun_Init();
    // 显示主界面
    

}

void System_Update(void) {
    // 更新传感器数据
    System_UpdateSensorData();
//    HAL_Delay(1000);

    // 根据当前模式处理逻辑
    if (system_state.current_mode == AUTO_MODE) {
        System_HandleAutoMode();
    }
    
    // 定期发送数据到云端
//    static uint32_t last_send_time = 0;
//    if (HAL_GetTick() - last_send_time > 5000) { // 每5秒发送一次
//        System_SendDataToCloud();
//        last_send_time = HAL_GetTick();
//    }
		LED_Toggle(LED_G_Pin);
}

void System_UpdateSensorData(void) {
    // 读取温湿度
    uint8_t temp, humi; 
    if (DHT11_ReadData(&temp, &humi) == HAL_OK) {
        system_state.temperature = temp;
        system_state.humidity = humi;
    }
    
    // 读取光照强度
    system_state.adc_volt = LightSenso_ADC_Read();
}

void System_HandleAutoMode(void) {
    static uint32_t motor_b_start_time = 0;
    static uint8_t motor_b_running = 0;
    
//	  System_SendDataToCloud();
	
    // 光照强度低于阈值时不操作
    if (system_state.adc_volt < system_state.light_threshold) {
        if (system_state.motor_a_state) {
            Motor_A_Off();
            system_state.motor_a_state = 0;
//            BafangYun_SendMotorState(0, 0);
        }
        return;
    }
    
    // 温度高于第一阈值，开启电机A
    if (system_state.temperature > system_state.temp_threshold1) {
        if (!system_state.motor_a_state) {
            Motor_A_On();
            system_state.motor_a_state = 1;
//            BafangYun_SendMotorState(1, 0);
        }
    } 
    // 温度低于等于第一阈值，关闭电机A
    else if (system_state.temperature <= system_state.temp_threshold1) {
        if (system_state.motor_a_state) {
            Motor_A_Off();
            system_state.motor_a_state = 0;
//            BafangYun_SendMotorState(0, 0);
        }
    }
    
    // 温度高于第二阈值，开启电机A和B
    if (system_state.temperature > system_state.temp_threshold2) {
        if (!system_state.motor_a_state) {
            Motor_A_On();
            system_state.motor_a_state = 1;
//            BafangYun_SendMotorState(1, 0);
        }
        
        if (!motor_b_running) {
            Motor_B_On();
            system_state.motor_b_state = 1;
            motor_b_running = 1;
            motor_b_start_time = HAL_GetTick();
//            BafangYun_SendMotorState(1, 1);
        }
    }
    
    // 5分钟后关闭电机B
    if (motor_b_running && (HAL_GetTick() - motor_b_start_time > 300000)) {
        Motor_B_Off();
        system_state.motor_b_state = 0;
        motor_b_running = 0;
//        BafangYun_SendMotorState(1, 0);
    }
//		LED_Toggle(LED_B_Pin);
}

//void System_SendDataToCloud(void) {
//    BafangYun_SendSensorData(system_state.temperature, system_state.humidity, system_state.adc_volt);
//}

void System_HandleEncoderAction(void) {
	  EC11_State state = EC11_Get_State();
      // 按下旋钮切换模式
    if (system_state.current_mode == AUTO_MODE) {
        if(state.pressed) {
            System_SwitchMode(MANUAL_MODE);
        }
        else { 
            System_SwitchMode(AUTO_MODE);
        }
        LED_Blink2(LED_R_Pin,100);
    } 
    else if (state.direction != 0 && system_state.current_mode == MANUAL_MODE) {
        // 在手动模式下，左右旋转切换页面
        ManualPage new_page = system_state.manual_page;
        
        if (state.direction > 0) { // 右旋
            new_page = (new_page + 1) % 3;
            LED_Blink2(LED_G_Pin,100);
        } 
        else if(state.direction < 0) { // 左旋
            if (new_page == MANUAL_PAGE_LED) {
                // 在第一页面左旋返回自动模式
                System_SwitchMode(AUTO_MODE);
                return;
            }
            new_page = (new_page - 1 + 3) % 3;
           LED_Blink2(LED_B_Pin,100);
        }
        System_SwitchManualPage(new_page);
    }
		else if (state.pressed && system_state.current_mode == MANUAL_MODE) {
        // 在手动模式下，按下旋钮控制当前页面的设备
        switch (system_state.manual_page) {
            case MANUAL_PAGE_LED:
                system_state.led_state = !system_state.led_state;
                LED_Blink2(LED_R_Pin,100);
        // 控制LED灯					
				if (system_state.led_state) {
                    LED_On(SLED_Pin);
                    system_state.led_state = 1;
                } else {
                    LED_Off(SLED_Pin);
                    system_state.led_state = 0;
                }
                break;
            case MANUAL_PAGE_MOTOR_A:
                system_state.motor_a_state = !system_state.motor_a_state;
								LED_Blink2(LED_R_Pin,100);
                if (system_state.motor_a_state) {
                    Motor_A_On();
                } else {
                    Motor_A_Off();
                }
//                BafangYun_SendMotorState(system_state.motor_a_state, system_state.motor_b_state);
                break;
            case MANUAL_PAGE_MOTOR_B:
                system_state.motor_b_state = !system_state.motor_b_state;
								LED_Blink2(LED_R_Pin,100);
                if (system_state.motor_b_state) {
                    Motor_B_On();
                } else {
                    Motor_B_Off();
                }
//                BafangYun_SendMotorState(system_state.motor_a_state, system_state.motor_b_state);
                break;
        }
    }
		LED_Toggle(LED_R_Pin);
}

void System_SwitchMode(SystemMode new_mode) {
    system_state.current_mode = new_mode;
    
    if (new_mode == AUTO_MODE) {
        OLED_Auto_Screen();
    } else {
        System_SwitchManualPage(MANUAL_PAGE_LED);
    }
}

void System_SwitchManualPage(ManualPage new_page) {
    system_state.manual_page = new_page;
    OLED_Manual_Mode_Screen();
}



