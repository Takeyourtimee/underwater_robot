#include "stm32f4xx.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>

uint8_t Serial_RxData;
uint8_t Serial_RxFlag;
uint8_t RxData;
	
void Serial_Init(void)
{
	// 使能UART4时钟（UART4挂载在APB1总线）
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
	// 使能GPIOC时钟（PC10/PC11属于GPIOC）
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	
	// 配置PC10为UART4_TX（复用推挽输出）
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;          // 复用功能模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;        // 推挽输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;          // 上拉
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	// 配置PC11为UART4_RX（复用输入）
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;          // 复用功能模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;        // 输入模式下无效，保持默认
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;          // 上拉输入
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	// 将PC10和PC11复用映射到UART4
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_UART4);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_UART4);
	
	// 配置UART4参数
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(UART4, &USART_InitStructure);
	
	// 使能UART4接收中断
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
	
	// 配置中断优先级
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;      // UART4中断通道
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	// 使能UART4
	USART_Cmd(UART4, ENABLE);
}

void Serial_SendByte(uint8_t Byte)
{
	USART_SendData(UART4, Byte);
	while (USART_GetFlagStatus(UART4, USART_FLAG_TXE) == RESET);
}

void Serial_SendArray(uint8_t *Array, uint16_t Length)
{
	uint16_t i;
	for (i = 0; i < Length; i ++)
	{
		Serial_SendByte(Array[i]);
	}
}

void Serial_SendString(char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i ++)
	{
		Serial_SendByte(String[i]);
	}
}

uint32_t Serial_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;
	while (Y --)
	{
		Result *= X;
	}
	return Result;
}

void Serial_SendNumber(uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i ++)
	{
		Serial_SendByte(Number / Serial_Pow(10, Length - i - 1) % 10 + '0');
	}
}


uint8_t Serial_GetRxFlag(void)
{
	if (Serial_RxFlag == 1)
	{
		Serial_RxFlag = 0;
		return 1;
	}
	return 0;
}

uint8_t Serial_GetRxData(void)
{
	return Serial_RxData;
}

// UART4中断服务函数
void UART4_IRQHandler(void)
{
	if (USART_GetITStatus(UART4, USART_IT_RXNE) == SET)
	{
		Serial_RxData = USART_ReceiveData(UART4);  // 读取接收数据到本地缓存
		RxData = Serial_RxData;                     // 同步到外部可访问变量
		Serial_RxFlag = 1;                          // 置位接收标志
		USART_ClearITPendingBit(UART4, USART_IT_RXNE);  // 清除中断标志
	}
}
    