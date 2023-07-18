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

static void actuators_switch_up(ActuatorType_t act_type)
{
	switch (act_type)
	{
		case UP_ACTUATOR:
			HAL_GPIO_WritePin(UP_MOTOR_SWITCH_PORT, UP_MOTOR_SWITCH_PIN, UP_MOTOR_SWITCH_DIRECT);
			break;
		case DOWN_ACTUATOR:
			HAL_GPIO_WritePin(DOWN_MOTOR_SWITCH_PORT, DOWN_MOTOR_SWITCH_PIN, DOWN_MOTOR_SWITCH_DIRECT);
			break;
		case SIDE_ACTUATOR:
			HAL_GPIO_WritePin(SIDE_1_MOTOR_SWITCH_PORT, SIDE_1_MOTOR_SWITCH_PIN, SIDE_1_MOTOR_SWITCH_DIRECT);
			break;
		default:
			break;
	}
}

static void actuators_switch_down(ActuatorType_t act_type)
{
	switch (act_type)
	{
		case UP_ACTUATOR:
			HAL_GPIO_WritePin(UP_MOTOR_SWITCH_PORT, UP_MOTOR_SWITCH_PIN, UP_MOTOR_SWITCH_REVERSE);
			break;
		case DOWN_ACTUATOR:
			HAL_GPIO_WritePin(DOWN_MOTOR_SWITCH_PORT, DOWN_MOTOR_SWITCH_PIN, DOWN_MOTOR_SWITCH_REVERSE);
			break;
		case SIDE_ACTUATOR:
			HAL_GPIO_WritePin(SIDE_1_MOTOR_SWITCH_PORT, SIDE_1_MOTOR_SWITCH_PIN, SIDE_1_MOTOR_SWITCH_REVERSE);
			break;
		default:
			break;
	}
}

static void actuators_main_power_on(bool is_need_reverse_polarity)
{
	if (false == is_need_reverse_polarity) {
		HAL_GPIO_WritePin(MAIN_MOTOR_PWR_PORT, MAIN_MOTOR_PWR_PIN, MAIN_MOTOR_PWR_ON);
	} else {
		HAL_GPIO_WritePin(MAIN_MOTOR_PWR_PORT, MAIN_MOTOR_PWR_PIN, MAIN_MOTOR_PWR_OFF);
	}
}

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

void actuators_main_power_off(bool is_need_reverse_polarity)
{
	// Снимаем питание с выходов VND5050AK-E на актуаторы
	actuators_power_off(UP_ACTUATOR);
	actuators_power_off(DOWN_ACTUATOR);
	actuators_power_off(SIDE_ACTUATOR);

	HAL_Delay(5);																										// Для VND5050AK-E (< 1ms)

	if (false == is_need_reverse_polarity) {
		HAL_GPIO_WritePin(MAIN_MOTOR_PWR_PORT, MAIN_MOTOR_PWR_PIN, MAIN_MOTOR_PWR_OFF);
	} else {
		HAL_GPIO_WritePin(MAIN_MOTOR_PWR_PORT, MAIN_MOTOR_PWR_PIN, MAIN_MOTOR_PWR_ON);
	}

	// Снимаем питание с катушек реле, путем коммутации актуаторов в направлении для движения "Вверх"
	actuators_switch_up(UP_ACTUATOR);
	actuators_switch_up(DOWN_ACTUATOR);
	actuators_switch_up(SIDE_ACTUATOR);

	HAL_Delay(30);																										// Для реле HF115F/012-2ZS4 (operate time 15ms max.)
}

void actuators_power_on(ActuatorType_t act_type)
{
	switch (act_type)
	{
		case UP_ACTUATOR:
			HAL_GPIO_WritePin(UP_MOTOR_PWR_PORT, UP_MOTOR_PWR_PIN, UP_MOTOR_PWR_ON);
			break;
		case DOWN_ACTUATOR:
			HAL_GPIO_WritePin(DOWN_MOTOR_PWR_PORT, DOWN_MOTOR_PWR_PIN, DOWN_MOTOR_PWR_ON);
			break;
		case SIDE_ACTUATOR:
			HAL_GPIO_WritePin(SIDE_1_MOTOR_PWR_PORT, SIDE_1_MOTOR_PWR_PIN, SIDE_1_MOTOR_PWR_ON);
			break;
		default:
			break;
	}
}

void actuators_power_off(ActuatorType_t act_type)
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
			HAL_GPIO_WritePin(SIDE_1_MOTOR_PWR_PORT, SIDE_1_MOTOR_PWR_PIN, SIDE_1_MOTOR_PWR_OFF);
			break;
		default:
			break;
	}
}

void actuators_prepare_move(Direction_t direction, bool is_need_reverse_polarity)
{
	// Снимаем питание с выходов VND5050AK-E на актуаторы
	actuators_power_off(UP_ACTUATOR);
	actuators_power_off(DOWN_ACTUATOR);
	actuators_power_off(SIDE_ACTUATOR);

	HAL_Delay(5);																										// Для VND5050AK-E (< 1ms)

	switch (direction)
	{
		case UP_DIR:
			// Коммутируем актуаторы в направлении для движения "Вверх"
			actuators_switch_up(UP_ACTUATOR);
			actuators_switch_up(DOWN_ACTUATOR);
			actuators_switch_up(SIDE_ACTUATOR);
			break;
		case DOWN_DIR:
			// Коммутируем актуаторы в направлении для движения "Вниз"
			actuators_switch_down(UP_ACTUATOR);
			actuators_switch_down(DOWN_ACTUATOR);
			actuators_switch_down(SIDE_ACTUATOR);
			break;
		default:
			break;
	}

	// Включаем общее питание
	actuators_main_power_on(is_need_reverse_polarity);

	HAL_Delay(30);																										// Для реле HF115F/012-2ZS4 (operate time 15ms max.)
}

//===================================================================================
