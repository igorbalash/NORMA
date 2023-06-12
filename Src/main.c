#include "clock_system.h"
#include "leds.h"

//===================================================================================

#include "cmsis_os.h"
#include "adc.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"


void SystemClock_Config(void);
void MX_FREERTOS_Init(void);

int main(void)
{
	clock_system_init();
	leds_init();


	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_DMA_Init();
	MX_ADC1_Init();
	MX_USART1_UART_Init();
	MX_TIM3_Init();

	/* Call init function for freertos objects (in freertos.c) */
	MX_FREERTOS_Init();
	/* Start scheduler */
	osKernelStart();

	/* We should never get here as control is now taken by the scheduler */
	/* Infinite loop */
	while (1)
	{

	}
}
