#ifndef __KEY_H
#define __KEY_H

#define KEY1_GPIO_PORT              GPIOA
#define KEY1_GPIO_PIN               GPIO_PIN_0
#define KEY1_GPIO_CLK_ENABLE()      do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)

#define KEY2_GPIO_PORT              GPIOA
#define KEY2_GPIO_PIN               GPIO_PIN_1
#define KEY2_GPIO_CLK_ENABLE()      do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)

#define KEY3_GPIO_PORT              GPIOA
#define KEY3_GPIO_PIN               GPIO_PIN_6
#define KEY3_GPIO_CLK_ENABLE()      do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)

#define KEY4_GPIO_PORT              GPIOA
#define KEY4_GPIO_PIN               GPIO_PIN_7
#define KEY4_GPIO_CLK_ENABLE()      do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)

#define KEY1                        HAL_GPIO_ReadPin(KEY1_GPIO_PORT, KEY1_GPIO_PIN)
#define KEY2                        HAL_GPIO_ReadPin(KEY2_GPIO_PORT, KEY2_GPIO_PIN)
#define KEY3                        HAL_GPIO_ReadPin(KEY3_GPIO_PORT, KEY3_GPIO_PIN)
#define KEY4                        HAL_GPIO_ReadPin(KEY4_GPIO_PORT, KEY4_GPIO_PIN)

#include "stdint.h"

void key_init(void);
uint8_t key_check(void);

#endif
