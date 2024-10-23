#include "lamps.h"
#include "tim.h"
#include "OLED.h"

uint8_t lamp_brightness = 0;		// 灯亮度默认为0 关闭状态


/// @brief 修改pwm值
/// @param val 
void Set_PWM_lampValue(uint8_t val)
{
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, val);
}

void lamp_Init(void)
{
	MX_TIM4_Init();
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);		// 启动定时器调制pwm波形
}

void lamp_off(void)
{
	Set_PWM_lampValue(0);
	lamp_brightness = 0;
	//OLED_ShowChineseWord(2, 11, 16);
}

void lamp_low(void)
{
	Set_PWM_lampValue(10);
	lamp_brightness = 1;
	//OLED_ShowChineseWord(2, 11, 13);
}

void lamp_Middle(void)
{
	Set_PWM_lampValue(50);
	lamp_brightness = 2;
	//OLED_ShowChineseWord(2, 11, 14);
}

void lamp_high(void)
{
	Set_PWM_lampValue(100-1);
	lamp_brightness = 3;
	//OLED_ShowChineseWord(2, 11, 15);
}

uint8_t get_lamp_brightness(void)
{
	return lamp_brightness;
}
