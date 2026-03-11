#ifndef _MOVE_H
#define _MOVE_H
#include "sys.h"



#define r1_motor(m)        TIM_SetCompare2(TIM5,1500-m-1)
#define l3_motor(m)        	TIM_SetCompare1(TIM4,1500-m-1)//middle left 2
#define r3_motor(m)  	  TIM_SetCompare4(TIM5,1500-m-1)//middle right 2 
#define r2_motor(m)       	TIM_SetCompare2(TIM4,1500+m-1)	 
#define l1_motor(m)       TIM_SetCompare3(TIM4,1500-m-1)
#define l2_motor(m)       TIM_SetCompare3(TIM5,1500+m-1) 
 

void pipe_control(void);
void control(void);
void balance(void);

void steer_gear(int angle);

void qianjin(void);
void houtui(void);
void zuozhuan(void);
void youzhuan(void);
void shangsheng(void);
void xiajiang(void);
void stop(void);
void led1(void);
void led2(void);
void led3(void);
#endif


