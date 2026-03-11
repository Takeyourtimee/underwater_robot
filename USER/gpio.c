#include "gpio.h"

void mygpio_init(void)
{

  GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD ,ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE ,ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG ,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF ,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB ,ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Pin = GUN_Pin;
  GPIO_InitStructure.GPIO_OType=GPIO_OType_PP ;
  GPIO_InitStructure.GPIO_PuPd=  GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GUN_Port, &GPIO_InitStructure);
	
		
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_OType=GPIO_OType_PP ;
  GPIO_InitStructure.GPIO_PuPd=  GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_SetBits(GPIOE,GPIO_Pin_0);
	
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Pin = AIN1_Pin;
  GPIO_InitStructure.GPIO_OType=GPIO_OType_PP ;
  GPIO_InitStructure.GPIO_PuPd=  GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(AIN1_GPIO_Port, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Pin = AIN2_Pin;
  GPIO_InitStructure.GPIO_OType=GPIO_OType_PP ;
  GPIO_InitStructure.GPIO_PuPd=  GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(AIN2_GPIO_Port, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Pin = BIN1_Pin;
  GPIO_InitStructure.GPIO_OType=GPIO_OType_PP ;
  GPIO_InitStructure.GPIO_PuPd=  GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(BIN1_GPIO_Port, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Pin = BIN2_Pin;
  GPIO_InitStructure.GPIO_OType=GPIO_OType_PP ;
  GPIO_InitStructure.GPIO_PuPd=  GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(BIN2_GPIO_Port, &GPIO_InitStructure);
  

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Pin = CIN1_Pin;
  GPIO_InitStructure.GPIO_OType=GPIO_OType_PP ;
  GPIO_InitStructure.GPIO_PuPd=  GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(CIN1_GPIO_Port, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Pin = CIN2_Pin;
  GPIO_InitStructure.GPIO_OType=GPIO_OType_PP ;
  GPIO_InitStructure.GPIO_PuPd=  GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(CIN2_GPIO_Port, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Pin = DIN1_Pin;
  GPIO_InitStructure.GPIO_OType=GPIO_OType_PP ;
  GPIO_InitStructure.GPIO_PuPd=  GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DIN1_GPIO_Port, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Pin = DIN2_Pin;
  GPIO_InitStructure.GPIO_OType=GPIO_OType_PP ;
  GPIO_InitStructure.GPIO_PuPd=  GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DIN2_GPIO_Port, &GPIO_InitStructure);
	
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_OType=GPIO_OType_PP ;
  GPIO_InitStructure.GPIO_PuPd=  GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	
	
		
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;     // 设置为输入模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;         // 配置引脚2
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;  // 不使用上拉或下拉电阻
	GPIO_Init(GPIOB, &GPIO_InitStructure);            // 初始化GPIOG端口



	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_OType=GPIO_OType_PP ;
  GPIO_InitStructure.GPIO_PuPd=  GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	//GPIO_SetBits(GPIOE,GPIO_Pin_0);
}
