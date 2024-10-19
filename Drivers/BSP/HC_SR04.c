#include "HC_SR04.h"
#include "tim.h"
#include "gpio.h"

double distance;

// ������ģ���ʼ��
void HCSR04_Init(void)
{
    MX_TIM1_Init();        // ��ʱ����ʼ��
    MX_GPIO_Init();		   // ��ӦGPIO��ʼ��
}

// ��ʼ���ͳ�����
void HC_start(void)
{
	Trige_LOW();        // Trige EchoĬ���õ͵�ƽ
    Echo_LOW(); 
	
    Trige_HIGH();
    HAL_Delay(18);       // ��Trige����15us�ߵ�ƽ ����trige��������
    Trige_LOW();  

	__HAL_TIM_SetCounter(&htim1, 0);        // ��0������
}

double get_distance(void)
{
    uint16_t total_time;
    HC_start();
    while (Echo_State() == GPIO_PIN_RESET);         
    timer1_start();               			// ��Echo�������ߵ�ƽʱ ������ʼ���� ��ʱ����ʼ��ʱ
	
    while (Echo_State() == GPIO_PIN_SET);
    timer1_stop();                			// ��Echo�������͵�ƽʱ ����������� ��ʱ��������ʱ

    //total_time = __HAL_TIM_GetCounter(&htim1);
    total_time = __HAL_TIM_GET_COUNTER(&htim1);
    distance = total_time * 1.0 / 2 * 0.0343;		// ��λcm 
	
    return distance;
}
