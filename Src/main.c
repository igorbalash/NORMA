#include "app_config.h"
#include "version.h"
#include "stm32f1xx_hal.h"
#include "clock_system.h"
#include "main_tasks.h"
#include "reset.h"
#include "leds.h"
#include "panel_type.h"
#include "nearby_panel.h"
#include "actuators.h"
#include "buttons.h"
#include "cmsis_os.h"
#include "flash_storage.h"
#include "flash.h"
#include "storage.h"

#ifdef ON_DEBUG_MESSAGE
	#include "debug.h"
#endif

//===================================================================================

static void boot_mode_proc(void);

//===================================================================================

int main(void)
{
	// Инициализация системы тактирования
	clock_system_init();

	// Инициализация светодиодов
	leds_init();

	// Обработка необходимости сброса на default настройки
	boot_mode_proc();

	// Инициализация IWDG
	reset_iwdg_init();

	// Инициализация модулей
	actuators_init();
	buttons_init();
	panel_type_init();
	nearby_panel_init();

#ifdef ON_DEBUG_MESSAGE
	debug_init();
#endif

	// Инициализация задач FreeRTOS
	main_tasks_initTasks();

	// Инициализация mutex FreeRTOS
	main_tasks_initOsMutex();

	// Инициализация таймеров FreeRTOS
	main_tasks_initOsTimer();

	// Запуск планировщика
	osKernelStart();

	while (1) {};

	// Should never come here, else reset MCU
	HAL_NVIC_SystemReset();
}

static void boot_mode_proc(void)
{
	#define MEASURE_PERION_MS	125
	#define HOLD_DURATION_MS	10000

	bool is_button_was_pressed = false;
	bool is_button_holding = false;
	bool is_timeout = false;

	uint16_t holding_cnt = 0;
	uint32_t current_time_ms;
	uint32_t timeout_prev_time_ms;
	uint32_t holding_prev_time_ms;

	// Инициализация кнопки Stop на вход
	buttons_stopButtonInit();

	timeout_prev_time_ms = HAL_GetTick();
	holding_prev_time_ms = timeout_prev_time_ms;

	while((true != is_timeout) || (false != is_button_holding))
	{
		current_time_ms = HAL_GetTick();

		// Проверка таймаута
		if ((current_time_ms - timeout_prev_time_ms) >= APP_START_DELAY_MS) {
			is_timeout = true;
		}

		// Проверка удержания кнопки с периодом MEASURE_PERION_MS
		if ((current_time_ms - holding_prev_time_ms) >= MEASURE_PERION_MS) {
			holding_prev_time_ms = current_time_ms;

			if (holding_cnt < (HOLD_DURATION_MS / MEASURE_PERION_MS)) {
				if (PRESSED_BTN == buttons_getState(STOP_BTN)) {
					is_button_was_pressed = true;
					is_button_holding = true;
					holding_cnt++;
				} else {
					is_button_holding = false;
					holding_cnt = 0;
				}
			} else {
				// Очищаем Flash с состояниями актуаторов
				for (uint8_t i = 0; i < STORAGE_SIZE_SECTORS; i++) {
					uint32_t sector_addr = STORAGE_START_ADDRESS + (i * _FLASH_SECTOR_SIZE);
					flash_sector_erase(sector_addr);
				}

				// Индиицруем процесс окончания сброса на default настройки
				leds_ledOn(GREEN);
				leds_ledOff(RED);
				while (true)
				{
					leds_ledToggle(RED);
					leds_ledToggle(GREEN);
					HAL_Delay(250);
				}
			}
		}
	}

	// Если кнопка была нажата, а потом отпущена до окончания процесса сброса настроек - делаем паузу до запуска ПО
	 if ((true == is_timeout) && (true == is_button_was_pressed)) {
		leds_ledOn(GREEN);
		HAL_Delay(APP_START_DELAY_MS);
	}
}
