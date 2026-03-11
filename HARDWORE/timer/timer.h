#ifndef _TIMER_H
#define _TIMER_H
#include "sys.h"
//定时中断：tim3 10ms
#define timer_arr 300//自动重装值
#define timer_psc 840-1//时钟预分频数
#define ABS(a) ((a)<0?((a)*(-1)):(a))
//定时器溢出时间:Tout=((arr+1)*(psc+1))/Ft.
//Ft:定时器工作频率84Mhz
void TIM6_Int_Init(u16 arr,u16 psc);
#endif


