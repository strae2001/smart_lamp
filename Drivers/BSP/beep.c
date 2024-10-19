#include "beep.h"
#include "gpio.h"

//初始化GPIO函数
void beep_init(void)
{
    //关闭蜂鸣器
    beep_off();
}

//打开蜂鸣器的函数
void beep_on(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);   // 拉低蜂鸣器引脚，打开蜂鸣器
}

//关闭蜂鸣器的函数
void beep_off(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);     // 拉高蜂鸣器引脚，关闭蜂鸣器
}
