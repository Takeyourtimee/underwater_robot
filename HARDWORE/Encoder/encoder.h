#ifndef __ENCODER_H
#define __ENCODER_H
#include "stm32f4xx.h"
void Get_Encoder(TIM_TypeDef* TIMx,int32_t motor_rpm,int32_t output_rpm);
void Encoder_Init(TIM_TypeDef* TIMx);
void TIM1_Encoder_Init(void);
void generate_sine_duty_cycle(int *duty_cycle, int num_samples) ;
int pid_control(int setpoint, int  process_variable);
#endif
