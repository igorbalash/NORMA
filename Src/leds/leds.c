/************************************************************************************
                  ** H **
Module:
Author: Unic-Lab <https://unic-lab.ru/>
************************************************************************************/

#include "leds.h"
#include "stm32f1xx_hal.h"

#include "version.h"

//===================================================================================

void leds_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	// gpio clock enable
	LEDS_GPIO_CLK_ENABLE()

	// configure GPIO pin Output Level
	HAL_GPIO_WritePin(LED_RED_PORT, LED_RED_PIN, LED_OFF);
	HAL_GPIO_WritePin(LED_GREEN_PORT, LED_GREEN_PIN, LED_OFF);

	// configure GPIO pins
	GPIO_InitStruct.Pin = LED_RED_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LED_RED_PORT, &GPIO_InitStruct);

	// configure GPIO pins
	GPIO_InitStruct.Pin = LED_GREEN_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LED_GREEN_PORT, &GPIO_InitStruct);
}

void leds_ledOn(LedColor_t color)
{
	switch (color)
	{
		case RED:
			HAL_GPIO_WritePin(LED_RED_PORT, LED_RED_PIN, LED_ON);
			break;
		case GREEN:
			HAL_GPIO_WritePin(LED_GREEN_PORT, LED_GREEN_PIN, LED_ON);
			break;
		default:
			break;
	}
}

void leds_ledOff(LedColor_t color)
{
	switch (color)
	{
		case RED:
			HAL_GPIO_WritePin(LED_RED_PORT, LED_RED_PIN, LED_OFF);
			break;
		case GREEN:
			HAL_GPIO_WritePin(LED_GREEN_PORT, LED_GREEN_PIN, LED_OFF);
			break;
		default:
			break;
	}
}

void leds_ledToggle(LedColor_t color)
{
	switch (color)
	{
		case RED:
			HAL_GPIO_TogglePin(LED_RED_PORT, LED_RED_PIN);
			break;
		case GREEN:
			HAL_GPIO_TogglePin(LED_GREEN_PORT, LED_GREEN_PIN);
			break;
		default:
			break;
	}
}

//===================================================================================
