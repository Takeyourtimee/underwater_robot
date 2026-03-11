#include "hc05.h"
#include "led.h"
#include "usart.h"
#include "move.h"

extern int tim3_cmd_flag;
void hc05_control(void)
{
	if(hc==1)
		{
			if(tim3_cmd_flag==1)
			{
				TIM_Cmd(TIM3,DISABLE);
				tim3_cmd_flag=0;
			}
		}
    
		else if(hc==0)
		{
		if(tim3_cmd_flag==0)
			{
				TIM_Cmd(TIM3,ENABLE);
				stop();
				tim3_cmd_flag=1;
			}
		}
		else if(hc==2)
		{
			if(tim3_cmd_flag==0)
			{printf("\r\ngo\r\n");
				qianjin();
			}
		}
		else if(hc==3)
		{
			if(tim3_cmd_flag==0)
			{printf("\r\nback\r\n");
				houtui();
			}
		}
		else if(hc==4)
		{
		if(tim3_cmd_flag==0)
			{printf("\r\nleft\r\n");
				zuozhuan();
			}
		}
		else if(hc==5)
		{
		if(tim3_cmd_flag==0)
			{printf("\r\nright\r\n");
			youzhuan();}
		}
		else if(hc==6)
		{
		if(tim3_cmd_flag==0)
			{stop();
			printf("\r\nstop\r\n");}
		}
		else if(hc==7)
		{
		if(tim3_cmd_flag==0)
			{led1();
			printf("\r\nled1\r\n");}
		}
		else if(hc==8)
		{
		if(tim3_cmd_flag==0)
			{led2();
			printf("\r\nled2\r\n");}
		}
		else if(hc==9)
		{
		if(tim3_cmd_flag==0)
			{led3();
			printf("\r\nled3\r\n");}
		}
}
