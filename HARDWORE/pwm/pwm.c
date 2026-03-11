#include "stm32f4xx.h"
#include "math.h"

// ------------------- 引脚与定时器参数定义 -------------------
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

// PWM 时基参数（频率 = 84MHz / (PSC+1) / (ARR+1) ）
#define PWM_PSC     840-1     // 84MHz / 84 = 1MHz 计数时钟
#define PWM_ARR     2000-1     // 1MHz / 1000 = 1kHz PWM 频率
#define PWM_MAX     (PWM_ARR + 1)      // 占空比分母（1000）

// ------------------- 定时器与引脚初始化 -------------------
void MotorPWM_Timer2_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
    TIM_OCInitTypeDef TIM_OCInitStruct;

    // 使能时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);        // TIM2 时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB, ENABLE); // GPIO 时钟

    // ------------ 引脚复用配置（TIM2_CH1/CH3）------------
    // 电机1 PWM：PA15 - TIM2_CH1
    GPIO_PinAFConfig(MOTOR1_PWM_PORT, GPIO_PinSource15, GPIO_AF_TIM2);
    // 电机2 PWM：PB10 - TIM2_CH3
    GPIO_PinAFConfig(MOTOR2_PWM_PORT, GPIO_PinSource10, GPIO_AF_TIM2);

    // 电机1 PWM引脚（PA15）：复用为TIM2_CH1
    GPIO_InitStruct.GPIO_Pin = MOTOR1_PWM_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(MOTOR1_PWM_PORT, &GPIO_InitStruct);

    // 电机2 PWM引脚（PB10）：复用为TIM2_CH3
    GPIO_InitStruct.GPIO_Pin = MOTOR2_PWM_PIN;
    GPIO_Init(MOTOR2_PWM_PORT, &GPIO_InitStruct);

    // 电机1方向引脚（PB3）：普通GPIO输出
    GPIO_InitStruct.GPIO_Pin = MOTOR1_DIR_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_Init(MOTOR1_DIR_PORT, &GPIO_InitStruct);

    // 电机2方向引脚（PB11）：普通GPIO输出
    GPIO_InitStruct.GPIO_Pin = MOTOR2_DIR_PIN;
    GPIO_Init(MOTOR2_DIR_PORT, &GPIO_InitStruct);

    // ---------------- 定时器时基配置 ----------------
    TIM_TimeBaseStruct.TIM_Prescaler = PWM_PSC;        // 84MHz / 84 = 1MHz 计数时钟
    TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStruct.TIM_Period = PWM_ARR;           // 1kHz PWM 频率（1MHz / 1000 = 1kHz）
    TIM_TimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStruct);

    // ---------------- PWM 模式配置 ----------------
    TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;

    // 电机1 PWM通道：TIM2_CH1
    TIM_OC1Init(TIM2, &TIM_OCInitStruct);
    TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);

    // 电机2 PWM通道：TIM2_CH3
    TIM_OC3Init(TIM2, &TIM_OCInitStruct);
    TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);

    // 使能定时器预装载和计数器
    TIM_ARRPreloadConfig(TIM2, ENABLE);
    TIM_Cmd(TIM2, ENABLE);
}

// ------------------- 单电机控制函数（内部使用） -------------------
static void MotorControl(int speed, TIM_TypeDef* tim, uint8_t channel,
                         GPIO_TypeDef* dir_port, uint16_t dir_pin) {
    uint32_t pwm_value = abs(speed);
    pwm_value = (pwm_value > 100) ? 100 : pwm_value; // 限幅到 ±100%
    pwm_value = (pwm_value * PWM_MAX) / 100;         // 映射到 0~PWM_MAX（0~1000）

    if (speed > 0) {
        // 正转：方向引脚输出低电平（SAIN2 接地）
        GPIO_ResetBits(dir_port, dir_pin);
    } else if (speed < 0) {
        // 反转：方向引脚输出高电平（SAIN2 接高）
        GPIO_SetBits(dir_port, dir_pin);
        pwm_value = PWM_MAX - pwm_value; // 互补占空比
    } else {
        // 停止：占空比 0，方向引脚低电平
        pwm_value = 0;
        GPIO_ResetBits(dir_port, dir_pin);
    }

    // 设置 PWM 占空比
    switch (channel) {
        case 1: TIM_SetCompare1(tim, pwm_value); break; // 电机1：TIM2_CH1
        case 3: TIM_SetCompare3(tim, pwm_value); break; // 电机2：TIM2_CH3
        default: break;
    }
}

// ------------------- 双电机总控函数（对外接口） -------------------
/**
 * @brief  双电机总控：一条语句控制两个电机
 * @param  speed1: 电机1目标转速（-100 ~ +100，对应占空比 -100% ~ +100%）
 * @param  speed2: 电机2目标转速（-100 ~ +100，对应占空比 -100% ~ +100%）
 * @note   示例：MotorTotalControl(20, -10) → 电机1正转20%，电机2反转10%
 */
void MotorTotalControl(int speed1, int speed2) {
    // 电机1控制：PA15（TIM2_CH1） + PB3（方向）
    MotorControl(speed1, TIM2, 1, MOTOR1_DIR_PORT, MOTOR1_DIR_PIN);
    // 电机2控制：PB10（TIM2_CH3） + PB11（方向）
    MotorControl(speed2, TIM2, 3, MOTOR2_DIR_PORT, MOTOR2_DIR_PIN);
}