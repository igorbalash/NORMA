/************************************************************************************
                  ** C **
Module:
Author: Unic-Lab <https://unic-lab.ru/>
************************************************************************************/

#include "panel_type.h"
#include "stm32f1xx_hal.h"
#include "version.h"

//===================================================================================

void panel_type_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	// gpio clock enable
	INPUT_CONFIG_GPIO_CLK_ENABLE()

	// configure GPIO pins : mode input
	GPIO_InitStruct.Pin = INPUT_CONFIG_1_PIN;
	GPIO_InitStruct.Mode = INPUT_CONFIG_1_MODE;
	GPIO_InitStruct.Pull = INPUT_CONFIG_1_PULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(INPUT_CONFIG_1_PORT, &GPIO_InitStruct);

	// configure GPIO pins : mode input
	GPIO_InitStruct.Pin = INPUT_CONFIG_2_PIN;
	GPIO_InitStruct.Mode = INPUT_CONFIG_2_MODE;
	GPIO_InitStruct.Pull = INPUT_CONFIG_2_PULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(INPUT_CONFIG_2_PORT, &GPIO_InitStruct);
}

PanelType_t panel_type_get_type(void)
{
	PanelType_t panel_type = COMMON;

	GPIO_PinState config_1 = HAL_GPIO_ReadPin(INPUT_CONFIG_1_PORT, INPUT_CONFIG_1_PIN);
	GPIO_PinState config_2 = HAL_GPIO_ReadPin(INPUT_CONFIG_2_PORT, INPUT_CONFIG_2_PIN);

	if ((GPIO_PIN_RESET == config_1 && GPIO_PIN_RESET == config_2) || (GPIO_PIN_SET == config_1 && GPIO_PIN_SET == config_2)) {
		panel_type = COMMON;
	} else if (GPIO_PIN_RESET == config_1) {
		panel_type = TELESCOPIC_2;
	} else {
		panel_type = TELESCOPIC_4;
	}

	return panel_type;
}

//===================================================================================
