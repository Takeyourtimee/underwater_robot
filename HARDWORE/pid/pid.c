#include "stm32f4xx.h"                  // Device header

 
#define CCR_UD           TIM5->CCR2    // up and down....    RANGE:1250-7500
#define CCR_LR           TIM5->CCR1    // Left and Right.... RANGE:1250-7500
#define Cen_x           160          // x轴中心坐标值
#define Cen_y           120          // y轴中心坐标值
 
#define KP1             0.25
#define KI1             0 
#define KD1             0.2

#define KP2             0.22
#define KI2             0  
#define KD2             0.2
     // 新增积分系数 KI2
 
//#define sp1             30
//#define sp2             23
//#define range           35
 
extern uint16_t Serial_RxPacket[2];    
extern  uint16_t xp,yp;


/*
 * @brief:        PID control function with Integral term
 * @param:        current:current value;target:target value
 * @retval:       None
 */
int PID1(int current, int target)   // PID速度控制
{
    static int LastError;        // Error[-1]
    static int IntegralError = 0; // 积分误差项
    int iError, Outpid;         // 当前误差
 
    iError = target - current;          // 增量计算
    IntegralError += iError;           // 累积误差
    Outpid = (KP1 * iError)            // E[k]项
           + (KD1 * (iError - LastError)) // E[k]-E[k-1]项
           + (KI1 * IntegralError);    // 积分项
    LastError = iError;               // 存储误差，用于下次计算
    return Outpid;
}
 
int PID2(int current, int target)   // PID速度控制
{
    static int LastError;        // Error[-1]
    static int IntegralError = 0; // 积分误差项
    int iError, Outpid;         // 当前误差
 
    iError = target - current;          // 增量计算
    IntegralError += iError;           // 累积误差
    Outpid = (KP2 * iError)            // E[k]项
           + (KD2 * (iError - LastError)) // E[k]-E[k-1]项
           + (KI2 * IntegralError);    // 积分项
    LastError = iError;               // 存储误差，用于下次计算
    return Outpid;
}


void Tilt(void)
{
    if (PID1(xp, Cen_x) + CCR_LR > 19000) CCR_LR = 19000;    // Right limit
    else if (CCR_LR < 18000) CCR_LR = 18000;           // Left limit
    else CCR_LR -= PID1(xp, Cen_x);   
 
    if (CCR_UD > 18700) CCR_UD = 18700;              // Up limit
    else if (CCR_UD < 18300) CCR_UD = 18300;         // Down limit
    else CCR_UD -= PID2(yp, Cen_y);
}