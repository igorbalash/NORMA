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

typedef enum {
	NO_ERROR = 0,
	EXT_POWER_ERROR,
	UP_MOTOR_TIMEOUT_ERROR,
	DOWN_MOTOR_TIMEOUT_ERROR,
	SIDE_MOTOR_TIMEOUT_ERROR
} ErrorsType_t;

static void check_ext_voltage_loop(RepetitionPeriod_t freq);
static void check_device_errors(void);
static void error_indication_loop(ErrorsType_t error_type);

//===================================================================================

typedef struct {
	bool flag_isAdcСonvReady;
	bool flag_isNeedRevPolMainMotorPwr;
} globalFlags_t;

typedef struct {
	bool error_ExtPowerError;
	bool error_UpMotorTimeoutError;
	bool error_DownMotorTimeoutError;
	bool error_SideMotorTimeoutError;
} globalErrors_t;

//===================================================================================

// структура глобальных флагов
globalFlags_t globalFlags = {
	.flag_isAdcСonvReady = false,
	.flag_isNeedRevPolMainMotorPwr = false
};

// структура глобальных ошибок
globalErrors_t globalErrors = {
	.error_ExtPowerError = false,
	.error_UpMotorTimeoutError = false,
	.error_DownMotorTimeoutError = false,
	.error_SideMotorTimeoutError = false
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
	check_ext_voltage_loop(ONCE);

	// Чтение из Flash текущего состояния актуаторов



	// Если подключено 2 источника - выдаем ошибку

	// Если подключе резервный ичтоник - меняем полярность управления общим реле по питаню

	while(1)
	{
		// Проверка внешних напряжений
		check_ext_voltage_loop(ONCE);

		// Проверка ошибок
		check_device_errors();

		if (true == globalErrors.error_ExtPowerError) {
			HAL_UART_Transmit(&huart1, "Voltage Error", 13, 500);
		}

//		leds_ledOn(GREEN);
//		leds_ledOff(RED);
//		osDelay(500);
//		leds_ledOff(GREEN);
//		leds_ledOn(RED);
//		osDelay(500);

		reset_iwdg_refresh();
	}

	// Should never come here, else reset MCU
	HAL_NVIC_SystemReset();
}

//===================================================================================

static void check_device_errors(void)
{
	ErrorsType_t error_type;

	// Проверка условий в порядке приоритета от High к Low
	if (true == globalErrors.error_ExtPowerError) {
		error_type = EXT_POWER_ERROR;
		#ifdef ON_DEBUG_MESSAGE
			HAL_UART_Transmit(&huart1, "EXT_POWER_ERROR\r\n", strlen("EXT_POWER_ERROR\r\n"), 500);
		#endif
	} else if (true == globalErrors.error_UpMotorTimeoutError) {
		error_type = UP_MOTOR_TIMEOUT_ERROR;
		#ifdef ON_DEBUG_MESSAGE
			HAL_UART_Transmit(&huart1, "UP_MOTOR_TIMEOUT_ERROR\r\n", strlen("UP_MOTOR_TIMEOUT_ERROR\r\n"), 500);
		#endif
	} else if (true == globalErrors.error_DownMotorTimeoutError) {
		error_type = DOWN_MOTOR_TIMEOUT_ERROR;
		#ifdef ON_DEBUG_MESSAGE
			HAL_UART_Transmit(&huart1, "DOWN_MOTOR_TIMEOUT_ERROR\r\n", strlen("DOWN_MOTOR_TIMEOUT_ERROR\r\n"), 500);
		#endif
	} else if (true == globalErrors.error_SideMotorTimeoutError) {
		error_type = SIDE_MOTOR_TIMEOUT_ERROR;
		#ifdef ON_DEBUG_MESSAGE
			HAL_UART_Transmit(&huart1, "SIDE_MOTOR_TIMEOUT_ERROR\r\n", strlen("SIDE_MOTOR_TIMEOUT_ERROR\r\n"), 500);
		#endif
	} else {
		error_type = NO_ERROR;
	}

	// Индикация ошибки
	error_indication_loop(error_type);
}

static void error_indication_loop(ErrorsType_t error_type)
{
	static ErrorsType_t prev_error_type = NO_ERROR;
	static uint32_t prev_time_ms = 0;

	uint32_t curr_time_ms = HAL_GetTick();

	if (prev_error_type != error_type) {
		prev_error_type = error_type;
		prev_time_ms = curr_time_ms;
	}

	switch (error_type)
	{
		case EXT_POWER_ERROR:
			leds_ledOn(RED);
			break;
		case UP_MOTOR_TIMEOUT_ERROR:
			if (curr_time_ms - prev_time_ms > 500) {
				prev_time_ms = curr_time_ms;
				leds_ledToggle(RED);
			}
			break;
		case DOWN_MOTOR_TIMEOUT_ERROR:
			if (curr_time_ms - prev_time_ms > 1000) {
				prev_time_ms = curr_time_ms;
				leds_ledToggle(RED);
			}
			break;
		case SIDE_MOTOR_TIMEOUT_ERROR:
			if (curr_time_ms - prev_time_ms > 250) {
				prev_time_ms = curr_time_ms;
				leds_ledToggle(RED);
			}
			break;
		case NO_ERROR:
			leds_ledOff(RED);
			break;
		default:
			break;
	}
}

static void check_ext_voltage_loop(RepetitionPeriod_t period)
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
					// Устанавливаем ошибку внешнего питания
					globalErrors.error_ExtPowerError = true;
					break;
				}
			} else {
				// Реверс полярности управления главным питанием моторов НУЖЕН
				globalFlags.flag_isNeedRevPolMainMotorPwr = true;
			}

			if (reserve_volt != 0 ) {
				if ( (reserve_volt < MIN_EXT_VOLTAGE_V) || (reserve_volt > MAX_EXT_VOLTAGE_V) ) {
					// Устанавливаем ошибку внешнего питания
					globalErrors.error_ExtPowerError = true;
					break;
				}
			}

			// Сбрасываем ошибку внешнего питания
			globalErrors.error_ExtPowerError = false;

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
