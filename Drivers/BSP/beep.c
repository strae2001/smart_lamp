#include "beep.h"
#include "gpio.h"

//��ʼ��GPIO����
void beep_init(void)
{
    //�رշ�����
    beep_off();
}

//�򿪷������ĺ���
void beep_on(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);   // ���ͷ��������ţ��򿪷�����
}

//�رշ������ĺ���
void beep_off(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);     // ���߷��������ţ��رշ�����
}
