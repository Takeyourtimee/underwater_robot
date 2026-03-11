#include "stm32f4xx.h"                  // Device header
#include "delay.h"

// 引脚定义修改为PA11(SCL)和PA12(SDA)
void MyI2C_W_SCL(uint8_t BitValue)
{
    GPIO_WriteBit(GPIOA, GPIO_Pin_11, (BitAction)BitValue);
    delay_us(50);  // 保持延时确保时序稳定
}

void MyI2C_W_SDA(uint8_t BitValue)
{
    GPIO_WriteBit(GPIOA, GPIO_Pin_12, (BitAction)BitValue);
    delay_us(50);  // 保持延时确保时序稳定
}

uint8_t MyI2C_R_SDA(void)
{
    uint8_t BitValue;
    BitValue = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12);
    delay_us(50);  // 保持延时确保时序稳定
    return BitValue;
}

void MyI2C_Init(void)
{
    // 使能GPIOA时钟（F4系列时钟使能在AHB1总线上）
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_OType_OD;;       // 开漏输出，适合I2C
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;  // PA11(SCL)和PA12(SDA)
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;      // 50MHz速率
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;           // 上拉电阻，确保空闲时为高电平
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    // 初始化时将SCL和SDA置高，处于空闲状态
    GPIO_SetBits(GPIOA, GPIO_Pin_11 | GPIO_Pin_12);
}

// 以下I2C时序函数保持不变，时序逻辑与引脚无关
void MyI2C_Start(void)
{
    MyI2C_W_SDA(1);
    MyI2C_W_SCL(1);
    MyI2C_W_SDA(0);
    MyI2C_W_SCL(0);
}

void MyI2C_Stop(void)
{
    MyI2C_W_SDA(0);
    MyI2C_W_SCL(1);
    MyI2C_W_SDA(1);
}

void MyI2C_SendByte(uint8_t Byte)
{
    uint8_t i;
    for (i = 0; i < 8; i ++)
    {
        MyI2C_W_SDA(Byte & (0x80 >> i));
        MyI2C_W_SCL(1);
        MyI2C_W_SCL(0);
    }
}

uint8_t MyI2C_ReceiveByte(void)
{
    uint8_t i, Byte = 0x00;
    MyI2C_W_SDA(1);  // 释放SDA线，由从机控制
    for (i = 0; i < 8; i ++)
    {
        MyI2C_W_SCL(1);
        if (MyI2C_R_SDA() == 1)
        {
            Byte |= (0x80 >> i);
        }
        MyI2C_W_SCL(0);
    }
    return Byte;
}

void MyI2C_SendAck(uint8_t AckBit)
{
    MyI2C_W_SDA(AckBit);
    MyI2C_W_SCL(1);
    MyI2C_W_SCL(0);
}

uint8_t MyI2C_ReceiveAck(void)
{
    uint8_t AckBit;
    MyI2C_W_SDA(1);  // 释放SDA线
    MyI2C_W_SCL(1);
    AckBit = MyI2C_R_SDA();
    MyI2C_W_SCL(0);
    return AckBit;
}
    