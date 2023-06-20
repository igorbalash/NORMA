/************************************************************************************
                  ** C **
Module:
Author: Unic-Lab <https://unic-lab.ru/>
************************************************************************************/

#include "buttons.h"
#include "stm32f1xx_hal.h"
#include "version.h"

//===================================================================================

void buttons_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	// gpio clock enable
	BUTTONS_GPIO_CLK_ENABLE()

	// configure GPIO pins : mode input
	GPIO_InitStruct.Pin = UP_BUTTON_PIN;
	GPIO_InitStruct.Mode = UP_BUTTON_MODE;
	GPIO_InitStruct.Pull = UP_BUTTON_PULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(UP_BUTTON_PORT, &GPIO_InitStruct);

	// EXTI interrupt init
	HAL_NVIC_SetPriority(UP_BUTTON_EXTI_IRQN, 5, 0);
	HAL_NVIC_EnableIRQ(UP_BUTTON_EXTI_IRQN);

	// configure GPIO pins : mode input
	GPIO_InitStruct.Pin = DOWN_BUTTON_PIN;
	GPIO_InitStruct.Mode = DOWN_BUTTON_MODE;
	GPIO_InitStruct.Pull = DOWN_BUTTON_PULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(DOWN_BUTTON_PORT, &GPIO_InitStruct);

	// EXTI interrupt init
	HAL_NVIC_SetPriority(DOWN_BUTTON_EXTI_IRQN, 5, 0);
	HAL_NVIC_EnableIRQ(DOWN_BUTTON_EXTI_IRQN);

	// configure GPIO pins : mode input
	GPIO_InitStruct.Pin = STOP_BUTTON_PIN;
	GPIO_InitStruct.Mode = STOP_BUTTON_MODE;
	GPIO_InitStruct.Pull = STOP_BUTTON_PULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(STOP_BUTTON_PORT, &GPIO_InitStruct);

	// EXTI interrupt init
	HAL_NVIC_SetPriority(STOP_BUTTON_EXTI_IRQN, 5, 0);
	HAL_NVIC_EnableIRQ(STOP_BUTTON_EXTI_IRQN);
}

ButtonState_t buttons_getState(ButtonType_t buttonType)
{
	ButtonState_t ret_state = NOT_PRESSED_BTN;
	GPIO_PinState lineStatus;

	switch (buttonType)
	{
		case UP_BTN:
		{
			lineStatus = HAL_GPIO_ReadPin(UP_BUTTON_PORT, UP_BUTTON_PIN);
			if (UP_BUTTON_PRESSED_STATE == lineStatus) {
				ret_state = PRESSED_BTN;
			}
			break;
		}
		case DOWN_BTN:
		{
			lineStatus = HAL_GPIO_ReadPin(DOWN_BUTTON_PORT, DOWN_BUTTON_PIN);
			if (DOWN_BUTTON_PRESSED_STATE == lineStatus) {
				ret_state = PRESSED_BTN;
			}
			break;
		}
		case STOP_BTN:
		{
			lineStatus = HAL_GPIO_ReadPin(STOP_BUTTON_PORT, STOP_BUTTON_PIN);
			if (STOP_BUTTON_PRESSED_STATE == lineStatus) {
				ret_state = PRESSED_BTN;
			}
			break;
		}
		default:
			break;
	}

	return ret_state;
}

//===================================================================================
