/************************************************************************************
                  ** C **
Module:
Author: Unic-Lab <https://unic-lab.ru/>
************************************************************************************/

#include "adc.h"
#include "version.h"
#include "stm32f1xx_hal.h"

//===================================================================================

#define EXT_VOLT_SAMPLING_PERIOD_MS			20

//===================================================================================

ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;
TIM_HandleTypeDef htim3;

//===================================================================================

static void ext_volt_adc_init(void)
{
	ADC_ChannelConfTypeDef sConfig = {0};
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	// ADC1 clock enable
	__HAL_RCC_ADC1_CLK_ENABLE();

	// GPIO clock enable
	EXT_VOLT_DET_GPIO_CLK_ENABLE()

	// Configure GPIO pins
	GPIO_InitStruct.Pin = MAIN_VOLT_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(MAIN_VOLT_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = RESERVE_VOLT_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(RESERVE_VOLT_PORT, &GPIO_InitStruct);

	// ADC1 DMA Init
	hdma_adc1.Instance = DMA1_Channel1;
	hdma_adc1.Init.Direction = DMA_PERIPH_TO_MEMORY;
	hdma_adc1.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_adc1.Init.MemInc = DMA_MINC_ENABLE;
	hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
	hdma_adc1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
	hdma_adc1.Init.Mode = DMA_CIRCULAR;
	hdma_adc1.Init.Priority = DMA_PRIORITY_LOW;
	HAL_DMA_Init(&hdma_adc1);

	__HAL_LINKDMA(&hadc1, DMA_Handle, hdma_adc1);

	// Common config
	hadc1.Instance = ADC1;
	hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
	hadc1.Init.ContinuousConvMode = DISABLE;
	hadc1.Init.DiscontinuousConvMode = DISABLE;
	hadc1.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T3_TRGO;
	hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc1.Init.NbrOfConversion = 3;
	HAL_ADC_Init(&hadc1);

	// Configure Regular Channel
	sConfig.Channel = MAIN_VOLT_ADC_CH;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
	HAL_ADC_ConfigChannel(&hadc1, &sConfig);

	// Configure Regular Channel
	sConfig.Channel = RESERVE_VOLT_ADC_CH;
	sConfig.Rank = ADC_REGULAR_RANK_2;
	sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
	HAL_ADC_ConfigChannel(&hadc1, &sConfig);

	// Configure Regular Channel
	sConfig.Channel = ADC_CHANNEL_VREFINT;
	sConfig.Rank = ADC_REGULAR_RANK_3;
	sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
	HAL_ADC_ConfigChannel(&hadc1, &sConfig);
}

static void ext_volt_dma_init(void)
{
	// DMA controller clock enable
	__HAL_RCC_DMA1_CLK_ENABLE();

	// DMA interrupt init
	// DMA1_Channel1_IRQn interrupt configuration
	HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 5, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
}

static void ext_volt_timer_init(void)
{
	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};

	__HAL_RCC_TIM3_CLK_ENABLE();

	uint32_t hclk_freq = HAL_RCC_GetHCLKFreq();
	uint32_t prescaler = (hclk_freq / 100000) - 1;
	uint32_t period = (EXT_VOLT_SAMPLING_PERIOD_MS / (1000.0 / 100000)) - 1;

	htim3.Instance = TIM3;
	htim3.Init.Prescaler = prescaler;
	htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim3.Init.Period = period;
	htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	HAL_TIM_Base_Init(&htim3);

	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig);

	sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig);
}

//===================================================================================

void adc_ext_volt_init(uint32_t* p_buf, uint16_t buf_size)
{
	// Инициализация DMA
	ext_volt_dma_init();

	// Инициализация АЦП
	ext_volt_adc_init();

	// Инициализация TIM
	ext_volt_timer_init();

	HAL_ADCEx_Calibration_Start(&hadc1);
	HAL_ADC_Start_DMA(&hadc1, p_buf, buf_size);
	HAL_TIM_Base_Start(&htim3);
}

void adc_ext_volt_deinit(void)
{
	// Останавливаем TIM
	HAL_TIM_Base_DeInit(&htim3);
	__HAL_RCC_TIM3_CLK_DISABLE();

	// Останавливаем ADC
	HAL_ADC_DeInit(&hadc1);
	__HAL_RCC_ADC1_CLK_DISABLE();
	HAL_GPIO_DeInit(MAIN_VOLT_PORT, MAIN_VOLT_PIN);
	HAL_GPIO_DeInit(RESERVE_VOLT_PORT, RESERVE_VOLT_PIN);

	// Останавливаем DMA
	HAL_DMA_DeInit(&hdma_adc1);
}

//===================================================================================
