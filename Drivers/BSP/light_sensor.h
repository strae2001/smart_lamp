#ifndef __LIGHT_SENSOR_H
#define __LIGHT_SENSOR_H

#include "stdint.h"

// 光敏传感器初始化
void light_Sensor_init(void);

// 获取光照强度，范围：0~100
uint8_t light_Sensor_getValue(uint32_t channel);

#endif
