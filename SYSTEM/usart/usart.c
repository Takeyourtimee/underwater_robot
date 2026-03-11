#include "stm32f4xx.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>
#include "usart.h"

// 用于存储与陀螺仪相关的发送数据包
uint8_t GYRO_UART5_TxPacket[4];
// 用于存储与陀螺仪相关的接收数据包
uint8_t gyroRxBuffer[11];  // 接收缓冲区，根据协议长度定义
uint16_t GYRO_UART5_RxPacket[3];
// 与陀螺仪相关的接收标志
uint8_t GYRO_UART5_RxFlag;

// 接收状态变量
uint8_t gyroRxState = 0;    // 接收状态机
uint8_t gyroRxCount = 0;    // 接收计数
// 陀螺仪角度变量

// 初始化与陀螺仪相关的 UART5 串口
void GYRO_UART5_Init(void)
{
    // 使能 UART5 时钟，UART5 挂在 APB1 总线上
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
    // 使能 GPIOC 和 GPIOD 时钟 (UART5使用PC12作为TX，PD2作为RX)
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    
    // 配置 PC12 为复用推挽输出（TX）- UART5_TX
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_UART5);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    // 配置 PD2 为上拉输入（RX）- UART5_RX
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource2, GPIO_AF_UART5);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    // 初始化 UART5
    USART_Init(UART5, &USART_InitStructure);

    // 使能 UART5 的接收中断
    USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    NVIC_InitTypeDef NVIC_InitStructure;
    // 配置 UART5 的中断通道
    NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_Init(&NVIC_InitStructure);

    // 使能 UART5
    USART_Cmd(UART5, ENABLE);
}

// 发送与陀螺仪相关的单个字节
void GYRO_UART5_SendByte(uint8_t Byte)
{
    // 使用 UART5 发送字节
    USART_SendData(UART5, Byte);
    while (USART_GetFlagStatus(UART5, USART_FLAG_TXE) == RESET);
}

// 发送与陀螺仪相关的字节数组
void GYRO_UART5_SendArray(uint8_t *Array, uint16_t Length)
{
    uint16_t i;
    for (i = 0; i < Length; i++)
    {
        GYRO_UART5_SendByte(Array[i]);
    }
}

// 发送与陀螺仪相关的字符串
void GYRO_UART5_SendString(char *String)
{
    uint8_t i;
    for (i = 0; String[i] != '\0'; i++)
    {
        GYRO_UART5_SendByte(String[i]);
    }
}

// 计算幂次方
uint32_t GYRO_UART5_Pow(uint32_t X, uint32_t Y)
{
    uint32_t Result = 1;
    while (Y--)
    {
        Result *= X;
    }
    return Result;
}

// 发送与陀螺仪相关的数字
void GYRO_UART5_SendNumber(uint32_t Number, uint8_t Length)
{
    uint8_t i;
    for (i = 0; i < Length; i++)
    {
        GYRO_UART5_SendByte(Number / GYRO_UART5_Pow(10, Length - i - 1) % 10 + '0');
    }
}

// 重定向标准输出到与陀螺仪相关的 UART5
int fputc(int ch, FILE *f)
{
    GYRO_UART5_SendByte(ch);
    return ch;
}

// 与陀螺仪相关的格式化输出
void GYRO_UART5_Printf(char *format, ...)
{
    char String[100];
    va_list arg;
    va_start(arg, format);
    vsprintf(String, format, arg);
    va_end(arg);
    GYRO_UART5_SendString(String);
}

// 发送与陀螺仪相关的数据包
void GYRO_UART5_SendPacket(void)
{
    GYRO_UART5_SendByte(0xA5);
    GYRO_UART5_SendArray(GYRO_UART5_TxPacket, 4);
    GYRO_UART5_SendByte(0xA6);
}

// 获取与陀螺仪相关的接收标志
uint8_t GYRO_UART5_GetRxFlag(void)
{
    if (GYRO_UART5_RxFlag == 1)
    {
        GYRO_UART5_RxFlag = 0;
        return 1;
    }
    return 0;
}
extern float pitch;
extern float roll;
extern float yaw;
extern float p,t,wet;
// UART5 中断处理函数，陀螺仪数据接收
void UART5_IRQHandler(void)
{
    if (USART_GetITStatus(UART5, USART_IT_RXNE) == SET)
    {
        uint8_t rxData = USART_ReceiveData(UART5);

        switch (gyroRxState)
        {
            case 0:  // 等待第一个包头0x55
                if (rxData == 0x55) {
                    gyroRxBuffer[gyroRxCount++] = rxData;
                    gyroRxState = 1;
                }
                break;

            case 1:  // 等待第二个包头0x53
                if (rxData == 0x53) {
                    gyroRxBuffer[gyroRxCount++] = rxData;
                    gyroRxState = 2;
                } else {
                    // 包头错误，重置状态机
                    gyroRxCount = 0;
                    gyroRxState = 0;
                }
                break;

            case 2:  // 接收数据
                gyroRxBuffer[gyroRxCount++] = rxData;
                if (gyroRxCount >= 11) {
                    // 解析角度数据
                    int16_t rollRaw = (gyroRxBuffer[3] << 8) | gyroRxBuffer[2];
                    roll = (float)rollRaw / 32768.0f * 180.0f;

                    int16_t pitchRaw = (gyroRxBuffer[5] << 8) | gyroRxBuffer[4];
                    pitch = (float)pitchRaw / 32768.0f * 180.0f;

                    int16_t yawRaw = (gyroRxBuffer[7] << 8) | gyroRxBuffer[6];
                    yaw = (float)yawRaw / 32768.0f * 180.0f;

                    // 设置接收标志
                    GYRO_UART5_RxFlag = 1;
                    
                    // 重置状态机
                    gyroRxCount = 0;
                    gyroRxState = 0;
                }
                break;

            default:
                gyroRxCount = 0;
                gyroRxState = 0;
                break;
        }

        USART_ClearITPendingBit(UART5, USART_IT_RXNE);
    }
}
    