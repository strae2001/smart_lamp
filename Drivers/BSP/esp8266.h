#ifndef _ESP8266_H_
#define _ESP8266_H_

#define TX3		GPIO_PIN_10
#define RX3		GPIO_PIN_11

#define REV_OK		0	//������ɱ�־
#define REV_WAIT	1	//����δ��ɱ�־

#include "stdint.h"

void ESP8266_Init(void);

void ESP8266_Clear(void);

void ESP8266_SendData(unsigned char *data, unsigned short len);

unsigned char *ESP8266_GetIPD(unsigned short timeOut);

void sendString_toEsp8266(unsigned char* str, uint16_t len);

#endif
