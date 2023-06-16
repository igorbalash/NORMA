/************************************************************************************
                  ** C **
Module:
Author: Unic-Lab <https://unic-lab.ru/>
************************************************************************************/

#include "actuators.h"
#include "stm32f1xx_hal.h"
#include "version.h"
#include <stdbool.h>

//===================================================================================

void actuators_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	// gpio clock enable
	MOTOR_PWR_GPIO_CLK_ENABLE()

	// configure GPIO pin Output Level
	HAL_GPIO_WritePin(MAIN_MOTOR_PWR_PORT, MAIN_MOTOR_PWR_PIN, MAIN_MOTOR_PWR_OFF);
	HAL_GPIO_WritePin(UP_MOTOR_PWR_PORT, UP_MOTOR_PWR_PIN, UP_MOTOR_PWR_OFF);
	HAL_GPIO_WritePin(DOWN_MOTOR_PWR_PORT, DOWN_MOTOR_PWR_PIN, DOWN_MOTOR_PWR_OFF);
	HAL_GPIO_WritePin(SIDE_1_MOTOR_PWR_PORT, SIDE_1_MOTOR_PWR_PIN, SIDE_1_MOTOR_PWR_OFF);

	// configure GPIO pins
	GPIO_InitStruct.Pin = MAIN_MOTOR_PWR_PIN;
	GPIO_InitStruct.Mode = MAIN_MOTOR_PWR_MODE;
	GPIO_InitStruct.Pull = MAIN_MOTOR_PWR_PULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(MAIN_MOTOR_PWR_PORT, &GPIO_InitStruct);

	// configure GPIO pins
	GPIO_InitStruct.Pin = UP_MOTOR_PWR_PIN;
	GPIO_InitStruct.Mode = UP_MOTOR_PWR_MODE;
	GPIO_InitStruct.Pull = UP_MOTOR_PWR_PULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(UP_MOTOR_PWR_PORT, &GPIO_InitStruct);

	// configure GPIO pins
	GPIO_InitStruct.Pin = DOWN_MOTOR_PWR_PIN;
	GPIO_InitStruct.Mode = DOWN_MOTOR_PWR_MODE;
	GPIO_InitStruct.Pull = DOWN_MOTOR_PWR_PULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(DOWN_MOTOR_PWR_PORT, &GPIO_InitStruct);

	// configure GPIO pins
	GPIO_InitStruct.Pin = SIDE_1_MOTOR_PWR_PIN;
	GPIO_InitStruct.Mode = SIDE_1_MOTOR_PWR_MODE;
	GPIO_InitStruct.Pull = SIDE_1_MOTOR_PWR_PULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(SIDE_1_MOTOR_PWR_PORT, &GPIO_InitStruct);

	// gpio clock enable
	MOTOR_SWITCH_GPIO_CLK_ENABLE()

	// configure GPIO pin Output Level
	HAL_GPIO_WritePin(UP_MOTOR_SWITCH_PORT, UP_MOTOR_SWITCH_PIN, UP_MOTOR_SWITCH_DIRECT);
	HAL_GPIO_WritePin(DOWN_MOTOR_SWITCH_PORT, DOWN_MOTOR_SWITCH_PIN, DOWN_MOTOR_SWITCH_DIRECT);
	HAL_GPIO_WritePin(SIDE_1_MOTOR_SWITCH_PORT, SIDE_1_MOTOR_SWITCH_PIN, SIDE_1_MOTOR_SWITCH_DIRECT);

	// configure GPIO pins
	GPIO_InitStruct.Pin = UP_MOTOR_SWITCH_PIN;
	GPIO_InitStruct.Mode = UP_MOTOR_SWITCH_MODE;
	GPIO_InitStruct.Pull = UP_MOTOR_SWITCH_PULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(UP_MOTOR_SWITCH_PORT, &GPIO_InitStruct);

	// configure GPIO pins
	GPIO_InitStruct.Pin = DOWN_MOTOR_SWITCH_PIN;
	GPIO_InitStruct.Mode = DOWN_MOTOR_SWITCH_MODE;
	GPIO_InitStruct.Pull = DOWN_MOTOR_SWITCH_PULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(DOWN_MOTOR_SWITCH_PORT, &GPIO_InitStruct);

	// configure GPIO pins
	GPIO_InitStruct.Pin = SIDE_1_MOTOR_SWITCH_PIN;
	GPIO_InitStruct.Mode = SIDE_1_MOTOR_SWITCH_MODE;
	GPIO_InitStruct.Pull = SIDE_1_MOTOR_SWITCH_PULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(SIDE_1_MOTOR_SWITCH_PORT, &GPIO_InitStruct);
}

void actuators_start_move_up(ActuatorType_t act_type)
{
	switch (act_type)
	{
		case UP_ACTUATOR:
			HAL_GPIO_WritePin(UP_MOTOR_PWR_PORT, UP_MOTOR_PWR_PIN, UP_MOTOR_PWR_OFF);
			HAL_GPIO_WritePin(UP_MOTOR_SWITCH_PORT, UP_MOTOR_SWITCH_PIN, UP_MOTOR_SWITCH_DIRECT);
			HAL_GPIO_WritePin(UP_MOTOR_PWR_PORT, UP_MOTOR_PWR_PIN, UP_MOTOR_PWR_ON);
			break;
		case DOWN_ACTUATOR:
			HAL_GPIO_WritePin(DOWN_MOTOR_PWR_PORT, DOWN_MOTOR_PWR_PIN, DOWN_MOTOR_PWR_OFF);
			HAL_GPIO_WritePin(DOWN_MOTOR_SWITCH_PORT, DOWN_MOTOR_SWITCH_PIN, DOWN_MOTOR_SWITCH_DIRECT);
			HAL_GPIO_WritePin(DOWN_MOTOR_PWR_PORT, DOWN_MOTOR_PWR_PIN, DOWN_MOTOR_PWR_ON);
			break;
		case SIDE_ACTUATOR:
			HAL_GPIO_WritePin(SIDE_1_MOTOR_SWITCH_PORT, SIDE_1_MOTOR_SWITCH_PIN, SIDE_1_MOTOR_PWR_OFF);
			HAL_GPIO_WritePin(SIDE_1_MOTOR_SWITCH_PORT, SIDE_1_MOTOR_SWITCH_PIN, SIDE_1_MOTOR_SWITCH_DIRECT);
			HAL_GPIO_WritePin(SIDE_1_MOTOR_SWITCH_PORT, SIDE_1_MOTOR_SWITCH_PIN, SIDE_1_MOTOR_PWR_ON);
			break;
		default:
			break;
	}
}

void actuators_start_move_down(ActuatorType_t act_type)
{
	switch (act_type)
	{
		case UP_ACTUATOR:
			HAL_GPIO_WritePin(UP_MOTOR_PWR_PORT, UP_MOTOR_PWR_PIN, UP_MOTOR_PWR_OFF);
			HAL_GPIO_WritePin(UP_MOTOR_SWITCH_PORT, UP_MOTOR_SWITCH_PIN, UP_MOTOR_SWITCH_REVERSE);
			HAL_GPIO_WritePin(UP_MOTOR_PWR_PORT, UP_MOTOR_PWR_PIN, UP_MOTOR_PWR_ON);
			break;
		case DOWN_ACTUATOR:
			HAL_GPIO_WritePin(DOWN_MOTOR_PWR_PORT, DOWN_MOTOR_PWR_PIN, DOWN_MOTOR_PWR_OFF);
			HAL_GPIO_WritePin(DOWN_MOTOR_SWITCH_PORT, DOWN_MOTOR_SWITCH_PIN, DOWN_MOTOR_SWITCH_REVERSE);
			HAL_GPIO_WritePin(DOWN_MOTOR_PWR_PORT, DOWN_MOTOR_PWR_PIN, DOWN_MOTOR_PWR_ON);
			break;
		case SIDE_ACTUATOR:
			HAL_GPIO_WritePin(SIDE_1_MOTOR_SWITCH_PORT, SIDE_1_MOTOR_SWITCH_PIN, SIDE_1_MOTOR_PWR_OFF);
			HAL_GPIO_WritePin(SIDE_1_MOTOR_SWITCH_PORT, SIDE_1_MOTOR_SWITCH_PIN, SIDE_1_MOTOR_SWITCH_REVERSE);
			HAL_GPIO_WritePin(SIDE_1_MOTOR_SWITCH_PORT, SIDE_1_MOTOR_SWITCH_PIN, SIDE_1_MOTOR_PWR_ON);
			break;
		default:
			break;
	}
}

void actuators_stop_move(ActuatorType_t act_type)
{
	switch (act_type)
	{
		case UP_ACTUATOR:
			HAL_GPIO_WritePin(UP_MOTOR_PWR_PORT, UP_MOTOR_PWR_PIN, UP_MOTOR_PWR_OFF);
			break;
		case DOWN_ACTUATOR:
			HAL_GPIO_WritePin(DOWN_MOTOR_PWR_PORT, DOWN_MOTOR_PWR_PIN, DOWN_MOTOR_PWR_OFF);
			break;
		case SIDE_ACTUATOR:
			HAL_GPIO_WritePin(SIDE_1_MOTOR_SWITCH_PORT, SIDE_1_MOTOR_SWITCH_PIN, SIDE_1_MOTOR_PWR_OFF);
			break;
		default:
			break;
	}
}

void actuators_main_power_on(bool is_need_reverse_polarity)
{
	if (false == is_need_reverse_polarity) {
		HAL_GPIO_WritePin(MAIN_MOTOR_PWR_PORT, MAIN_MOTOR_PWR_PIN, MAIN_MOTOR_PWR_ON);
	} else {
		HAL_GPIO_WritePin(MAIN_MOTOR_PWR_PORT, MAIN_MOTOR_PWR_PIN, MAIN_MOTOR_PWR_OFF);
	}
}

void actuators_main_power_off(bool is_need_reverse_polarity)
{
	if (false == is_need_reverse_polarity) {
		HAL_GPIO_WritePin(MAIN_MOTOR_PWR_PORT, MAIN_MOTOR_PWR_PIN, MAIN_MOTOR_PWR_OFF);
	} else {
		HAL_GPIO_WritePin(MAIN_MOTOR_PWR_PORT, MAIN_MOTOR_PWR_PIN, MAIN_MOTOR_PWR_ON);
	}
}

//===================================================================================
