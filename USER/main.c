#include "sys.h"
#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "NRF24L01.h"
#include "OLED.h"
#include "pwm.h"
#include "gpio.h"
#include "encoder.h"
#include "timer.h"
#include "usart.h"	
#include "pid.h"
#include "BME.h"
#include "task.h"

uint8_t RX_BUFF[32];
int Vx,Vy,Vz;
float x,y;
float xpwm,ypwm;
int sx,sy,sz;
uint16_t xp=160,yp=120;





uint8_t ID;
float p,t,w;
float pitch=0,roll=0,yaw=0;
int16_t speed1,speed2;
int temp,wet,pressure;
int32_t s1=20,s2=20,angle=200;
extern uint8_t RxData;
float now_roll;


int main(void)
{
	
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  delay_init(84);  //初始化延时函数
	GYRO_UART5_Init();
	OLED_Init();	
	OLED_Clear();
//	OLED_ShowNumber(2,1,1,1,10);
//	OLED_ShowString(0,0,"Pitch",10);
	Bmp_Init();
  TIM6_Int_Init(100-1,8400-1);
  Encoder_Init(TIM1);
	Encoder_Init(TIM3);  
  SG90_pwm_init();
	Serial_Init();
	MotorPWM_Timer2_Init();
  MotorTotalControl(0,0);
		while(1)
	{		
		
	//	TIM_SetCompare1(TIM5,2000);
		 //MotorTotalControl(0,20);
//		OLED_ShowNumber(0,0,speed1,3,10);		
//   OLED_ShowNumber(0,10,speed2,3,10);
//    loop(20,20);		
		p=BMP280_Get_Pressure();
		t=BMP280_Get_Temperature();
		w=p/t*0.01;
		
		temp=t;
		pressure=p;
		wet=w;

			if(RxData==0x01)//前进
			{
				while(RxData==0x01)
				{
//				s1=30;s2=30;
				run();
				show();
				}
			}
			
	
			if(RxData==0x02)//后退
			{
				while(RxData==0x02)
				{
				s1=-30;s2=-30;
				run();
				show();
				}
			}
			
			
			
      if(RxData==0x03)//舵机左转
			{
        RxData=0x01;
        angle+=10;
				if(angle>=250){angle=250;}
				

			}
			
			
      if(RxData==0x04)//舵机右转
			{
			
				RxData=0x01;
        angle-=10;
				if(angle<=150){angle=150;}

			}
			

			
			if(RxData==0x05)//电机加速
			{ 
				RxData=0x01;
				s1+=10;
				s2+=10;
			}
			
			
    if(RxData==0x06)//电机减速
			{
				RxData=0x01;
				s1-=10;
				s2-=10;
			}
			
			

      if(RxData==0x07)//信息回传
			{
				
				//run();
				show();
				send(); 
  			RxData=0x01;
				
			}
						

     if(RxData==0x08)//右翻转90闭环
			{
				now_roll=roll;
				while(RxData==0x08)
				{
        Turn_angle(90);
        TIM5 -> CCR2=200;
        show();					

				}
			}
			
	
     if(RxData==0x09)//左翻转90闭环
			{
				now_roll=roll;
				while(RxData==0x09)
				{
        Turn_angle(-90);
        TIM5 -> CCR2=200;
        show();	
				}
			}				
	}


}
