#include "key.h"
#include "gpio.h"

void key_init(void)
{
	
}

uint8_t key_check()
{
	static uint8_t key_up = 1;		// �����ɿ���ʶ
	uint8_t key_val = 0;			
	
	// ������һ������������
	if(key_up && (!KEY1 || !KEY2 || !KEY3 || !KEY4))
	{
		HAL_Delay(10);	// ����
		key_up = 0;
		if(!KEY1)		key_val = 1;
		else if(!KEY2)	key_val = 2;
		else if(!KEY3)	key_val = 3;
		else if(!KEY4)	key_val = 4;
	}
	else if(KEY1 && KEY2 && KEY3 && KEY4)
		key_up = 1;
	
	return key_val;
}
