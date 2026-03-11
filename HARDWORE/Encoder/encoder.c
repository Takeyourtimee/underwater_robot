#include "encoder.h"
#include "OLED.h"
#include "pwm.h"
#include "usart.h"
#include "math.h"
#include "PID.h"

float  sample_count=0;
// 定义定时器指针数组
TIM_TypeDef *TIM_Array[] = 
{
    (TIM_TypeDef *)TIM1_BASE,
    (TIM_TypeDef *)TIM3_BASE,

};
void ENC_GPIO_Init(TIM_TypeDef* TIMx)  //Moter_D
{
  if(TIMx==TIM1)
    {
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOA时钟
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//最高反应速度为100MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;		    //引脚复用模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;		  //开漏输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		    //上拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOA,GPIO_PinSource8,GPIO_AF_TIM1);//
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_TIM1);//
    }
    else if(TIMx==TIM3)
    {
  GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOA时钟
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//最高反应速度为100MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;		    //引脚复用模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;		  //开漏输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		    //上拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_TIM3);//
  GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_TIM3);//
    }
    

}

void Encoder_Init(TIM_TypeDef* TIMx) 
{ 
    // 1. 时钟使能（根据TIMx自动判断）
    if(TIMx == TIM1)  RCC_APB2PeriphClockCmd(RCC_APB2ENR_TIM1EN,ENABLE);
    else if(TIMx == TIM3) RCC_APB1PeriphClockCmd(RCC_APB1ENR_TIM3EN,ENABLE);
   
    // 2. GPIO配置（根据TIMx自动判断）
    ENC_GPIO_Init(TIMx);
    // 3. 定时器编码器模式配置
    TIM_EncoderInterfaceConfig(TIMx, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
    TIM_Cmd(TIMx, ENABLE);
}

int Read_Speed(int TIMx)
{
    int value_1;
    switch (TIMx)
    {
    case 1:
        value_1 = (short)TIM_GetCounter(TIM1);
        TIM_SetCounter(TIM1, 0);
        break; // IF是定时器2，1.采集编码器的计数值并保存。2.将定时器的计数值清零。
    case 2:
        value_1 = (short)TIM_GetCounter(TIM3);
        TIM_SetCounter(TIM3, 0);
        break;

    default:
        value_1 = 0;
    }
    return value_1;
}


extern float p,t,w;

extern int16_t speed1,speed2;
void TIM6_DAC_IRQHandler(void)
{  
	if(TIM_GetITStatus(TIM6,TIM_IT_Update)==SET)
		{ 

	  speed1 =-Read_Speed(1);
		speed2 = -Read_Speed(2);

			

			
			
		}

     TIM_ClearITPendingBit(TIM6,TIM_IT_Update);  //清除中断标志位
}
		
