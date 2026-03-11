#include "stm32f4xx.h"                  // Device header
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


//信息上传函数
extern int temp,wet,pressure;
extern int16_t speed1,speed2;
extern int16_t s1,s2,angle;
void send()
{
		Serial_SendString("speed1:");
		Serial_SendNumber(speed1,3);
	  Serial_SendByte(0x0D);
  
	
	  Serial_SendString("speed2:");
		Serial_SendNumber(speed2,3);
	  Serial_SendByte(0x0D);
	
//		Serial_SendString("Yaw:");
//		Serial_SendNumber(yaw,3);
//	  Serial_SendByte(0x0D);
//	 
// 	  Serial_SendString("Pitch:");
//		Serial_SendNumber(pitch,3);
//	  Serial_SendByte(0x0D);
//	
//	
// 	  Serial_SendString("Roll:");
//		Serial_SendNumber(roll,3);
//	  Serial_SendByte(0x0D);
//	  
	 	Serial_SendString("Pressure:");
		Serial_SendNumber(pressure,8);
	  Serial_SendByte(0x0D);
	
	  Serial_SendString("Temppreture:");
		Serial_SendNumber(temp,3);
	  Serial_SendByte(0x0D);
		
	  Serial_SendString("Wet:");
		Serial_SendNumber(wet,2);
	  Serial_SendByte(0x0D);
	

}

extern uint8_t RxData;
extern float roll,yaw;
void show()
	{   
		OLED_ShowString(0,0,"Rx:",12);
	  OLED_ShowNumber(20,0,RxData,3,12);
		
		OLED_ShowString(0,2,"S1:",12);
		OLED_Num3(4,2, speed1);
		OLED_ShowString(53,2,"S2:",12);
		OLED_Num3(13,2, speed2);

		OLED_ShowString(0,4,"T1:",12);
		OLED_Num3(4,4, s1);
		OLED_ShowString(53,4,"T2:",12);
		OLED_Num3(13,4, s2);
		
		OLED_ShowString(0,6,"Roll:",12);
	  OLED_Float(40,6, roll,10);

	
	
}






void loop(int16_t pwm1,int16_t pwm2)
{
	 
	 int16_t motor1, motor2;
 	 motor1=Line_PWM1(speed1,pwm1);
 	 motor2=Line_PWM2(speed2,pwm2);
	 MotorTotalControl(motor1,motor2);

}



extern int16_t speed1,speed2;

//角度
#define rho_Kp_Angle 0.7
#define rho_Kd_Angle 0.01;

//速度
#define KP1 0.05
#define KD1 0.01

#define KP2 0.05
#define KD2 0.01


int Line_PWM1(int rho, int Taget_rho)
{
	static float rho_Kp = KP1, rho_Kd = KD1;
	static float PWM_out, error;
	static float last_error = 0;

	error = Taget_rho - rho;

	PWM_out += rho_Kp * error + rho_Kd * (error - last_error);

	last_error = error;
  if(PWM_out>200)PWM_out=200;
	if(PWM_out<-200)PWM_out=-200;
	return PWM_out;
}



int Line_PWM2(int rho, int Taget_rho)
{
	static float rho_Kp = KP2, rho_Kd = KD2;
	static float PWM_out, error;
	static float last_error = 0;

	error = Taget_rho - rho;

	PWM_out += rho_Kp * error + rho_Kd * (error - last_error);

	last_error = error;
  if(PWM_out>200)PWM_out=200;
	if(PWM_out<-200)PWM_out=-200;
	return PWM_out;
}



int Angle_PWM(int rho, int Taget_rho)
{
	static float rho_Kp = rho_Kp_Angle, rho_Kd = rho_Kd_Angle;
	static float PWM_out, error;
	static float last_error = 0;

	error = Taget_rho - rho;
 if(error<=0){error=-error;}
	PWM_out = rho_Kp * error + rho_Kd * (error - last_error);

	last_error = error;
	if(PWM_out>=200){PWM_out=200;}
	if(PWM_out<=-200){PWM_out=-200;}
	return PWM_out;
}


extern float now_roll;
void Turn_angle(int r_l)
{
	uint8_t anglepwm;
	anglepwm=Angle_PWM(roll,now_roll+r_l);
  if(roll<0){loop(0-anglepwm, 0+anglepwm);}
  if(roll>0){loop(0+anglepwm ,0-anglepwm );}
}
	


void run()
{
	loop(s1,s2);
	TIM_SetCompare2(TIM5,angle);
}
