#ifndef __IA_SENSOR_H
#define __IA_SENSOR_H

#include "stdint.h"

#define     TRUE    1
#define     FALSE   0

// 红外模块初始化
void ia_init(void);

// 获取红外检测结果 低电平则有人 高电平则无人
uint8_t ia_flag_get(void);

#endif
