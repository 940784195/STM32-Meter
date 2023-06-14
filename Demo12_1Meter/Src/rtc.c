/**
  ******************************************************************************
  * File Name          : RTC.c
  * Description        : This file provides code for the configuration
  *                      of the RTC instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "rtc.h"
#include "usart.h"
#include "stm32f4xx_hal_uart.h"
/* USER CODE BEGIN 0 */
#include	"tftlcd.h"
#include	"usart.h"
#include	<stdio.h>	//用到函数sprintf()
#include	<string.h>	//用到函数strlen()

uint8_t RXNE_Times=0;
uint8_t ORE_Times=0;

/* USER CODE END 0 */

RTC_HandleTypeDef hrtc;

/* RTC init function */
void MX_RTC_Init(void)
{
  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */
    
  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date
  */
  sTime.Hours = 15;
  sTime.Minutes = 23;
  sTime.Seconds = 0;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
  sDate.WeekDay = RTC_WEEKDAY_WEDNESDAY;
  sDate.Month = RTC_MONTH_MAY;
  sDate.Date = 15;
  sDate.Year = 19;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
  /** Enable the WakeUp
  */
  if (HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, 0, RTC_WAKEUPCLOCK_CK_SPRE_16BITS) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_RTC_MspInit(RTC_HandleTypeDef* rtcHandle)
{

  if(rtcHandle->Instance==RTC)
  {
  /* USER CODE BEGIN RTC_MspInit 0 */

  /* USER CODE END RTC_MspInit 0 */
    /* RTC clock enable */
    __HAL_RCC_RTC_ENABLE();

    /* RTC interrupt Init */
    HAL_NVIC_SetPriority(RTC_WKUP_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(RTC_WKUP_IRQn);
  /* USER CODE BEGIN RTC_MspInit 1 */

  /* USER CODE END RTC_MspInit 1 */
  }
}

void HAL_RTC_MspDeInit(RTC_HandleTypeDef* rtcHandle)
{

  if(rtcHandle->Instance==RTC)
  {
  /* USER CODE BEGIN RTC_MspDeInit 0 */

  /* USER CODE END RTC_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_RTC_DISABLE();

    /* RTC interrupt Deinit */
    HAL_NVIC_DisableIRQ(RTC_WKUP_IRQn);
  /* USER CODE BEGIN RTC_MspDeInit 1 */

  /* USER CODE END RTC_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc)
{
	RTC_TimeTypeDef sTime;
	RTC_DateTypeDef sDate;

	if (HAL_RTC_GetTime(hrtc, &sTime,  RTC_FORMAT_BIN) == HAL_OK)
	{
		HAL_RTC_GetDate(hrtc, &sDate,  RTC_FORMAT_BIN);
		uint8_t		timeStr[20];	//时间字符�? hh:mm:ss
		sprintf(timeStr,"%2d:%2d:%2d\n",sTime.Hours,sTime.Minutes,sTime.Seconds);  //自动加�?�\0�?

		LCD_ShowStr(30,110, timeStr);  //在LCD上显示当前时�?
		if (isUploadTime)  //isUploadTime在文件usart.c中定�?
			HAL_UART_Transmit(&huart1,timeStr,strlen(timeStr),200);	//in  blocking mode
		//strlen()以结束符'\0'为标志计算字符串长度，但是不包含'\0',上位机只要换行符'\n'

		if(__HAL_UART_GET_FLAG(&huart1, UART_FLAG_ORE) != RESET)
		{
			ORE_Times++;
			LCD_ShowStr(10, 250, "ORE_Times");
			LCD_ShowUint(5+LCD_CurX, LCD_CurY, ORE_Times);

			HAL_UART_Receive_IT(&huart1, RxBuffer,RX_CMD_LEN);

			__HAL_UART_CLEAR_OREFLAG(&huart1); //清除溢出中断
/*
			if(__HAL_UART_GET_IT_SOURCE(&huart1, UART_IT_RXNE) != RESET)//RXNE =1表示非空，寄存器可被读取；读出后复位=0
			{

				HAL_UART_Receive_IT(&huart1, RxBuffer,RX_CMD_LEN);

				__HAL_UART_CLEAR_FLAG(&huart1,UART_FLAG_RXNE);

				if(__HAL_UART_GET_IT_SOURCE(&huart1, UART_IT_RXNE) != RESET)//再次判断有无复位
				{
					RXNE_Times++;
					LCD_ShowStr(10, 300, "RXNE RESET FAIL");
					LCD_ShowUint(5+LCD_CurX, LCD_CurY, RXNE_Times);
				}


			}
*/


		//rebuf[num++] = USART_ReceiveData(USART2); 读取串口数据
		}
	}

}

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
