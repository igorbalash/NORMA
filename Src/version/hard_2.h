/************************************************************************************
                  ** H **
Module: Модуль предназначен для задания дефайнов при работе с данным исполнением схемы и платы.
Author: Unic-Lab <https://unic-lab.ru/>
************************************************************************************/

#ifndef _HARD_2_H_
#define _HARD_2_H_

//===================================================================================

#define PRODUCT_HARD	2

//===================================================================================

// defines для модуля leds.h
#define LEDS_GPIO_CLK_ENABLE()						{ __HAL_RCC_GPIOB_CLK_ENABLE(); }

#define LED_ON										GPIO_PIN_SET
#define LED_OFF										GPIO_PIN_RESET

#define LED_RED_PORT								GPIOB
#define LED_RED_PIN									GPIO_PIN_13

#define LED_GREEN_PORT								GPIOB
#define LED_GREEN_PIN								GPIO_PIN_14

//===================================================================================

// defines для модуля buttons.h
#define BUTTONS_GPIO_CLK_ENABLE()					{ __HAL_RCC_GPIOB_CLK_ENABLE(); }

#define UP_BUTTON_PORT								GPIOB
#define UP_BUTTON_PIN								GPIO_PIN_3
#define UP_BUTTON_PULL								GPIO_NOPULL
#define UP_BUTTON_MODE								GPIO_MODE_IT_FALLING
#define UP_BUTTON_EXTI_IRQN							EXTI3_IRQn

#define DOWN_BUTTON_PORT							GPIOB
#define DOWN_BUTTON_PIN								GPIO_PIN_4
#define DOWN_BUTTON_PULL							GPIO_NOPULL
#define DOWN_BUTTON_MODE							GPIO_MODE_IT_FALLING
#define DOWN_BUTTON_EXTI_IRQN						EXTI4_IRQn

#define STOP_BUTTON_PORT							GPIOB
#define STOP_BUTTON_PIN								GPIO_PIN_5
#define STOP_BUTTON_PULL							GPIO_NOPULL
#define STOP_BUTTON_MODE							GPIO_MODE_IT_FALLING
#define STOP_BUTTON_EXTI_IRQN						EXTI9_5_IRQn

#define EXTI3_IRQ_HANDLER_PROCESSING()				{ HAL_GPIO_EXTI_IRQHandler(UP_BUTTON_PIN); }
#define EXTI4_IRQ_HANDLER_PROCESSING()				{ HAL_GPIO_EXTI_IRQHandler(DOWN_BUTTON_PIN); }
#define EXTI5_IRQ_HANDLER_PROCESSING()				{ HAL_GPIO_EXTI_IRQHandler(STOP_BUTTON_PIN); }

//===================================================================================

// defines для модуля input_signal.h
#define INPUT_SIGNAL_GPIO_CLK_ENABLE()				{ __HAL_RCC_GPIOC_CLK_ENABLE(); }

#define INPUT_SIGNAL_ACTIVE_STATE					GPIO_PIN_RESET
#define INPUT_SIGNAL_PORT							GPIOC
#define INPUT_SIGNAL_PIN							GPIO_PIN_2
#define INPUT_SIGNAL_PIN_INDEX						2
#define INPUT_SIGNAL_PULL							GPIO_NOPULL
#define INPUT_SIGNAL_MODE							GPIO_MODE_INPUT
#define INPUT_SIGNAL_PORT_INPUT_ADDR				(&(INPUT_SIGNAL_PORT->IDR))

//===================================================================================

// defines для модуля input_config.h
#define INPUT_CONFIG_GPIO_CLK_ENABLE()				{ __HAL_RCC_GPIOC_CLK_ENABLE(); }

#define INPUT_CONFIG_1_PORT							GPIOC
#define INPUT_CONFIG_1_PIN							GPIO_PIN_7
#define INPUT_CONFIG_1_PIN_INDEX					7
#define INPUT_CONFIG_1_PULL							GPIO_NOPULL
#define INPUT_CONFIG_1_MODE							GPIO_MODE_INPUT
#define INPUT_CONFIG_1_PORT_INPUT_ADDR				(&(INPUT_CONFIG_1_PORT->IDR))

#define INPUT_CONFIG_2_PORT							GPIOC
#define INPUT_CONFIG_2_PIN							GPIO_PIN_8
#define INPUT_CONFIG_2_PIN_INDEX					8
#define INPUT_CONFIG_2_PULL							GPIO_NOPULL
#define INPUT_CONFIG_2_MODE							GPIO_MODE_INPUT
#define INPUT_CONFIG_2_PORT_INPUT_ADDR				(&(INPUT_CONFIG_2_PORT->IDR))

//===================================================================================

// defines для модуля debug.h
#define DEBUG_USART									USART1
#define DEBUG_HUART_NUM								huart1
#define DEBUG_USART_IRQN							USART1_IRQn

#define UART1_IRQ_HANDLER_PROCESSING()				{ HAL_UART_IRQHandler(debug_getUartHandleAddr()); }

#define DEBUG_USART_CLK_ENABLE()					{ __HAL_RCC_USART1_CLK_ENABLE(); }
#define DEBUG_USART_CLK_DISABLE()					{ __HAL_RCC_USART1_CLK_DISABLE(); }
#define DEBUG_USART_GPIO_CLK_ENABLE()				{ __HAL_RCC_GPIOA_CLK_ENABLE(); }
#define DEBUG_TX_PORT								GPIOA
#define DEBUG_RX_PORT								GPIOA
#define DEBUG_TX_PIN								GPIO_PIN_9
#define DEBUG_RX_PIN								GPIO_PIN_10

//===================================================================================

// defines для модуля motor_control.h
// power
#define MOTOR_PWR_GPIO_CLK_ENABLE()					{ __HAL_RCC_GPIOB_CLK_ENABLE(); __HAL_RCC_GPIOC_CLK_ENABLE(); }

#define MAIN_MOTOR_PWR_ON							GPIO_PIN_SET
#define MAIN_MOTOR_PWR_OFF							GPIO_PIN_RESET
#define MAIN_MOTOR_PWR_PORT							GPIOC
#define MAIN_MOTOR_PWR_PIN							GPIO_PIN_0
#define MAIN_MOTOR_PWR_PULL							GPIO_NOPULL
#define MAIN_MOTOR_PWR_MODE							GPIO_MODE_OUTPUT_PP

#define UP_MOTOR_PWR_ON								GPIO_PIN_SET
#define UP_MOTOR_PWR_OFF							GPIO_PIN_RESET
#define UP_MOTOR_PWR_PORT							GPIOC
#define UP_MOTOR_PWR_PIN							GPIO_PIN_5
#define UP_MOTOR_PWR_PULL							GPIO_NOPULL
#define UP_MOTOR_PWR_MODE							GPIO_MODE_OUTPUT_PP

#define DOWN_MOTOR_PWR_ON							GPIO_PIN_SET
#define DOWN_MOTOR_PWR_OFF							GPIO_PIN_RESET
#define DOWN_MOTOR_PWR_PORT							GPIOB
#define DOWN_MOTOR_PWR_PIN							GPIO_PIN_0
#define DOWN_MOTOR_PWR_PULL							GPIO_NOPULL
#define DOWN_MOTOR_PWR_MODE							GPIO_MODE_OUTPUT_PP

#define SIDE_1_MOTOR_PWR_ON							GPIO_PIN_SET
#define SIDE_1_MOTOR_PWR_OFF						GPIO_PIN_RESET
#define SIDE_1_MOTOR_PWR_PORT						GPIOB
#define SIDE_1_MOTOR_PWR_PIN						GPIO_PIN_2
#define SIDE_1_MOTOR_PWR_PULL						GPIO_NOPULL
#define SIDE_1_MOTOR_PWR_MODE						GPIO_MODE_OUTPUT_PP

// switch
#define MOTOR_SWITCH_GPIO_CLK_ENABLE()				{ __HAL_RCC_GPIOB_CLK_ENABLE(); __HAL_RCC_GPIOC_CLK_ENABLE(); }

#define UP_MOTOR_SWITCH_REVERSE						GPIO_PIN_SET
#define UP_MOTOR_SWITCH_DIRECT						GPIO_PIN_RESET
#define UP_MOTOR_SWITCH_PORT						GPIOC
#define UP_MOTOR_SWITCH_PIN							GPIO_PIN_1
#define UP_MOTOR_SWITCH_PULL						GPIO_NOPULL
#define UP_MOTOR_SWITCH_MODE						GPIO_MODE_OUTPUT_PP

#define DOWN_MOTOR_SWITCH_REVERSE					GPIO_PIN_SET
#define DOWN_MOTOR_SWITCH_DIRECT					GPIO_PIN_RESET
#define DOWN_MOTOR_SWITCH_PORT						GPIOB
#define DOWN_MOTOR_SWITCH_PIN						GPIO_PIN_1
#define DOWN_MOTOR_SWITCH_PULL						GPIO_NOPULL
#define DOWN_MOTOR_SWITCH_MODE						GPIO_MODE_OUTPUT_PP

#define SIDE_1_MOTOR_SWITCH_REVERSE					GPIO_PIN_SET
#define SIDE_1_MOTOR_SWITCH_DIRECT					GPIO_PIN_RESET
#define SIDE_1_MOTOR_SWITCH_PORT					GPIOB
#define SIDE_1_MOTOR_SWITCH_PIN						GPIO_PIN_10
#define SIDE_1_MOTOR_SWITCH_PULL					GPIO_NOPULL
#define SIDE_1_MOTOR_SWITCH_MODE					GPIO_MODE_OUTPUT_PP

//===================================================================================

// defines для модуля ext_volt.h
#define EXT_VOLT_DET_GPIO_CLK_ENABLE()				{ __HAL_RCC_GPIOA_CLK_ENABLE(); }

#define MAIN_VOLT_PORT								GPIOA
#define MAIN_VOLT_PIN								GPIO_PIN_2
#define MAIN_VOLT_ADC_CH							ADC_CHANNEL_2

#define RESERVE_VOLT_PORT							GPIOA
#define RESERVE_VOLT_PIN							GPIO_PIN_1
#define RESERVE_VOLT_ADC_CH							ADC_CHANNEL_1

#define EXT_VOLT_DIV_RES_UP_OHM						47000.0f
#define EXT_VOLT_DIV_RES_DOWN_OHM					4700.0f

//===================================================================================

// defines для модуля current_sense.h
#define UP_CURRENT_SENSE_PORT						GPIOA
#define UP_CURRENT_SENSE_PIN						GPIO_PIN_5
#define UP_CURRENT_SENSE_PULL						GPIO_NOPULL

#define DOWN_CURRENT_SENSE_PORT						GPIOA
#define DOWN_CURRENT_SENSE_PIN						GPIO_PIN_3
#define DOWN_CURRENT_SENSE_PULL						GPIO_NOPULL

#define SIDE_1_CURRENT_SENSE_PORT					GPIOA
#define SIDE_1_CURRENT_SENSE_PIN					GPIO_PIN_6
#define SIDE_1_CURRENT_SENSE_PULL					GPIO_NOPULL

#define CURRENT_SENSE_DIV_RES_UP_OHM				10000.0f
#define CURRENT_SENSE_DIV_RES_DOWN_OHM				4700.0f

//===================================================================================

#endif // _HARD_2_H_
