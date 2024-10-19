#ifndef __HCSR04_H
#define __HCSR04_H

#include "stdint.h"

// Trige Echo��������
#define Trige_GPIO      GPIOA
#define Trige_PIN       GPIO_PIN_11
#define Trige_HIGH()    HAL_GPIO_WritePin(Trige_GPIO, Trige_PIN, GPIO_PIN_SET)
#define Trige_LOW()     HAL_GPIO_WritePin(Trige_GPIO, Trige_PIN, GPIO_PIN_RESET)

#define Echo_GPIO       GPIOA
#define Echo_PIN        GPIO_PIN_12
#define Echo_HIGH()     HAL_GPIO_WritePin(Echo_GPIO, Echo_PIN, GPIO_PIN_SET)
#define Echo_LOW()      HAL_GPIO_WritePin(Echo_GPIO, Echo_PIN, GPIO_PIN_RESET)
#define Echo_State()    HAL_GPIO_ReadPin(Echo_GPIO, Echo_PIN)

// ������ģ���ʼ��
void HCSR04_Init(void);

// ��ȡ����
double get_distance(void);

#endif
