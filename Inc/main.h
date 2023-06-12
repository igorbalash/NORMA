/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define MOTOR_UP_1_Pin GPIO_PIN_1
#define MOTOR_UP_1_GPIO_Port GPIOC
#define MOTOR_UP_2_Pin GPIO_PIN_2
#define MOTOR_UP_2_GPIO_Port GPIOC
#define MOTOR_DOWN_1_Pin GPIO_PIN_3
#define MOTOR_DOWN_1_GPIO_Port GPIOC
#define MCU_UP_SENSE_Pin GPIO_PIN_1
#define MCU_UP_SENSE_GPIO_Port GPIOA
#define MCU_DOWN_SENSE_Pin GPIO_PIN_2
#define MCU_DOWN_SENSE_GPIO_Port GPIOA
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
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
