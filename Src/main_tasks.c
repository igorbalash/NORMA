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
#include "panel_type.h"
#include "nearby_panel.h"
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

#define CHECK_EXT_VOLTAGE_PERIOD_MS				10000
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
static void check_nearby_panel_loop(void);

//===================================================================================

typedef struct {
	bool flag_isAdcСonvReady;
	bool flag_isNeedRevPolMainMotorPwr;
	bool flag_isNeedPullUpDownMotor;
	bool flag_isNeedPullSideMotor;
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
} motorState_t;

typedef struct {
	PanelType_t gvar_panel_type;
	NearbyPanelState_t gvar_nearby_panel_state;
} globalVars_t;

//===================================================================================

// структура глобальных флагов
globalFlags_t globalFlags = {
	.flag_isAdcСonvReady = false,
	.flag_isNeedRevPolMainMotorPwr = false,
	.flag_isNeedPullUpDownMotor = false,
	.flag_isNeedPullSideMotor = false
};

// структура глобальных ошибок
globalErrors_t globalErrors = {
	.error_ExtPowerError = false,
	.error_UpMotorTimeoutError = false,
	.error_DownMotorTimeoutError = false,
	.error_SideMotorTimeoutError = false
};

// структура глобальных состояний моторов
motorState_t motorState = {
	.state_UpDownMotor = PULL_DOWN_POS,
	.state_SideMotor = PULL_DOWN_POS
};

// структура глобальных переменных
globalVars_t globalVars = {
	.gvar_panel_type = COMMON,																							// тип панели, управляемой ПО
	.gvar_nearby_panel_state = PANNEL_NO_CONNECT																		// состояние соседней панели
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

	// Определение типа панели
	globalVars.gvar_panel_type = panel_type_get_type();

	// Чтение из Flash текущего состояния актуаторов
	// Обновляем состояния
//	motorState.state_SideMotor = ;
//	motorState.state_UpDownMotor = ;
//	globalVars.gvar_nearby_panel_state = ;

	// Проверка условия необходимости автоматического выдвижения актуаторов
	// Если актуаторы до перезагрузки или пропажи питания не были выдвинуты, то планируем их выдвинуть автоматически
	if ((PULL_DOWN_POS == motorState.state_UpDownMotor) && (PULL_DOWN_POS == motorState.state_SideMotor)) {
		globalFlags.flag_isNeedPullUpDownMotor = true;
	}

	while(1)
	{
		// Цикл проверки внешних напряжений
		check_ext_voltage_loop();

		// Проверка наличия ошибок
		ErrorsType_t error_type = check_device_errors();

		// Цикл индикации ошибки
		error_indication_loop(error_type);



//		if (true == globalErrors.error_ExtPowerError) {
//			HAL_UART_Transmit(&huart1, "Voltage Error", 13, 500);
//		}

//		leds_ledOn(GREEN);
//		leds_ledOff(RED);
//		osDelay(500);
//		leds_ledOff(GREEN);
//		leds_ledOn(RED);
//		osDelay(500);

		// Цикл проверки подключенности соседней панели (актуально только для обычной панели)
		if (COMMON == globalVars.gvar_panel_type) {
			check_nearby_panel_loop();
		}

		// Цикл управление моторами
//		motor_control_loop();

		// Цикл индикации состояния моторов
		motor_state_indication_loop();

		reset_iwdg_refresh();
	}

	// Should never come here, else reset MCU
	HAL_NVIC_SystemReset();
}

//===================================================================================

static void check_nearby_panel_loop(void)
{
	#define PANEL_STATE_REPETITIONS_CNT			8
	#define DISCONN_NEARBY_PANEL_PERIOD_MS		(MOTOR_TIMEOUT_MS / PANEL_STATE_REPETITIONS_CNT)
	#define CONN_NEARBY_PANEL_PERIOD_MS			125

	static uint8_t panel_state_bits = ~PANNEL_NO_CONNECT + 1;
	static uint32_t prev_time_ms = 0U - CONN_NEARBY_PANEL_PERIOD_MS;
	uint32_t check_period_ms;

	// Выбираем период сэмплирования в зависимости от того, подключена соседняя панель или нет
	if (PANNEL_CONNECT == globalVars.gvar_nearby_panel_state) {
		check_period_ms = CONN_NEARBY_PANEL_PERIOD_MS;
	} else {
		check_period_ms = DISCONN_NEARBY_PANEL_PERIOD_MS;
	}

	uint32_t curr_time_ms = HAL_GetTick();

	if (curr_time_ms - prev_time_ms > check_period_ms) {
		prev_time_ms = curr_time_ms;

		// Получаем состояние соседней панели
		NearbyPanelState_t curr_panel_state = nearby_panel_get_state();

		// Меняем значение переменной устойчивых состояний
		panel_state_bits = (panel_state_bits << 1) | curr_panel_state;

		// Если в течении PANEL_STATE_REPETITIONS_CNT уровень устойчив - меняем глобальную переменную состояния соседней панели
		if (panel_state_bits == (uint8_t)(~PANNEL_CONNECT + 1)) {
			if (PANNEL_CONNECT != globalVars.gvar_nearby_panel_state) {
				globalVars.gvar_nearby_panel_state = PANNEL_CONNECT;
				#ifdef ON_DEBUG_MESSAGE
					HAL_UART_Transmit(&huart1, "Nearby Panel - CONNECT\r\n", strlen("Nearby Panel - CONNECT\r\n"), 500);
				#endif
			}
		} else if (panel_state_bits == (uint8_t)(~PANNEL_NO_CONNECT + 1)) {
			if (PANNEL_NO_CONNECT != globalVars.gvar_nearby_panel_state) {
				globalVars.gvar_nearby_panel_state = PANNEL_NO_CONNECT;

				// Устанавливаем необходимость выдвинуть актуаторы, т.к. соседняя панель отсоединилась
				globalFlags.flag_isNeedPullUpDownMotor = true;

				#ifdef ON_DEBUG_MESSAGE
					HAL_UART_Transmit(&huart1, "Nearby Panel - DISCONNECT\r\n", strlen("Nearby Panel - DISCONNECT\r\n"), 500);
				#endif
			}
		}
	}
}

static ErrorsType_t check_device_errors(void)
{
	ErrorsType_t error_type;

	#ifdef ON_DEBUG_MESSAGE
	static uint32_t prev_time_ms = 0;
	uint32_t curr_time_ms = HAL_GetTick();
	#endif

	// Проверка условий в порядке приоритета от High к Low
	if (true == globalErrors.error_ExtPowerError) {
		error_type = EXT_POWER_ERROR;
		#ifdef ON_DEBUG_MESSAGE
			if (curr_time_ms - prev_time_ms > 1000) {
				prev_time_ms = curr_time_ms;
				HAL_UART_Transmit(&huart1, "EXT_POWER_ERROR\r\n", strlen("EXT_POWER_ERROR\r\n"), 500);
			}
		#endif
	} else if (true == globalErrors.error_UpMotorTimeoutError) {
		error_type = UP_MOTOR_TIMEOUT_ERROR;
		#ifdef ON_DEBUG_MESSAGE
			if (curr_time_ms - prev_time_ms > 1000) {
				prev_time_ms = curr_time_ms;
				HAL_UART_Transmit(&huart1, "UP_MOTOR_TIMEOUT_ERROR\r\n", strlen("UP_MOTOR_TIMEOUT_ERROR\r\n"), 500);
			}
		#endif
	} else if (true == globalErrors.error_DownMotorTimeoutError) {
		error_type = DOWN_MOTOR_TIMEOUT_ERROR;
		#ifdef ON_DEBUG_MESSAGE
			if (curr_time_ms - prev_time_ms > 1000) {
				prev_time_ms = curr_time_ms;
				HAL_UART_Transmit(&huart1, "DOWN_MOTOR_TIMEOUT_ERROR\r\n", strlen("DOWN_MOTOR_TIMEOUT_ERROR\r\n"), 500);
			}
		#endif
	} else if (true == globalErrors.error_SideMotorTimeoutError) {
		error_type = SIDE_MOTOR_TIMEOUT_ERROR;
		#ifdef ON_DEBUG_MESSAGE
			if (curr_time_ms - prev_time_ms > 1000) {
				prev_time_ms = curr_time_ms;
				HAL_UART_Transmit(&huart1, "SIDE_MOTOR_TIMEOUT_ERROR\r\n", strlen("SIDE_MOTOR_TIMEOUT_ERROR\r\n"), 500);
			}
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
