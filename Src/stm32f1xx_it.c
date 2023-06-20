#include "main.h"
#include "stm32f1xx_it.h"
#include "FreeRTOS.h"
#include "task.h"

/* Private includes ----------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "version.h"

/* External variables --------------------------------------------------------*/
extern DMA_HandleTypeDef hdma_adc1;
extern TIM_HandleTypeDef htim1;

/******************************************************************************/
/*           Cortex-M3 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
#ifdef RELEASE_MODE
	// В релизной прошивке перезагружаем МК
	HAL_NVIC_SystemReset();
#else
	// В отладочной прошивке мигаем светодиодом и
	// можем сделать attach to runing targer, чтобы посмотреть из-за чего произошло исключение
	asm("BKPT #0");
	while (1);
#endif
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
#ifdef RELEASE_MODE
	// В релизной прошивке перезагружаем МК
	HAL_NVIC_SystemReset();
#else
	// В отладочной прошивке мигаем светодиодом и
	// можем сделать attach to runing targer, чтобы посмотреть из-за чего произошло исключение
	asm("BKPT #0");
	while (1);
#endif
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
#ifdef RELEASE_MODE
	// В релизной прошивке перезагружаем МК
	HAL_NVIC_SystemReset();
#else
	// В отладочной прошивке мигаем светодиодом и
	// можем сделать attach to runing targer, чтобы посмотреть из-за чего произошло исключение
	asm("BKPT #0");
	while (1);
#endif
}

/**
  * @brief This function handles Prefetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
#ifdef RELEASE_MODE
	// В релизной прошивке перезагружаем МК
	HAL_NVIC_SystemReset();
#else
	// В отладочной прошивке мигаем светодиодом и
	// можем сделать attach to runing targer, чтобы посмотреть из-за чего произошло исключение
	asm("BKPT #0");
	while (1);
#endif
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
#ifdef RELEASE_MODE
	// В релизной прошивке перезагружаем МК
	HAL_NVIC_SystemReset();
#else
	// В отладочной прошивке мигаем светодиодом и
	// можем сделать attach to runing targer, чтобы посмотреть из-за чего произошло исключение
	asm("BKPT #0");
	while (1);
#endif
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
#ifdef RELEASE_MODE
	// В релизной прошивке перезагружаем МК
	HAL_NVIC_SystemReset();
#else
	// В отладочной прошивке мигаем светодиодом и
	// можем сделать attach to runing targer, чтобы посмотреть из-за чего произошло исключение
	asm("BKPT #0");
	while (1);
#endif
}

/******************************************************************************/
/* STM32F1xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f1xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles EXTI line3 interrupt.
  */
void EXTI3_IRQHandler(void)
{
  EXTI3_IRQ_HANDLER_PROCESSING()
}

/**
  * @brief This function handles EXTI line4 interrupt.
  */
void EXTI4_IRQHandler(void)
{
	EXTI4_IRQ_HANDLER_PROCESSING()
}

/**
  * @brief This function handles DMA1 channel1 global interrupt.
  */
void DMA1_Channel1_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&hdma_adc1);
}

/**
  * @brief This function handles EXTI line[9:5] interrupts.
  */
void EXTI9_5_IRQHandler(void)
{
	EXTI9_5_IRQ_HANDLER_PROCESSING()
}

/**
  * @brief This function handles TIM1 update interrupt.
  */
void TIM1_UP_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&htim1);
}
