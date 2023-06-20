/************************************************************************************
                  ** C **
Module:
Author: Unic-Lab <https://unic-lab.ru/>
************************************************************************************/

#include "reset.h"
#include "version.h"
#include "stm32f1xx_hal.h"

#define IWDG_PRESCALER_4				0x00000000u                                     								// IWDG prescaler set to 4
#define IWDG_PRESCALER_8                IWDG_PR_PR_0                                    								// IWDG prescaler set to 8
#define IWDG_PRESCALER_16               IWDG_PR_PR_1                                    								// IWDG prescaler set to 16
#define IWDG_PRESCALER_32               (IWDG_PR_PR_1 | IWDG_PR_PR_0)                  								 	// IWDG prescaler set to 32
#define IWDG_PRESCALER_64               IWDG_PR_PR_2                                   								 	// IWDG prescaler set to 64
#define IWDG_PRESCALER_128              (IWDG_PR_PR_2 | IWDG_PR_PR_0)                  									// IWDG prescaler set to 128
#define IWDG_PRESCALER_256				(IWDG_PR_PR_2 | IWDG_PR_PR_1)                   								// IWDG prescaler set to 256

#define IWDG_KEY_RELOAD                 0x0000AAAAu  																	// IWDG Reload Counter Enable
#define IWDG_KEY_ENABLE                 0x0000CCCCu  																	// IWDG Peripheral Enable
#define IWDG_KEY_WRITE_ACCESS_ENABLE    0x00005555u  																	// IWDG KR Write Access Enable
#define IWDG_KEY_WRITE_ACCESS_DISABLE   0x00000000u  																	// IWDG KR Write Access Disable

#define IWDG_KERNEL_UPDATE_FLAGS		(IWDG_SR_RVU | IWDG_SR_PVU)

void reset_iwdg_init(void)
{
#if defined(ON_WDOG_IN_APP)
	#ifdef MCU_DEBUG_PORT_ACTIVE
		__HAL_DBGMCU_FREEZE_IWDG();																						// IWDG останавливает счет во время breakpoint
	#endif

	// Wait for register to be updated
	while ((IWDG->SR & IWDG_KERNEL_UPDATE_FLAGS) != 0x00u);

	// Enable IWDG. LSI is turned on automatically
	WRITE_REG(IWDG->KR, IWDG_KEY_ENABLE);

	// Enable write access to IWDG_PR and IWDG_RLR registers by writing 0x5555 in KR
	WRITE_REG(IWDG->KR, IWDG_KEY_WRITE_ACCESS_ENABLE);

	// Write to IWDG registers the Prescaler & Reload values to work with
	IWDG->PR = IWDG_PRESCALER_256;
	IWDG->RLR = 0xFFF;

	// Wait for register to be updated
	while ((IWDG->SR & IWDG_KERNEL_UPDATE_FLAGS) != 0x00u);

	// Reload IWDG counter with value defined in the reload register
	WRITE_REG(IWDG->KR, IWDG_KEY_RELOAD);
#endif
}

void reset_iwdg_refresh(void)
{
#if defined(ON_WDOG_IN_APP)
	// Reload IWDG counter with value defined in the reload register
	WRITE_REG(IWDG->KR, IWDG_KEY_RELOAD);
#endif
}
