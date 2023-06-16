/************************************************************************************
                  ** C **
Module:
Author: Unic-Lab <https://unic-lab.ru/>
************************************************************************************/

#include "current_sense.h"
#include "version.h"
#include "adc.h"

//===================================================================================

#define REF_INT_VOLT_CNT					1
#define CURRENT_SENSE_CH_CNT				3
#define CURRENT_SENSE_DATA_SAMPLE_SIZE		4

#define ADC_REF_INT_VOLTAGE_V				1.2f
#define ADC_MAX_VAL							4095
#define ADC_MIN_CURRENT_A					0.01

//===================================================================================

#define CURRENT_SENSE_ADC_DATA_SIZE			(sizeof(current_sense_adc_data) / sizeof(current_sense_adc_data[0]))
#define CURRENT_SENSE_DIVIDER_COEFF	 		(CURRENT_SENSE_DIV_RES_DOWN_OHM / (CURRENT_SENSE_DIV_RES_UP_OHM + CURRENT_SENSE_DIV_RES_DOWN_OHM))

uint16_t current_sense_adc_data[(CURRENT_SENSE_CH_CNT + REF_INT_VOLT_CNT) * CURRENT_SENSE_DATA_SAMPLE_SIZE] = {0}; 		// по CURRENT_SENSE_DATA_SAMPLE_SIZE элементов в выборке для каждого канала

// В амперах
float up_current = 0;
float down_current = 0;
float side_current = 0;

//===================================================================================

void current_sense_start_measure(void)
{
	// Инициализация АЦП, DMA, TIM и старт измерений
	adc_current_sense_init((uint32_t*)&current_sense_adc_data, CURRENT_SENSE_ADC_DATA_SIZE);
}

void current_sense_stop_measure(void)
{
	// Деинициализация АЦП, DMA, TIM и стоп измерений
	adc_current_senset_deinit();
}

void current_sense_process_adc_data(void)
{
	float tmp_up_current = 0;
	float tmp_down_current = 0;
	float tmp_side_current = 0;
	float ref_int_volt = 0;

	for (uint16_t i = 0; i < CURRENT_SENSE_ADC_DATA_SIZE; i += 4) {
		tmp_up_current += current_sense_adc_data[i];
		tmp_down_current += current_sense_adc_data[i + 1];
		tmp_side_current += current_sense_adc_data[i + 2];
		ref_int_volt += current_sense_adc_data[i + 3];
	}

	// Находим ср. знач. ADC в выборках
	tmp_up_current = tmp_up_current / CURRENT_SENSE_DATA_SAMPLE_SIZE;
	tmp_down_current = tmp_down_current / CURRENT_SENSE_DATA_SAMPLE_SIZE;
	tmp_side_current = tmp_side_current / CURRENT_SENSE_DATA_SAMPLE_SIZE;
	ref_int_volt = ref_int_volt / CURRENT_SENSE_DATA_SAMPLE_SIZE;

	// Находим напряжение питания МК
	ref_int_volt = ADC_MAX_VAL * ADC_REF_INT_VOLTAGE_V / ref_int_volt;
	// Находим напряжения на выходах Sense резисторов High-Side драйверов
	tmp_up_current = ((ref_int_volt / ADC_MAX_VAL) * tmp_up_current) / CURRENT_SENSE_DIVIDER_COEFF;
	tmp_down_current = ((ref_int_volt / ADC_MAX_VAL) * tmp_down_current) / CURRENT_SENSE_DIVIDER_COEFF;
	tmp_side_current = ((ref_int_volt / ADC_MAX_VAL) * tmp_side_current) / CURRENT_SENSE_DIVIDER_COEFF;
	// Находим токи нагрузок
	tmp_up_current = (IN_OUT_CURRENT_RATIO * tmp_up_current) / CURRENT_SENSE_RES_OHM;
	tmp_down_current = (IN_OUT_CURRENT_RATIO * tmp_down_current) / CURRENT_SENSE_RES_OHM;
	tmp_side_current = (IN_OUT_CURRENT_RATIO * tmp_side_current) / CURRENT_SENSE_RES_OHM;

	if (tmp_up_current < ADC_MIN_CURRENT_A) {
		tmp_up_current = 0;
	}

	if (tmp_down_current < ADC_MIN_CURRENT_A) {
		tmp_down_current = 0;
	}

	if (tmp_side_current < ADC_MIN_CURRENT_A) {
		tmp_side_current = 0;
	}

	up_current = tmp_up_current;
	down_current = tmp_down_current;
	side_current = tmp_side_current;
}

float current_sense_get_up_current(void)
{
	return up_current;
}

float current_sense_get_down_current(void)
{
	return down_current;
}

float current_sense_get_side_current(void)
{
	return side_current;
}

//===================================================================================
