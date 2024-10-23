/**
	************************************************************
	************************************************************
	************************************************************
	*	�ļ����� 	esp8266.c
	*	���ߣ� 		zh
	*	���ڣ� 		
	*	�汾�� 		V1.0
	*	˵���� 		ESP8266�ļ�����
	*	�޸ļ�¼��	
	************************************************************
	************************************************************
	************************************************************
**/

//��Ƭ��ͷ�ļ�
#include "cmsis_os.h"

//�����豸����
#include "esp8266.h"

//Ӳ������
//#include "delay.h"
#include "usart.h"

//C��
#include <string.h>
#include <stdio.h>

#define ESP8266_WIFI_INFO		"AT+CWJAP=\"Honor100\",\"Cc.246850@\"\r\n"

//#define ESP8266_ONENET_INFO		"AT+CIPSTART=\"TCP\",\"183.230.40.39\",6002\r\n"    //�ɰ�OneNET��ַ
#define ESP8266_ONENET_INFO		"AT+CIPSTART=\"TCP\",\"mqtts.heclouds.com\",1883\r\n"  	//�°�OneNET��ַ
//#define ESP8266_ONENET_INFO		"AT+CIPSTART=\"TCP\",\"192.168.11.125\",8080\r\n"   //���Ե�ַ

unsigned char esp8266_buf[128];
unsigned short esp8266_cnt = 0, esp8266_cntPre = 0;

//==========================================================
//	�������ƣ�	ESP8266_Clear
//
//	�������ܣ�	��ջ���
//
//	��ڲ�����	��
//
//	���ز�����	��
//
//	˵����		
//==========================================================
void ESP8266_Clear(void)
{

	memset(esp8266_buf, 0, sizeof(esp8266_buf));
	esp8266_cnt = 0;

}

//==========================================================
//	�������ƣ�	ESP8266_WaitRecive
//
//	�������ܣ�	�ȴ��������
//
//	��ڲ�����	��
//
//	���ز�����	REV_OK-�������		REV_WAIT-���ճ�ʱδ���
//
//	˵����		ѭ�����ü���Ƿ�������
//==========================================================
_Bool ESP8266_WaitRecive(void)
{
	if(esp8266_cnt == 0) 							//������ռ���Ϊ0 ��˵��û�д��ڽ��������У�����ֱ����������������
		return REV_WAIT;
		
	if(esp8266_cnt == esp8266_cntPre)				//�����һ�ε�ֵ�������ͬ����˵���������
	{
		esp8266_cnt = 0;							//��0���ռ���
			
		return REV_OK;								//���ؽ�����ɱ�־
	}
		
	esp8266_cntPre = esp8266_cnt;					//��Ϊ��ͬ
	
	return REV_WAIT;								//���ؽ���δ��ɱ�־

}

//==========================================================
//	�������ƣ�	ESP8266_SendCmd
//
//	�������ܣ�	��������
//
//	��ڲ�����	cmd������
//				res����Ҫ���ķ���ָ��
//
//	���ز�����	0-�ɹ�	1-ʧ��
//
//	˵����		
//==========================================================
_Bool ESP8266_SendCmd(char *cmd, char *res)
{
	
	unsigned char timeOut = 200;

	sendString_toEsp8266((unsigned char *)cmd, strlen((const char *)cmd));
	
	while(timeOut--)
	{
		if(ESP8266_WaitRecive() == REV_OK)							//����յ�����
		{
			if(strstr((const char *)esp8266_buf, res) != NULL)		//����������ؼ���
			{
				ESP8266_Clear();									//��ջ���
				
				return 0;
			}
		}
		HAL_Delay(10);
	}
	
	return 1;

}

//==========================================================
//	�������ƣ�	ESP8266_SendData
//
//	�������ܣ�	��������
//
//	��ڲ�����	data������
//				len������
//
//	���ز�����	��
//
//	˵����		
//==========================================================
void ESP8266_SendData(unsigned char *data, unsigned short len)
{

	char cmdBuf[200];
	
	ESP8266_Clear();								//��ս��ջ���
	sprintf(cmdBuf, "AT+CIPSEND=%d\r\n", len);		//��������
	if(!ESP8266_SendCmd(cmdBuf, ">"))				//�յ���>��ʱ���Է�������
	{
		sendString_toEsp8266(data, len);	//�����豸������������
	}

}

//==========================================================
//	�������ƣ�	ESP8266_GetIPD
//
//	�������ܣ�	��ȡƽ̨���ص�����
//
//	��ڲ�����	�ȴ���ʱ��(����10ms)
//
//	���ز�����	ƽ̨���ص�ԭʼ����
//
//	˵����		��ͬ�����豸���صĸ�ʽ��ͬ����Ҫȥ����
//				��ESP8266�ķ��ظ�ʽΪ	"+IPD,x:yyy"	x�������ݳ��ȣ�yyy����������
//==========================================================
unsigned char *ESP8266_GetIPD(unsigned short timeOut)
{

	char *ptrIPD = NULL;
	
	do
	{
		if(ESP8266_WaitRecive() == REV_OK)								//����������
		{
			ptrIPD = strstr((char *)esp8266_buf, "IPD,");				//������IPD��ͷ
			if(ptrIPD != NULL)										
			{
				ptrIPD = strchr(ptrIPD, ':');							//�ҵ�':'
				if(ptrIPD != NULL)
				{
					ptrIPD++;
					return (unsigned char *)(ptrIPD);
				}
				else
					return NULL;
				
			}
		}
		HAL_Delay(5);													//��ʱ�ȴ�
	} while(timeOut--);
	
	return NULL;														//��ʱ��δ�ҵ������ؿ�ָ��

}

//==========================================================
//	�������ƣ�	ESP8266_Init
//
//	�������ܣ�	��ʼ��ESP8266
//
//	��ڲ�����	��
//
//	���ز�����	��
//
//	˵����		
//==========================================================

void ESP8266_Init(void)
{
	MX_USART3_UART_Init();

    __HAL_UART_ENABLE_IT(&huart3, UART_IT_RXNE);    // ʹ��uart3 ��ʹ��UART_IT_RXNE���������ж�
	
	ESP8266_Clear();

	while(ESP8266_SendCmd("AT\r\n", "OK"))
		osDelay(500);
	
	while(ESP8266_SendCmd("AT+CWMODE=1\r\n", "OK"))
		osDelay(500);
	
	while(ESP8266_SendCmd(ESP8266_WIFI_INFO, "GOT IP"))
		osDelay(500);
	
	while(ESP8266_SendCmd(ESP8266_ONENET_INFO, "CONNECT"))
		osDelay(500);

}

//==========================================================
//	�������ƣ�	USART3_IRQHandler
//
//	�������ܣ�	����3�շ��ж�
//
//	��ڲ�����	��
//
//	���ز�����	��
//
//	˵����		
//==========================================================
void USART3_IRQHandler(void)
{
	static uint8_t recv = 0;
	if(__HAL_UART_GET_FLAG(&huart3,UART_FLAG_RXNE) != RESET) //�����ж�
	{
		if(esp8266_cnt >= sizeof(esp8266_buf))	
			esp8266_cnt = 0; 					//��ֹ���ڱ�ˢ��
		HAL_UART_Receive(&huart3, &recv, 1, 1000);
		esp8266_buf[esp8266_cnt++] = recv;
	}
}


/*	 �������ݷ�װ	*/
void serial3_sendByte(uint8_t Byte)
{
	HAL_UART_Transmit(&huart3, &Byte, 1, 100);
}

void sendString_toEsp8266(unsigned char* str, uint16_t len)
{
	ESP8266_Clear();
	for(uint8_t i=0; i<len; i++)
	{
		serial3_sendByte(str[i]);
	}
}
