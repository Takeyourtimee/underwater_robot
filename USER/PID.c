#include "PID.h"
#include "pwm.h"
#include "usart.h"
float kp = 2.5;
float ki = 0.6;
float kd = 0.1;
extern volatile int32_t motor_rpm[4];  // 电机转速（RPM）
extern volatile int32_t output_rpm[4]; // 输出轴转速
extern int pid_output1;
extern int pid_output2;
extern int pid_output3;
extern int pid_output4;
extern int TargetA;
extern int TargetB;
extern int TargetC;
extern int TargetD;
#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))
int pid_control_a(int setpoint, int  process_variable) 
  {
    float kp = 2.5;
    float ki = 0.6;
    float kd = 0.1;
    // 误差相关变量
    static float error,output;
    static float last_error=0;
    static float integral,derivative;
    error = setpoint - process_variable;
    integral += error;
    derivative = error - last_error;
    output = kp * error + ki * integral + kd * derivative;
    last_error = error;
    if(output >=128){output =128;}
    if(output <=-128){output =-128;} 
    return output;
   }
int pid_control_b(int setpoint, int  process_variable) 
  {
    float kp = 2.5;
    float ki = 0.6;
    float kd = 0.1;
    // 误差相关变量
    static float error,output;
    static float last_error=0;
    static float integral,derivative;
    error = setpoint - process_variable;
    integral += error;
    derivative = error - last_error;
    output = kp * error + ki * integral + kd * derivative;
    last_error = error;
    if(output >=128){output =128;}
    if(output <=-128){output =-128;} 
    return output;
   }
  int pid_control_c(int setpoint, int  process_variable) 
  {
    float kp = 2.5;
    float ki = 0.6;
    float kd = 0.1;
    // 误差相关变量
    static float error,output;
    static float last_error=0;
    static float integral,derivative;
    error = setpoint - process_variable;
    integral += error;
    derivative = error - last_error;
    output = kp * error + ki * integral + kd * derivative;
    last_error = error;
    if(output >=128){output =128;}
    if(output <=-128){output =-128;} 
    return output;
   }
  int pid_control_d(int setpoint, int  process_variable) 
  {
    float kp = 2.45;
    float ki = 0.05;
    float kd = 0.02;
    // 误差相关变量
    static float error,output;
    static float last_error=0;
    static float integral,derivative;
    error = setpoint - process_variable;
    integral += error;
    derivative = error - last_error;
    output = kp * error + ki * integral + kd * derivative;
    last_error = error;
    if(output >=128){output =128;}
    if(output <=-128){output =-128;} 
    return output;
   }
  
   
 void my_pid_control(int Vx,int Vy,int Vz)
   {
  int Car_W=1;
  TargetA=Vx+Vy+Vz*(Car_W);
  TargetB=Vx-Vy-Vz*(Car_W);
  TargetC=Vx+Vy-Vz*(Car_W);
	TargetD=Vx-Vy+Vz*(Car_W); 
  TargetA = max(-128, min(128, TargetA));
  TargetB = max(-128, min(128, TargetB));
  TargetC = max(-128, min(128, TargetC));
  TargetD = max(-128, min(128, TargetD));
  int PWM_A=pid_control_a(TargetA,output_rpm[0]);
  int PWM_B=pid_control_b(TargetB,output_rpm[1]);
  int PWM_C=pid_control_c(TargetC,output_rpm[2]);
  int PWM_D=pid_control_d(TargetD,output_rpm[3]);   
   Motor_A(PWM_A );
   Motor_B(PWM_B ); 
   Motor_C(PWM_C );
   Motor_D(PWM_D );
    printf("RPM:%d\n", PWM_A);
    printf("TARGETA:%d\n", TargetA);
//    printf("RPM:%d\n", PWM_B);
//    printf("RPM:%d\n", PWM_C);
//    printf("RPM:%d\n", PWM_D);
   }