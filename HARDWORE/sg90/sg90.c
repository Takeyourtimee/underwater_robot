#include "sg90.h"
#include "stm32f4xx.h"                  // Device header

// 橙色信号线　
// 红色正极　
// 棕褐色负极

// 具体的周期计算公式为：周期 = （arr + 1） * （psc + 1） / CLK。其中 CLK 为计数器的时钟频率，
// 我的是 168MHZ，也就是 168000000。最后计算结果单位为秒，结果为 0.02s，也就是 20ms。

// PWM 周期为 20ms = (8400 * 200) / 168000000 = 0.02
// 所以 TIM_Period = 199，TIM_Prescaler = 8399
// t = 0.5ms ——————- 舵机会转动 0 °
// t = 1.0ms ——————- 舵机会转动 45°
// t = 1.5ms ——————- 舵机会转动 90°
// t = 2.0ms ——————- 舵机会转动 135°
// t = 2.5ms ——————- 舵机会转动 180°
// PWM 占空比是指在一个周期内，信号处于高电平的时间占据整个信号周期的百分比，
// 由于 PWM 周期为 20ms，所以（以舵机会转动 45°为例），占空比就应该为 1ms / 20ms = 5%，
// 所以 TIM_SetCompare1 的 TIMx 捕获比较 1 寄存器值就为 200 - 200 * 5% = 190

void SG90_pwm_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;   // 声明一个结构体变量，用来初始化 GPIO
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure; // 声明一个结构体变量，用来初始化定时器
    TIM_OCInitTypeDef TIM_OCInitStructure; // 根据 TIM_OCInitStruct 中指定的参数初始化外设 TIMx

    /* 开启时钟 */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);

    /*  配置 GPIO 的模式和 IO 口 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1; // PA0, PA1
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; // 复用模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; // 推挽输出
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // 无上下拉
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 将 GPIO 连接到定时器
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM5);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM5);

    TIM_TimeBaseInitStructure.TIM_Period = 2000-1; // PWM 周期为 20ms
    TIM_TimeBaseInitStructure.TIM_Prescaler = 840-1;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = 0; // 设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; // TIM 向上计数模式
    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseInitStructure);

    // PWM 初始化 // 根据 TIM_OCInitStruct 中指定的参数初始化外设 TIMx
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; // PWM 输出使能
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

    TIM_OC1Init(TIM5, &TIM_OCInitStructure);
    TIM_OC2Init(TIM5, &TIM_OCInitStructure);
    // 注意此处初始化时 TIM_OC1Init 而不是 TIM_OCInit，否则会出错。因为固件库的版本不一样。
    TIM_OC1PreloadConfig(TIM5, TIM_OCPreload_Enable);
    TIM_OC2PreloadConfig(TIM5, TIM_OCPreload_Enable); // 使能或者失能 TIMx 在 CCR1 上的预装载寄存器
    TIM_Cmd(TIM5, ENABLE); // 使能或者失能 TIMx 外设
}    