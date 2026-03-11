#include "move.h"
#include "MS5837.h"
#include "led.h"

extern float angle_x,angle_y,angle_z;

extern float real_depth,target_depth;

void pipe_control(void)
{
	static float off_err2=0;//
	static float off_err1=0;//
	static float off_err3=0;//
  
	static float ang_err2=0;//
	static float ang_err1=0;//
	static float ang_err3=0;//
	
	static float off_p=0;
	static float off_i=0;
	static float off_d=0;
  
	static float ang_p=0;
	static float ang_i=0;
	static float ang_d=0;
	
	static float sumoff_I=0;
	static float sumang_I=0;
	
	static float off_r1;
	static float ang_r2;
	
	static float speed_front_1=90;
	static float speed_front_3=20;    //0
	static float a;
	
	a=real_angle;
	real_angle=real_offset;
	real_offset=a;//real_angle和real_offset交换
	
	off_err3=off_err2;
	off_err2=off_err1;//
	off_err1=real_angle;//
	
	ang_err3=ang_err2;
	ang_err2=ang_err1;
	ang_err1=real_offset;
	
  
	sumoff_I=off_err1+off_err2+off_err3;//
	sumang_I=ang_err1+ang_err2+ang_err3;//

	
	off_p=5;
	off_i=0;
	off_d=3;//0.2
	
  
	if(real_angle>15||real_angle<-15)
		{
		ang_p = 4;//2.5
		ang_i = 0;
		ang_d =3;//0.5
//		speed_front_1=180;
		}
	else
		{
		ang_p = 2.7;//1.5
		ang_i = 0;
		ang_d = 5;//0.2
//		speed_front_1=180;
		}

    
    
	off_r1=off_p*off_err1-off_i*sumoff_I+off_d*(off_err2-off_err1);
	ang_r2=ang_p*ang_err1-ang_i*sumang_I+ang_d*(ang_err2-ang_err1);//位置
	
    
	off_r1 = off_r1<100?off_r1:100;
	off_r1 = off_r1>-100?off_r1:-100; //阈值
    
	ang_r2 = ang_r2<200?ang_r2:200;
    
	ang_r2 = ang_r2>-200?ang_r2:-200; //阈值:0)));//0.2,1
  /*****对PID计算结果限幅***/  
  r3_motor((int)(speed_front_3-ang_r2));//差速控制
	l3_motor((int)(speed_front_3+ang_r2));	
    
	l1_motor((int)(speed_front_1+(int)(off_r1>0?((int)(off_r1)):0)+0.2*ang_r2));//0.2,1
  r1_motor((int)(speed_front_1+(int)(off_r1<0?((int)(-off_r1)):0)+0.2*ang_r2));//差速控制
    
//	l1_motor((int)(speed_front));//0.2,1
//	r1_motor((int)(speed_front));//0.2,1

}


void balance(void)//陀螺仪 深度
{
	static float p=6;//6
	static float d=10;//5
	
	static float ang_err1=0;
	static float ang_err2=0;
	static float angr1=0;
	//陀螺仪
	static float dep_p=19;
	static float dep_d=12;
	static float depr1=0;
	
	static float dep_err1=0;
	static float dep_err2=0;
	static float dep_err3=0;
	
  dep_err3=dep_err2;
	dep_err2=dep_err1;
	dep_err1=real_depth-target_depth;
	
	ang_err2=ang_err1;
	ang_err1=angle_y;
	
	angr1=p*ang_err1+d*(ang_err1-ang_err2);
	
	depr1=depr1+dep_p*(dep_err1-dep_err2)+dep_d*(dep_err1+dep_err3-2*dep_err2);
	angr1 = angr1<200?angr1:200;
	angr1 = angr1>-200?angr1:-200; //阈值:0)));//0.2,1
	depr1 = depr1<200?depr1:200;
	depr1 = depr1>-200?depr1:-200;
//	l2_motor(-(int)(angr1)+(int)(depr1));
//	r2_motor((int)(angr1)+(int)(depr1));
l2_motor((int)(depr1)-(int)(angr1));
r2_motor((int)(depr1)+(int)(angr1));
}


void steer_gear(int angle)
{
	static int angle_nano;
	static int pwm;
	angle_nano=angle;
	pwm=(int)(angle_nano/90.0*1000+1500);//500：-90度   1500：0度   2500：90度
	TIM_SetCompare4(TIM4,pwm-1);
}




//蓝牙控制运动
void qianjin(void)
{
	r1_motor(200);
	r3_motor(200);
	l1_motor(200);
	l3_motor(200);
}


void houtui(void)
{
	r1_motor(-200);
	r3_motor(-200);
	l1_motor(-200);
	l3_motor(-200);
}

void zuozhuan(void)
{
	r1_motor(100);
	r3_motor(100);
	l1_motor(-100);
	l3_motor(-100);
}

void youzhuan(void)
{
	r1_motor(-100);
	r3_motor(-100);
	l1_motor(100);
	l3_motor(100);
}

void shangsheng(void)
{
	r2_motor(100);
	l2_motor(100);
}


void xiajiang(void)
{
	r2_motor(-100);
	l2_motor(-100);

}

void stop(void)
{
	r1_motor(0);
	l1_motor(0);
	r2_motor(0);
	l2_motor(0);
	r3_motor(0);
	l3_motor(0);
}

void led1(void)
{
	GPIO_SetBits(GPIOE,GPIO_Pin_0);
	delay_ms(500);
	GPIO_ResetBits(GPIOE,GPIO_Pin_0);
}
void led2(void)
{
	GPIO_SetBits(GPIOE,GPIO_Pin_2);
	delay_ms(500);
	GPIO_ResetBits(GPIOE,GPIO_Pin_2);
}
void led3(void)
{
	GPIO_SetBits(GPIOE,GPIO_Pin_3);
	delay_ms(500);
	GPIO_ResetBits(GPIOE,GPIO_Pin_3);
}
