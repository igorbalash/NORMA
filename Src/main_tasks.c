/************************************************************************************
                  ** C **
Module:
Author: Unic-Lab <https://unic-lab.ru/>
************************************************************************************/

#include "main_tasks.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include "stm32f1xx_hal.h"

#include "leds.h"
#include "reset.h"

//===================================================================================

// Заголовки задач
osThreadId superloop_TaskHandle;

//===================================================================================

// Прототипы задач
void taskFunc_superloop(void const* argument);

//===================================================================================

void main_tasks_initTasks(void)
{
	osThreadDef(t_superloop, taskFunc_superloop, osPriorityNormal, 0, 512);
	superloop_TaskHandle = osThreadCreate(osThread(t_superloop), NULL);
}

//===================================================================================

void taskFunc_superloop(void const* argument)
{
	while(1)
	{
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