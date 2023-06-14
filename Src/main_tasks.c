/************************************************************************************
                  ** C **
Module:
Author: Unic-Lab <https://unic-lab.ru/>
************************************************************************************/

#include "main_tasks.h"
#include "app_config.h"
#include "version.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include "stm32f1xx_hal.h"

#include "leds.h"
#include "reset.h"
#include "ext_volt.h"
#include <stdbool.h>

//===================================================================================
// Временно
#ifdef ON_DEBUG_MESSAGE
	#include "usart.h"
	#include "string.h"
	#include <stdio.h>

	char debug_buf[256] = {0};
#endif
// Временно
//===================================================================================

typedef enum {
	ONCE = 0,
	PERIODICALLY
} RepetitionPeriod_t;

static void check_ext_voltage(RepetitionPeriod_t freq);

//===================================================================================

typedef struct {
	bool flag_isAdcСonvReady;
	bool flag_isNeedRevPolMainMotorPwr;
} globalFlags_t;

typedef struct {
	bool error_ExtPowerFailed;
} globalErrors_t;

//===================================================================================

// структура глобальных флагов
globalFlags_t globalFlags = {
	.flag_isAdcСonvReady = false,
	.flag_isNeedRevPolMainMotorPwr = false
};

// структура глобальных ошибок
globalErrors_t globalErrors = {
	.error_ExtPowerFailed = false
};

//===================================================================================

// Заголовки задач
osThreadId superloop_TaskHandle;

//===================================================================================

// Прототипы задач
void taskFunc_superloop(void const* argument);

//===================================================================================

// Инициализация задач
void main_tasks_initTasks(void)
{
	osThreadDef(t_superloop, taskFunc_superloop, osPriorityNormal, 0, 512);
	superloop_TaskHandle = osThreadCreate(osThread(t_superloop), NULL);
}

//===================================================================================

// Задачи
void taskFunc_superloop(void const* argument)
{
	// Измеряем напряжение основного и резервного источников
	check_ext_voltage(ONCE);



	// Если подключено 2 источника - выдаем ошибку

	// Если подключе резервный ичтоник - меняем полярность управления общим реле по питаню

	while(1)
	{
		check_ext_voltage(ONCE);

		if (true == globalErrors.error_ExtPowerFailed) {
			HAL_UART_Transmit(&huart1, "Voltage Error", 13, 500);
		}

		leds_ledOn(GREEN);
		leds_ledOff(RED);
		osDelay(500);
		leds_ledOff(GREEN);
		leds_ledOn(RED);
		osDelay(500);

		reset_iwdg_refresh();
	}

	// Should never come here, else reset MCU
	HAL_NVIC_SystemReset();
}

//===================================================================================

static void check_ext_voltage(RepetitionPeriod_t period)
{
	float main_volt = 0;
	float reserve_volt = 0;

	switch (period)
	{
		case ONCE:
			// Запускаем измерение
			ext_volt_start_measure();

			// Ожидаем флаг окончания
			while (globalFlags.flag_isAdcСonvReady != true);
			// Сбрасываем флаг
			globalFlags.flag_isAdcСonvReady = false;
			// Останавливаем измерение
			ext_volt_stop_measure();

			// Запускаем обработку данных
			ext_volt_process_adc_data();
			main_volt = ext_volt_get_main_volt();
			reserve_volt = ext_volt_get_reserve_volt();

			#ifdef ON_DEBUG_MESSAGE
				snprintf(debug_buf, sizeof(debug_buf), "Voltage: main = %.2f, reverse = %.2f\r\n", main_volt, reserve_volt);
				HAL_UART_Transmit(&huart1, (uint8_t*)debug_buf, strlen(debug_buf), 500);
			#endif

			if (main_volt != 0) {
				// Реверс полярности управления главным питанием моторов НЕ НУЖЕН
				globalFlags.flag_isNeedRevPolMainMotorPwr = false;

				if ( (main_volt < MIN_EXT_VOLTAGE_V) || (main_volt > MAX_EXT_VOLTAGE_V) ) {
					// Ошибка внешнего питания
					globalErrors.error_ExtPowerFailed = true;
					break;
				}
			} else {
				// Реверс полярности управления главным питанием моторов НУЖЕН
				globalFlags.flag_isNeedRevPolMainMotorPwr = true;
			}

			if (reserve_volt != 0 ) {
				if ( (reserve_volt < MIN_EXT_VOLTAGE_V) || (reserve_volt > MAX_EXT_VOLTAGE_V) ) {
					// Ошибка внешнего питания
					globalErrors.error_ExtPowerFailed = true;
					break;
				}
			}

			globalErrors.error_ExtPowerFailed = false;

			break;
		case PERIODICALLY:

			break;
		default:
			break;
	}
}

//===================================================================================

// Callback
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	if (hadc->Instance == ADC1) {
		globalFlags.flag_isAdcСonvReady = true;
	}
}

//===================================================================================
