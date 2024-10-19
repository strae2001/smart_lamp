#ifndef __OLED_H
#define __OLED_H

#include "stdint.h"

void OLED_Init(void);
void OLED_Clear(void);
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char);
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String);
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length);
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);

// ÏÔÊ¾ºº×Ö
// N: ºº×Ö×Ö¿âÐÐºÅ
void OLED_ShowChineseWord(uint8_t Line, uint8_t Column, uint8_t N);

// OLEDÏÔÊ¾³õÊ¼»¯
void OLED_Show_Init(void);

#endif
