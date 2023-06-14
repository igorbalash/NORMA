/************************************************************************************
                  ** C **
Module:
Author: Unic-Lab <https://unic-lab.ru/>
************************************************************************************/

#include "ext_volt.h"
#include "version.h"
#include "adc.h"

//===================================================================================

#define REF_INT_VOLT_CNT				1
#define EXT_VOLT_CNT					2
#define VOLT_DATA_SAMPLE_SIZE			16

#define DIODE_FORWARD_VOLTAGE_V			0.28f

#define ADC_REF_INT_VOLTAGE_V			1.2f
#define ADC_MAX_VAL						4095
#define ADC_MIN_VOLTAGE_V				DIODE_FORWARD_VOLTAGE_V

//===================================================================================

#define EXT_VOLT_ADC_DATA_SIZE			(sizeof(ext_volt_adc_data) / sizeof(ext_volt_adc_data[0]))
#define EXT_VOLT_DIVIDER_COEFF	 		(EXT_VOLT_DIV_RES_DOWN_OHM / (EXT_VOLT_DIV_RES_UP_OHM + EXT_VOLT_DIV_RES_DOWN_OHM))

uint16_t ext_volt_adc_data[(EXT_VOLT_CNT + REF_INT_VOLT_CNT) * VOLT_DATA_SAMPLE_SIZE] = {0}; 							// по VOLT_DATA_SAMPLE_SIZE элементов в выборке для каждого канала напряжения

float main_volt = 0;
float reserve_volt = 0;

//===================================================================================

void ext_volt_start_measure(void)
{
	// Инициализация АЦП, DMA, TIM и старт измерений
	adc_ext_volt_init((uint32_t*)&ext_volt_adc_data, EXT_VOLT_ADC_DATA_SIZE);
}

void ext_volt_stop_measure(void)
{
	// Деинициализация АЦП, DMA, TIM и стоп измерений
	adc_ext_volt_deinit();
}

void ext_volt_process_adc_data(void)
{
	float tmp_main_volt = 0;
	float tmp_reserve_volt = 0;
	float ref_int_volt = 0;

	for (uint16_t i = 0; i < VOLT_DATA_SAMPLE_SIZE; i += 3) {
		tmp_main_volt += ext_volt_adc_data[i];
		tmp_reserve_volt += ext_volt_adc_data[i + 1];
		ref_int_volt += ext_volt_adc_data[i + 2];
	}

	tmp_main_volt = tmp_main_volt / VOLT_DATA_SAMPLE_SIZE;
	tmp_reserve_volt = tmp_reserve_volt / VOLT_DATA_SAMPLE_SIZE;
	ref_int_volt = ref_int_volt / VOLT_DATA_SAMPLE_SIZE;

	ref_int_volt = ADC_MAX_VAL * ADC_REF_INT_VOLTAGE_V / ref_int_volt;
	tmp_main_volt = ((ref_int_volt / ADC_MAX_VAL) * tmp_main_volt) / EXT_VOLT_DIVIDER_COEFF;
	tmp_reserve_volt = ((ref_int_volt / ADC_MAX_VAL) * tmp_reserve_volt) / EXT_VOLT_DIVIDER_COEFF;

	if (tmp_main_volt >= ADC_MIN_VOLTAGE_V) {
		tmp_main_volt += DIODE_FORWARD_VOLTAGE_V;
	} else {
		tmp_main_volt = 0;
	}

	if (tmp_reserve_volt >= ADC_MIN_VOLTAGE_V) {
		tmp_reserve_volt += DIODE_FORWARD_VOLTAGE_V;
	} else {
		tmp_reserve_volt = 0;
	}

	main_volt = tmp_main_volt;
	reserve_volt = tmp_reserve_volt;
}

float ext_volt_get_main_volt(void)
{
	return main_volt;
}

float ext_volt_get_reserve_volt(void)
{
	return reserve_volt;
}

//===================================================================================
