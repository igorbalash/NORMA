#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"

/* Private defines -----------------------------------------------------------*/
#define MOTOR_UP_1_Pin GPIO_PIN_1
#define MOTOR_UP_1_GPIO_Port GPIOC
#define MOTOR_UP_2_Pin GPIO_PIN_2
#define MOTOR_UP_2_GPIO_Port GPIOC
#define MOTOR_DOWN_1_Pin GPIO_PIN_3
#define MOTOR_DOWN_1_GPIO_Port GPIOC
#define MAIN_VOLT_DETECT_Pin GPIO_PIN_0
#define MAIN_VOLT_DETECT_GPIO_Port GPIOA
#define REVERSE_VOLT_DETECT_Pin GPIO_PIN_3
#define REVERSE_VOLT_DETECT_GPIO_Port GPIOA
#define MOTOR_DOWN_2_Pin GPIO_PIN_4
#define MOTOR_DOWN_2_GPIO_Port GPIOC
#define MOTOR_SIDE1_1_Pin GPIO_PIN_5
#define MOTOR_SIDE1_1_GPIO_Port GPIOC
#define INPUT_SIGNAL_Pin GPIO_PIN_0
#define INPUT_SIGNAL_GPIO_Port GPIOB
#define INPUT_SIGNAL_EXTI_IRQn EXTI0_IRQn
#define CONFIG_1_Pin GPIO_PIN_1
#define CONFIG_1_GPIO_Port GPIOB
#define CONFIG_2_Pin GPIO_PIN_2
#define CONFIG_2_GPIO_Port GPIOB
#define MOTOR_COMM_POWER_Pin GPIO_PIN_10
#define MOTOR_COMM_POWER_GPIO_Port GPIOB
#define RED_LED_Pin GPIO_PIN_13
#define RED_LED_GPIO_Port GPIOB
#define GREEN_LED_Pin GPIO_PIN_14
#define GREEN_LED_GPIO_Port GPIOB
#define MOTOR_SIDE1_2_Pin GPIO_PIN_6
#define MOTOR_SIDE1_2_GPIO_Port GPIOC
#define USART1_TX_Pin GPIO_PIN_9
#define USART1_TX_GPIO_Port GPIOA
#define UP_BUTTON_Pin GPIO_PIN_3
#define UP_BUTTON_GPIO_Port GPIOB
#define UP_BUTTON_EXTI_IRQn EXTI3_IRQn
#define DOWN_BUTTON_Pin GPIO_PIN_4
#define DOWN_BUTTON_GPIO_Port GPIOB
#define DOWN_BUTTON_EXTI_IRQn EXTI4_IRQn
#define STOP_BUTTON_Pin GPIO_PIN_5
#define STOP_BUTTON_GPIO_Port GPIOB
#define STOP_BUTTON_EXTI_IRQn EXTI9_5_IRQn

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
