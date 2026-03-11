#ifndef __PWM_H
#define __PWM_H	 
#include "sys.h" 
#include "stm32f4xx.h" 

// ------------------- 引脚定义 -------------------
// 电机1：BTN7971 控制
#define MOTOR1_PWM_PORT  GPIOA
#define MOTOR1_PWM_PIN   GPIO_Pin_15   // TIM2_CH1（PA15）
#define MOTOR1_DIR_PORT  GPIOB
#define MOTOR1_DIR_PIN   GPIO_Pin_3    // 方向控制：PB3

// 电机2：BTN7971 控制
#define MOTOR2_PWM_PORT  GPIOB
#define MOTOR2_PWM_PIN   GPIO_Pin_10   // TIM2_CH3（PB10）
#define MOTOR2_DIR_PORT  GPIOB
#define MOTOR2_DIR_PIN   GPIO_Pin_11   // 方向控制：PB11

// ------------------- PWM 参数定义 -------------------
#define PWM_PSC     8400-1      // 预分频值，84MHz / 84 = 1MHz 计数时钟
#define PWM_ARR     200-1     // 自动重装载值，1MHz / 1000 = 1kHz PWM 频率
#define PWM_MAX     (PWM_ARR + 1)      // 最大占空比对应值（1000）

// ------------------- 函数声明 -------------------

/**
 * @brief  初始化电机控制所需的定时器2和GPIO引脚
 * @note   配置TIM2_CH1(PA15)和TIM2_CH3(PB10)为PWM输出
 *         配置PB3和PB11为方向控制GPIO输出
 */
void MotorPWM_Timer2_Init(void);

/**
 * @brief  双电机总控函数，一条语句控制两个电机的转速和方向
 * @param  speed1: 电机1目标转速（-100 ~ +100）
 *         正值表示正转，负值表示反转，绝对值表示占空比百分比
 * @param  speed2: 电机2目标转速（-100 ~ +100）
 *         正值表示正转，负值表示反转，绝对值表示占空比百分比
 * @example MotorTotalControl(20, -10); // 电机1正转20%，电机2反转10%
 */
void MotorTotalControl(int speed1, int speed2);
#endif
