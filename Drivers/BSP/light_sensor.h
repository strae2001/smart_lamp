#ifndef __LIGHT_SENSOR_H
#define __LIGHT_SENSOR_H

#include "stdint.h"

// ������������ʼ��
void light_Sensor_init(void);

// ��ȡ����ǿ�ȣ���Χ��0~100
uint8_t light_Sensor_getValue(uint32_t channel);

#endif
