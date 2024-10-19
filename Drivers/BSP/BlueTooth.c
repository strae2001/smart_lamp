#include "BlueTooth.h"
#include "string.h"
#include <stdio.h>
#include "usart.h"

uint8_t Rx2Buf[Rx2BUF_SIZE];	// 串口2接收数据缓冲区

uint8_t command = 0;			// 接收蓝牙发来的指令

void BlueTooth_Init(void)
{
    MX_USART2_UART_Init();

    __HAL_UART_ENABLE_IT(&huart2, UART_IT_RXNE);     	 // 使能UART2 并开启UART_IT_RXNE触发中断
	__HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);	 	 // 使能UART2 空闲中断
}

void USART2_IRQHandler(void)
{
    static uint8_t cnt = 0;
    static uint8_t recv;
    if(__HAL_UART_GET_FLAG(&huart2, UART_FLAG_RXNE) != RESET)
    {
        HAL_UART_Receive(&huart2, &recv, 1, 10);
        if(cnt >= Rx2BUF_SIZE)
            cnt = 0;
        Rx2Buf[cnt++] = recv;
    }

    if(__HAL_UART_GET_FLAG(&huart2, UART_FLAG_IDLE) != RESET)
    {
        printf("blueTooth_recv: %s\r\n", Rx2Buf);
		command = Rx2Buf[0];						// 保存接收到的1位指令
		
		memset(Rx2Buf, 0, sizeof(Rx2BUF_SIZE));		// 清空数据缓冲区
		cnt = 0;
		
		__HAL_UART_CLEAR_IDLEFLAG(&huart2);     	// 清除串口空闲挂起标志位
    }
}


uint8_t bt_rx_getCmd(void)
{
	uint8_t temp = command;
	command = '9';
	return temp;
}


void serial2_sendByte(uint8_t Byte)
{
    HAL_UART_Transmit(&huart2, &Byte, 1, 10);
}

void Bluetooth_sendString(char* str)
{
    for (uint8_t i = 0; str[i]!='\0'; i++)
    {
        serial2_sendByte(str[i]);
    }
}
