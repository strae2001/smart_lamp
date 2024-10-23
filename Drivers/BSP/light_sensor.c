#include "light_sensor.h"
#include "adc.h"

// AD转换通道配置
void AD_channelConfig(ADC_HandleTypeDef* hadc, uint32_t channel, uint32_t rank, uint32_t stime)
{
	ADC_ChannelConfTypeDef sConfig;
	sConfig.Channel = channel;
	sConfig.Rank = rank;
	sConfig.SamplingTime = stime;		// 采样时间
	HAL_ADC_ConfigChannel(hadc, &sConfig);

}

uint32_t AD_getValue(uint32_t channel)
{
	AD_channelConfig(&hadc1, channel, ADC_REGULAR_RANK_1, ADC_SAMPLETIME_239CYCLES_5);
	
	HAL_ADC_Start(&hadc1);		// 开始转换
	HAL_ADC_PollForConversion(&hadc1, 10); 		// 等待规则组转换完成 超时时间10	

	// 返回AD1转换结果存放在DR寄存器低16位  高16位为AD2转换结果
	return (uint16_t)HAL_ADC_GetValue(&hadc1);	
}

void light_Sensor_init(void)
{
	MX_ADC1_Init();
}

uint8_t light_Sensor_getValue(uint32_t channel)
{
	// 返回值限定在0~100，光照越强，返回值越大
	return (uint8_t)(100 - AD_getValue(channel)*100/4096);		
}
