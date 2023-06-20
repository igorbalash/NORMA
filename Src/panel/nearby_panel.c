/************************************************************************************
                  ** C **
Module:
Author: Unic-Lab <https://unic-lab.ru/>
************************************************************************************/

#include "nearby_panel.h"
#include "stm32f1xx_hal.h"
#include "version.h"

//===================================================================================

void nearby_panel_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	// gpio clock enable
	INPUT_SIGNAL_GPIO_CLK_ENABLE()

	// configure GPIO pins : mode input
	GPIO_InitStruct.Pin = INPUT_SIGNAL_PIN;
	GPIO_InitStruct.Mode = INPUT_SIGNAL_MODE;
	GPIO_InitStruct.Pull = INPUT_SIGNAL_PULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(INPUT_SIGNAL_PORT, &GPIO_InitStruct);
}

NearbyPanelState_t nearby_panel_get_state(void)
{
	NearbyPanelState_t panel_state = PANNEL_NO_CONNECT;

	GPIO_PinState input_signal = HAL_GPIO_ReadPin(INPUT_SIGNAL_PORT, INPUT_SIGNAL_PIN);

	if (INPUT_SIGNAL_ACTIVE_STATE == input_signal) {
		panel_state = PANNEL_CONNECT;
	} else {
		panel_state = PANNEL_NO_CONNECT;
	}

	return panel_state;
}

//===================================================================================
