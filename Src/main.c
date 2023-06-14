#include "app_config.h"
#include "version.h"
#include "stm32f1xx_hal.h"
#include "clock_system.h"
#include "main_tasks.h"
#include "reset.h"
#include "leds.h"

//===================================================================================

#include "cmsis_os.h"
#include "usart.h"
#include "gpio.h"

void MX_FREERTOS_Init(void);

int main(void)
{
	// Инициализация системы тактирования
	clock_system_init();

	// Задержка перед стартом программы
	HAL_Delay(APP_START_DELAY_MS);

	// Инициализация IWDG
	reset_iwdg_init();
	// Инициализация модулей
	leds_init();

	/* Initialize all configured peripherals */
	MX_GPIO_Init();

#ifdef ON_DEBUG_MESSAGE
	MX_USART1_UART_Init();
#endif

	// Инициализация задач FreeRTOS
	main_tasks_initTasks();

	// Запуск планировщика
	osKernelStart();

	while (1) {};

	// Should never come here, else reset MCU
	HAL_NVIC_SystemReset();
}
