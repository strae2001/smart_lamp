#ifndef __IA_SENSOR_H
#define __IA_SENSOR_H

#include "stdint.h"

#define     TRUE    1
#define     FALSE   0

// ����ģ���ʼ��
void ia_init(void);

// ��ȡ�������� �͵�ƽ������ �ߵ�ƽ������
uint8_t ia_flag_get(void);

#endif
