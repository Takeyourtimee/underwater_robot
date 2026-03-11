#ifndef _USART_H
#define _USART_H

#include "stm32f4xx.h"

// 声明与陀螺仪相关的发送和接收数据包
extern uint8_t GYRO_UART5_TxPacket[4];
extern uint8_t gyroRxBuffer[11];
extern uint16_t GYRO_UART5_RxPacket[3];


// 声明与陀螺仪相关的接收标志
extern uint8_t GYRO_UART5_RxFlag;

// 声明初始化函数
void GYRO_UART5_Init(void);
// 声明发送单个字节函数
void GYRO_UART5_SendByte(uint8_t Byte);
// 声明发送字节数组函数
void GYRO_UART5_SendArray(uint8_t *Array, uint16_t Length);
// 声明发送字符串函数
void GYRO_UART5_SendString(char *String);
// 声明发送数字函数
void GYRO_UART5_SendNumber(uint32_t Number, uint8_t Length);
// 声明格式化输出函数
void GYRO_UART5_Printf(char *format, ...);
// 声明发送数据包函数
void GYRO_UART5_SendPacket(void);
// 声明获取接收标志函数
uint8_t GYRO_UART5_GetRxFlag(void);

#endif
    