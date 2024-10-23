#include "light_sensor.h"
#include "adc.h"

// ADת��ͨ������
void AD_channelConfig(ADC_HandleTypeDef* hadc, uint32_t channel, uint32_t rank, uint32_t stime)
{
	ADC_ChannelConfTypeDef sConfig;
	sConfig.Channel = channel;
	sConfig.Rank = rank;
	sConfig.SamplingTime = stime;		// ����ʱ��
	HAL_ADC_ConfigChannel(hadc, &sConfig);

}

uint32_t AD_getValue(uint32_t channel)
{
	AD_channelConfig(&hadc1, channel, ADC_REGULAR_RANK_1, ADC_SAMPLETIME_239CYCLES_5);
	
	HAL_ADC_Start(&hadc1);		// ��ʼת��
	HAL_ADC_PollForConversion(&hadc1, 10); 		// �ȴ�������ת����� ��ʱʱ��10	

	// ����AD1ת����������DR�Ĵ�����16λ  ��16λΪAD2ת�����
	return (uint16_t)HAL_ADC_GetValue(&hadc1);	
}

void light_Sensor_init(void)
{
	MX_ADC1_Init();
}

uint8_t light_Sensor_getValue(uint32_t channel)
{
	// ����ֵ�޶���0~100������Խǿ������ֵԽ��
	return (uint8_t)(100 - AD_getValue(channel)*100/4096);		
}
