/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lamps.h"
#include "tim.h"
#include <stdio.h>
#include "OLED.h"
#include "HC_SR04.h"
#include "light_sensor.h"
#include "beep.h"
#include "infrared_sensor.h"
#include "key.h"
#include "BlueTooth.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

enum LAMP_MODE
{
	SMART_MODE = 0,
	BTN_MODE,
	REMOTE_MODE,
};

uint8_t person_flag 		= 0;			// 有无人标志， 			默认0 无人
//uint8_t tim_enable		= 0;			// 定时器能否开始计时		默认0 不计时
uint8_t measure_enable 	= 0;				// 测距使能标志位			默认0 不测距

uint8_t key_num = 0;						// 按键标识 				默认0 没有任何按键按下
uint8_t lamp_mode = SMART_MODE;				// 台灯模式默认为智能模式
	
/* USER CODE END Variables */
osThreadId Infrared_CheckHandle;
osThreadId keyScanHandle;
osThreadId SmartModeHandle;
osThreadId BtnModeHandle;
osThreadId RemoteModeHandle;
osThreadId TimCntHandle;
osThreadId OledShowHandle;
osThreadId MeasureHandle;
osMessageQId myQueueHandle;
osMutexId myMutexHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartInfrared_Check(void const * argument);
void StartkeyScan(void const * argument);
void StartSmartMode(void const * argument);
void StartBtnMode(void const * argument);
void StartRemoteMode(void const * argument);
void StartTimCnt(void const * argument);
void StartOledShow(void const * argument);
void StartMeasure(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
	
  /* USER CODE END Init */
  /* Create the mutex(es) */
  /* definition and creation of myMutex */
  osMutexDef(myMutex);
  myMutexHandle = osMutexCreate(osMutex(myMutex));

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* definition and creation of myQueue */
  osMessageQDef(myQueue, 16, uint16_t);
  myQueueHandle = osMessageCreate(osMessageQ(myQueue), NULL);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of Infrared_Check */
  osThreadDef(Infrared_Check, StartInfrared_Check, osPriorityHigh, 0, 128);
  Infrared_CheckHandle = osThreadCreate(osThread(Infrared_Check), NULL);

  /* definition and creation of keyScan */
  osThreadDef(keyScan, StartkeyScan, osPriorityAboveNormal, 0, 128);
  keyScanHandle = osThreadCreate(osThread(keyScan), NULL);

  /* definition and creation of SmartMode */
  osThreadDef(SmartMode, StartSmartMode, osPriorityNormal, 0, 128);
  SmartModeHandle = osThreadCreate(osThread(SmartMode), NULL);

  /* definition and creation of BtnMode */
  osThreadDef(BtnMode, StartBtnMode, osPriorityNormal, 0, 128);
  BtnModeHandle = osThreadCreate(osThread(BtnMode), NULL);

  /* definition and creation of RemoteMode */
  osThreadDef(RemoteMode, StartRemoteMode, osPriorityNormal, 0, 128);
  RemoteModeHandle = osThreadCreate(osThread(RemoteMode), NULL);

  /* definition and creation of TimCnt */
  osThreadDef(TimCnt, StartTimCnt, osPriorityBelowNormal, 0, 128);
  TimCntHandle = osThreadCreate(osThread(TimCnt), NULL);

  /* definition and creation of OledShow */
  osThreadDef(OledShow, StartOledShow, osPriorityLow, 0, 128);
  OledShowHandle = osThreadCreate(osThread(OledShow), NULL);

  /* definition and creation of Measure */
  osThreadDef(Measure, StartMeasure, osPriorityBelowNormal, 0, 128);
  MeasureHandle = osThreadCreate(osThread(Measure), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartInfrared_Check */
/**
  * @brief  Function implementing the Infrared_Check thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartInfrared_Check */
void StartInfrared_Check(void const * argument)
{
  /* USER CODE BEGIN StartInfrared_Check */
	uint8_t tim_enable = 1;						// 默认使能计时
  /* Infinite loop */
  for(;;)
  {
	person_flag = ia_flag_get();
	if(person_flag == TRUE)
	{
		if(tim_enable)
		{
			sit_time_set(0);				// 清0上次计时
			timer3_start();					// 开始计时
			tim_enable = 0;					// 失能计时  防止重复启动定时器
		}
		
		measure_enable = 1;					// 使能测距
	}
	else
	{
		measure_enable = 0;
		timer3_stop();
		tim_enable = 1;						// 使能计时 为下一次计时作准备
		beep_off();							// 关闭蜂鸣器
	}
	
    osDelay(10);
  }
  /* USER CODE END StartInfrared_Check */
}

/* USER CODE BEGIN Header_StartkeyScan */
/**
* @brief Function implementing the keyScan thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartkeyScan */
void StartkeyScan(void const * argument)
{
  /* USER CODE BEGIN StartkeyScan */
	
  /* Infinite loop */
  for(;;)
  {
	key_num = key_check();
	if(key_num == 1)					// 按下key1 模式切换
	{
		if(lamp_mode++ >= 2)
			lamp_mode = SMART_MODE;
	}
	else if(key_num == 3)
	{
		timer3_toggle();
	}
	else if(key_num == 4)
	{
		timer3_stop();
		sit_time_set(0);
	}
	
    osDelay(10);
  }
  /* USER CODE END StartkeyScan */
}

/* USER CODE BEGIN Header_StartSmartMode */
/**
* @brief Function implementing the SmartMode thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartSmartMode */
void StartSmartMode(void const * argument)
{
  /* USER CODE BEGIN StartSmartMode */
	uint8_t lignt_val = 0;				// 环境亮度
  /* Infinite loop */
  for(;;)
  {
	if(lamp_mode == SMART_MODE)
	{
		if(person_flag)
		{
			lignt_val = light_Sensor_getValue(ADC_CHANNEL_5);
			if(lignt_val < 30)
			{
				lamp_high();
			}
			else if(lignt_val >= 30 && lignt_val < 70)
			{
				lamp_Middle();
			}
			else if(lignt_val >= 70)
				lamp_low();
		}
		else
			lamp_off();
	}
    osDelay(10);
  }
  /* USER CODE END StartSmartMode */
}

/* USER CODE BEGIN Header_StartBtnMode */
/**
* @brief Function implementing the BtnMode thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartBtnMode */
void StartBtnMode(void const * argument)
{
  /* USER CODE BEGIN StartBtnMode */
	uint8_t lamp_brightness = 0;				// 台灯亮度	 默认0 台灯关闭状态
  /* Infinite loop */
  for(;;)
  {
	if(key_num == 2)
	{
		xSemaphoreTake(myMutexHandle, portMAX_DELAY);
		lamp_brightness = get_lamp_brightness();			// 获取台灯亮度
		xSemaphoreGive(myMutexHandle);
		
		if(lamp_brightness++ >= 3)
		{
			lamp_brightness = 0;
		}
		
		switch(lamp_brightness)			// 按键调整台灯亮度
		{
			case 0:
				lamp_off();
				break;
			case 1:
				lamp_low();
				break;
			case 2:
				lamp_Middle();
				break;
			case 3:
				lamp_high();
				break;
		}
	}
    osDelay(10);
  }
  /* USER CODE END StartBtnMode */
}

/* USER CODE BEGIN Header_StartRemoteMode */
/**
* @brief Function implementing the RemoteMode thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartRemoteMode */
void StartRemoteMode(void const * argument)
{
  /* USER CODE BEGIN StartRemoteMode */
	uint8_t lamp_brightness = 0;
  /* Infinite loop */
  for(;;)
  {
	 xSemaphoreTake(myMutexHandle, portMAX_DELAY);
	 lamp_brightness = get_lamp_brightness();			// 获取当前台灯亮度
	 xSemaphoreGive(myMutexHandle);
	  
	switch(lamp_brightness)			
	{
		case 0:	lamp_off();		break;
		case 1:	lamp_low();		break;
		case 2:	lamp_Middle();	break;
		case 3:	lamp_high();	break;
	}
	
	 if(lamp_mode == REMOTE_MODE)
	 {
		switch(bt_rx_getCmd() - '0')
		{
			case 0:
				lamp_off();
				break;
			case 1:
				lamp_low();
				break;
			case 2:
				lamp_Middle();
				break;
			case 3:
				lamp_high();
				break;
			case 4:
				timer3_toggle();
				break;
			case 5:
				timer3_stop();
				sit_time_set(0);
				break;
		}
	 }

    osDelay(10);
  }
  /* USER CODE END StartRemoteMode */
}

/* USER CODE BEGIN Header_StartTimCnt */
/**
* @brief Function implementing the TimCnt thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTimCnt */
void StartTimCnt(void const * argument)
{
  /* USER CODE BEGIN StartTimCnt */
	uint8_t the_time = 0;
  /* Infinite loop */
  for(;;)
  {
	  the_time = sit_time_get();
	  if(the_time / 10  >= 1 && the_time % 10 == 0)			// 久坐提示(10s钟提示一次)
	  {
		  beep_on();
	  }
	
    osDelay(10);
  }
  /* USER CODE END StartTimCnt */
}

/* USER CODE BEGIN Header_StartOledShow */
/**
* @brief Function implementing the OledShow thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartOledShow */
void StartOledShow(void const * argument)
{
  /* USER CODE BEGIN StartOledShow */
	uint8_t lamp_brightness;	
	uint8_t readDis = 0;						// 定义变量接收从队列中读取的距离值
  /* Infinite loop */
  for(;;)
  {
	/*	 模式显示	*/
	switch(lamp_mode)
	{
		case SMART_MODE:
			OLED_ShowChineseWord(1, 10, 2);		// 智
			OLED_ShowChineseWord(1, 12, 3);		// 能
			break;
		
		case BTN_MODE:
			OLED_ShowChineseWord(1, 10, 4);		// 按
			OLED_ShowChineseWord(1, 12, 5);		// 键
			break;
		
		case REMOTE_MODE:
			OLED_ShowChineseWord(1, 10, 6);		// 远
			OLED_ShowChineseWord(1, 12, 7);		// 程
			break;
	}	
	
	/*	 亮度显示	*/
	lamp_brightness = get_lamp_brightness();			// 获取台灯亮度
	switch(lamp_brightness)
	{
		case 0:
			OLED_ShowChineseWord(2, 11, 16);
			break;
		case 1:
			OLED_ShowChineseWord(2, 11, 13);
			break;
		case 2:
			OLED_ShowChineseWord(2, 11, 14);
			break;
		case 3:
			OLED_ShowChineseWord(2, 11, 15);
			break;
	}
	
	/*	 有/无人、距离显示	*/
	if(person_flag)
	{
		OLED_ShowChineseWord(3, 2, 8);			// 有
	}
	else
		OLED_ShowChineseWord(3, 2, 9);			// 无
		
	if(xQueueReceive(myQueueHandle, &readDis, 20) == pdTRUE)
	{
		//printf("从队列读取距离数据成功, readDis: %d\r\n", readDis);
	}
	
	if(person_flag == TRUE)
	{
		OLED_ShowNum(3, 11, readDis, 2);
		OLED_ShowString(3,14, "cm");
		OLED_ShowNum(4, 11, sit_time_get(), 2);
		OLED_ShowChar(4, 14, 's');
	}
	else
	{
		OLED_ShowString(3, 11, "      ");
		//OLED_ShowString(4, 11, "      ");
	}
	
    osDelay(10);
  }
  /* USER CODE END StartOledShow */
}

/* USER CODE BEGIN Header_StartMeasure */
/**
* @brief Function implementing the Measure thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartMeasure */
void StartMeasure(void const * argument)
{
  /* USER CODE BEGIN StartMeasure */
	uint8_t distance = 0;				// 测出的距离
  /* Infinite loop */
  for(;;)
  {
	if(measure_enable)
	{
		distance = get_distance();
		
		if(xQueueSend(myQueueHandle, &distance, 20) == pdTRUE)
		{
			//printf("距离数据写入队列成功, distance: %d\r\n", distance);
		}
		
		if(distance < 10)			// 距离台灯太近 蜂鸣器安全提示
		{
			beep_on();
		}
		else
			beep_off();
	}
	else
	{	
		distance = 0;
		if(xQueueSend(myQueueHandle, &distance, 20) == pdTRUE)
		{
			//printf("距离清0\r\n");
		}
	}
	
    osDelay(10);
  }
  /* USER CODE END StartMeasure */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

