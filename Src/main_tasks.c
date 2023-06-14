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

#define CHECK_EXT_VOLTAGE_PERIOD_MS				15000
#define MOTOR_MOVING_INDICATION_PERIOD_MS		1000

//===================================================================================

typedef enum {
	READY = 0,
	RUNNING
} ProcessState_t;

typedef enum {
	PULL_DOWN_POS = 0,
	PULL_UP_POS,
	MOVING_POS
} MotorPosition_t;

typedef enum {
	NO_ERROR = 0,
	EXT_POWER_ERROR,
	UP_MOTOR_TIMEOUT_ERROR,
	DOWN_MOTOR_TIMEOUT_ERROR,
	SIDE_MOTOR_TIMEOUT_ERROR
} ErrorsType_t;

static void check_ext_voltage_loop(void);
static ErrorsType_t check_device_errors(void);
static void error_indication_loop(ErrorsType_t error_type);
static void motor_state_indication_loop(void);

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

typedef struct {
	MotorPosition_t state_UpDownMotor;
	MotorPosition_t state_SideMotor;
} MotorState_t;

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

// структура глобальных состояний моторов
MotorState_t motorState = {
	.state_UpDownMotor = PULL_DOWN_POS,
	.state_SideMotor = PULL_DOWN_POS
};

//===================================================================================

// Заголовки задач
osThreadId superloop_TaskHandle;

// Заголовки мьютексов
osMutexId adcUse_MutexHandle;

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

// Инициализация mutex
void main_tasks_initOsMutex(void)
{
	osMutexDef(m_adcUse	);																								// создание mutex для управления доступом к ADC
	adcUse_MutexHandle = osMutexCreate(osMutex(m_adcUse));
}

//===================================================================================

// Задачи
void taskFunc_superloop(void const* argument)
{
	// Индикация появившегося питания
	leds_ledOn(GREEN);

	// Чтение из Flash текущего состояния актуаторов

	while(1)
	{
		// Цикл проверки внешних напряжений
		check_ext_voltage_loop();

		// Проверка наличия ошибок
		ErrorsType_t error_type = check_device_errors();

		// Цикл индикации ошибки
		error_indication_loop(error_type);

		// Цикл индикации состояния моторов
		motor_state_indication_loop();

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

static ErrorsType_t check_device_errors(void)
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

	return error_type;
}

static void motor_state_indication_loop(void)
{
	MotorPosition_t motor_pos = PULL_DOWN_POS;
	static uint32_t prev_time_ms = 0;

	uint32_t curr_time_ms = HAL_GetTick();

	if (motorState.state_UpDownMotor == MOVING_POS || motorState.state_SideMotor == MOVING_POS ) {
		motor_pos = MOVING_POS;
	}

	switch (motor_pos)
	{
		case PULL_DOWN_POS:
		case PULL_UP_POS:
			leds_ledOn(GREEN);
			break;
		case MOVING_POS:
			if (curr_time_ms - prev_time_ms > MOTOR_MOVING_INDICATION_PERIOD_MS) {
				prev_time_ms = curr_time_ms;
				leds_ledToggle(GREEN);
			}
			break;
		default:
			break;
	}
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

static void check_ext_voltage_loop(void)
{
	static ProcessState_t loop_state = READY;
	static uint32_t prev_time_ms = 0U - CHECK_EXT_VOLTAGE_PERIOD_MS;

	uint32_t curr_time_ms = HAL_GetTick();

	switch (loop_state)
	{
		case READY:
			if (curr_time_ms - prev_time_ms > CHECK_EXT_VOLTAGE_PERIOD_MS) {
				prev_time_ms = curr_time_ms;

				// Защищаем доступ к ADC с помощью mutex
				osStatus ret_stat = osMutexWait(adcUse_MutexHandle, 0);

				// Если доступ получен - выполняем цикл
				if (ret_stat == osOK) {
					loop_state = RUNNING;
					// Запускаем измерение
					ext_volt_start_measure();
				}

			}
			break;
		case RUNNING:
			if (true == globalFlags.flag_isAdcСonvReady) {
				// Останавливаем измерение
				ext_volt_stop_measure();
				// Сбрасываем флаг
				globalFlags.flag_isAdcСonvReady = false;
				// Запускаем обработку данных
				ext_volt_process_adc_data();

				float main_volt = ext_volt_get_main_volt();
				float reserve_volt = ext_volt_get_reserve_volt();

				#ifdef ON_DEBUG_MESSAGE
					snprintf(debug_buf, sizeof(debug_buf), "Voltage: main = %.2f, reverse = %.2f\r\n", main_volt, reserve_volt);
					HAL_UART_Transmit(&huart1, (uint8_t*)debug_buf, strlen(debug_buf), 500);
				#endif

				// Предварительно сбрасываем ошибку внешнего питания
				globalErrors.error_ExtPowerError = false;

				if (main_volt != 0) {
					// Реверс полярности управления главным питанием моторов НЕ НУЖЕН
					globalFlags.flag_isNeedRevPolMainMotorPwr = false;

					if ( (main_volt < MIN_EXT_VOLTAGE_V) || (main_volt > MAX_EXT_VOLTAGE_V) ) {
						// Устанавливаем ошибку внешнего питания
						globalErrors.error_ExtPowerError = true;
					}
				} else {
					// Реверс полярности управления главным питанием моторов НУЖЕН
					globalFlags.flag_isNeedRevPolMainMotorPwr = true;
				}

				if (reserve_volt != 0 ) {
					if ( (reserve_volt < MIN_EXT_VOLTAGE_V) || (reserve_volt > MAX_EXT_VOLTAGE_V) ) {
						// Устанавливаем ошибку внешнего питания
						globalErrors.error_ExtPowerError = true;
					}
				}

				loop_state = READY;

				// Освобождаем доступ к ADC, возвращая mutex
				osMutexRelease(adcUse_MutexHandle);
			}
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
