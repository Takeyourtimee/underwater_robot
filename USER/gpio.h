#ifndef __GPIO_H
#define __GPIO_H
#include "stm32f4xx.h"
#define AIN1_Pin GPIO_Pin_4
#define AIN1_GPIO_Port GPIOD
#define AIN2_Pin GPIO_Pin_5
#define AIN2_GPIO_Port GPIOD
#define BIN1_Pin GPIO_Pin_6
#define BIN1_GPIO_Port GPIOD
#define BIN2_Pin GPIO_Pin_7
#define BIN2_GPIO_Port GPIOD
#define CIN1_Pin GPIO_Pin_12
#define CIN1_GPIO_Port GPIOE
#define CIN2_Pin GPIO_Pin_13
#define CIN2_GPIO_Port GPIOE
#define DIN1_Pin GPIO_Pin_14
#define DIN1_GPIO_Port GPIOE
#define DIN2_Pin GPIO_Pin_15
#define DIN2_GPIO_Port GPIOE

#define PWMC_Pin GPIO_Pin_10
#define PWMC_GPIO_Port GPIOB
#define PWMD_Pin GPIO_Pin_11
#define PWMD_GPIO_Port GPIOB
#define BEEP_Pin GPIO_Pin_2
#define BEEP_GPIO_Port GPIOG
#define PWMA_Pin GPIO_Pin_15
#define PWMA_GPIO_Port GPIOA

#define LED_G_Pin GPIO_Pin_11
#define LED_G_GPIO_Port GPIOG
#define PWMB_Pin GPIO_Pin_3
#define PWMB_GPIO_Port GPIOB


#define GUN_Pin  GPIO_Pin_2
#define GUN_Port GPIOF


void mygpio_init(void);
#endif
