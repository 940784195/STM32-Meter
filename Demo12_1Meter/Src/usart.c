/**
  ******************************************************************************
  * File Name          : USART.c
  * Description        : This file provides code for the configuration
  *                      of the USART instances.
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
#include "usart.h"

/* USER CODE BEGIN 0 */
#include	"rtc.h"
#include	"tftlcd.h"
#include	<string.h>	//用到函数strlen()

uint8_t	rxCompleted=RESET;	//HAL_UART_Receive_IT()接收是否完成，作为接收完成的标志�??
uint8_t	isUploadTime=0;	//默认不上传时间数�?

//定义新的空白数组用于处理数据
uint8_t RxBuffer[35];//定义新的空白数组用于缓存数据
uint8_t	ProBuffer[35];


uint16_t Times=0;
uint16_t A_CPU=0;
uint16_t A_RAM=0;


/* USER CODE END 0 */

UART_HandleTypeDef huart1;

/* USART1 init function */

void MX_USART1_UART_Init(void)
{

  huart1.Instance = USART1;
  huart1.Init.BaudRate = 57600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart->Instance == USART1)
	{
		//接收到固定长度数据后使能UART_IT_IDLE中断，在UART_IT_IDLE中断里再次接�??
		rxCompleted=SET;	//接收完成

		for(uint16_t i=0;i<RX_CMD_LEN;i++)
			ProBuffer[i]=RxBuffer[i];

		__HAL_UART_ENABLE_IT(huart, UART_IT_IDLE); //接收到数据后才开�?? IDLE中断

	}
}

void	on_UART_IDLE(UART_HandleTypeDef *huart)		//在串口IDLE时处�??
{
	if (__HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE) == RESET)	//IDLE中断挂起标志位是否置位，UART_FLAG_IDLE是空闲标�??
		return;

	__HAL_UART_CLEAR_IDLEFLAG(huart); 	//清除IDLE挂起标志
	__HAL_UART_DISABLE_IT(huart, UART_IT_IDLE); 	//禁止IDLE事件中断
	if (rxCompleted)	//接收�??1条指�??
	{

		LCD_ShowStr(10, 170, (uint8_t *)"Enter on_UART_IDLE Times");
		Times++;//记录进入空闲中断的时间
		LCD_ShowUint(10, LCD_CurY+LCD_SP15,Times);

		LCD_ShowStr(10, LCD_CurY+LCD_SP15, (uint8_t *)ProBuffer);//将接收到的打印在LCD上

		rxCompleted=RESET;

		HAL_UART_Receive_IT(huart, RxBuffer, RX_CMD_LEN);	//再次启动串口接收

	}

}

uint16_t GetCPU(uint8_t val[6])
{
	uint16_t CPU=0;
	CPU = (val[0] - '0') * 100 + (val[1] - '0')*10+(val[2] - '0')*1;

	/*
	if (val[1] == ' ')  //判断“十”位是否为空
	  {
	    CPU = val[0] - '0';//转化字符串为数�?�，�? �? 位的字符串减去末尾的'0'后变成int型数�?
	  }
	  //当数据为十位数时
	  if ((val[1] != ' ') && (val[2] == ' '))//判断 “十”位是否 有效，同时�?�百位�?�是否为�?
	  {
	    CPU = (val[0] - '0') * 10 + (val[1] - '0');//转化字符串为数�?�，�? 百或�? 位的字符串减去末尾的'0'后变成int型数�?
	  }
	  //当数据为100�?
	  if ((val[1] == '0') && (val[2] =='0'))
	  {
	    CPU = 100;
	  }
*/
	  return CPU;
}

uint16_t GetRAM(uint8_t val[6])
{
	uint8_t RAM=0;
	RAM = (val[3] - '0') * 100 + (val[4] - '0')*10+(val[5] - '0')*1;
/*
	if (val[4] == ' ')
		  {
		    RAM = val[3] - '0';//转化字符串为数�??
		  }
		  //当数据为十位数时
		  if ((val[4] != ' ') && (val[5] == ' '))
		  {
		    RAM = (val[3] - '0') * 10 + (val[4] - '0');//转化字符串为数�??
		  }
		  //当数据为100�?
		  if ((val[4] == '0') && (val[5] =='0'))
		  {
		    RAM = 100;
		  }
*/
		  return RAM;
}

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
