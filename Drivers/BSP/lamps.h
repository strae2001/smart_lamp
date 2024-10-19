#ifndef __LAMPS_H
#define __LAMPS_H

#include "stdint.h"

#define LAMP	GPIO_PIN_8

void Set_PWM_lampValue(uint8_t val);

// 初始化台灯
void lamp_Init(void);

// 关闭台灯
void lamp_off(void);

// 台灯低亮度开启
void lamp_low(void);

// 台灯中亮度开启
void lamp_Middle(void);

// 台灯高亮度开启
void lamp_high(void);

// 获取台灯亮度
uint8_t get_lamp_brightness(void);

#endif
