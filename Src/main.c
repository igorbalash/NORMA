#include "app_config.h"
#include "clock_system.h"
#include "leds.h"

//===================================================================================

#include "cmsis_os.h"
#include "adc.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

void MX_FREERTOS_Init(void);

int main(void)
{
	// Инициализация системы тактирования
	clock_system_init();

	// Задержка перед стартом программы
	HAL_Delay(APP_START_DELAY_MS);

	// Инициализация модулей
	leds_init();

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_DMA_Init();
	MX_ADC1_Init();
	MX_USART1_UART_Init();
	MX_TIM3_Init();

	// Инициализация задач FreeRTOS
	main_tasks_initTasks(0);

	// Запуск планировщика
	osKernelStart();

	while (1) {};
}
