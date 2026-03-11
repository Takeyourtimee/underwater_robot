#ifndef __PID_H
#define __PID_H
#include "stm32f4xx.h"
int pid_control_a(int setpoint, int  process_variable);
int pid_control_b(int setpoint, int  process_variable);
int pid_control_c(int setpoint, int  process_variable);
int pid_control_d(int setpoint, int  process_variable);
void my_pid_control(int Vx,int Vy,int Vz);
#endif 
