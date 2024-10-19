#ifndef __BLUETOOTH_H
#define __BLUETOOTH_H

#include "stdint.h"

#define TX2 GPIO_PIN_2
#define RX2 GPIO_PIN_3

#define Rx2BUF_SIZE 128

void BlueTooth_Init(void);

void Bluetooth_sendString(char* str);

// 获取蓝牙发送的指令，提供给外部函数调用
uint8_t bt_rx_getCmd(void);

#endif
