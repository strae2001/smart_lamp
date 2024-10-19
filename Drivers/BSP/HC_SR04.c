#include "HC_SR04.h"
#include "tim.h"
#include "gpio.h"

double distance;

// 超声波模块初始化
void HCSR04_Init(void)
{
    MX_TIM1_Init();        // 定时器初始化
    MX_GPIO_Init();		   // 对应GPIO初始化
}

// 开始发送超声波
void HC_start(void)
{
	Trige_LOW();        // Trige Echo默认置低电平
    Echo_LOW(); 
	
    Trige_HIGH();
    HAL_Delay(18);       // 给Trige至少15us高电平 触发trige发送声波
    Trige_LOW();  

	__HAL_TIM_SetCounter(&htim1, 0);        // 清0计数器
}

double get_distance(void)
{
    uint16_t total_time;
    HC_start();
    while (Echo_State() == GPIO_PIN_RESET);         
    timer1_start();               			// 待Echo跳变至高电平时 声波开始发送 定时器开始计时
	
    while (Echo_State() == GPIO_PIN_SET);
    timer1_stop();                			// 待Echo跳变至低电平时 声波接收完毕 定时器结束计时

    //total_time = __HAL_TIM_GetCounter(&htim1);
    total_time = __HAL_TIM_GET_COUNTER(&htim1);
    distance = total_time * 1.0 / 2 * 0.0343;		// 单位cm 
	
    return distance;
}
