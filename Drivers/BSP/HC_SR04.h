#ifndef __HCSR04_H
#define __HCSR04_H

#include "stdint.h"

// Trige Echo引脚配置
#define Trige_GPIO      GPIOA
#define Trige_PIN       GPIO_PIN_11
#define Trige_HIGH()    HAL_GPIO_WritePin(Trige_GPIO, Trige_PIN, GPIO_PIN_SET)
#define Trige_LOW()     HAL_GPIO_WritePin(Trige_GPIO, Trige_PIN, GPIO_PIN_RESET)

#define Echo_GPIO       GPIOA
#define Echo_PIN        GPIO_PIN_12
#define Echo_HIGH()     HAL_GPIO_WritePin(Echo_GPIO, Echo_PIN, GPIO_PIN_SET)
#define Echo_LOW()      HAL_GPIO_WritePin(Echo_GPIO, Echo_PIN, GPIO_PIN_RESET)
#define Echo_State()    HAL_GPIO_ReadPin(Echo_GPIO, Echo_PIN)

// 超声波模块初始化
void HCSR04_Init(void);

// 获取距离
double get_distance(void);

#endif
