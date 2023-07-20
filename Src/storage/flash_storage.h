/************************************************************************************
                  ** H **
Module:
Author: Unic-Lab <https://unic-lab.ru/>
************************************************************************************/

#ifndef _FLASH_STORAGE_H_
#define _FLASH_STORAGE_H_

//===================================================================================

#include <stdint.h>
#include <stdbool.h>
#include "version.h"

//===================================================================================

#define STORAGE_SIZE_SECTORS			4																				// 4 страницы flash выделяем для хранения данных

#if (PRODUCT_HARD == 1)
	#define STORAGE_START_ADDRESS		0x0800F000																		// 1 стр. 1kB
#elif (PRODUCT_HARD == 2)
	#define STORAGE_START_ADDRESS		0x0803E000																		// 1 стр. 2kB
#else
	#error "UNKNOWN PRODUCT_HARD!"
#endif

#define STORAGE_END_ADDRESS				(STORAGE_START_ADDRESS + (_FLASH_SECTOR_SIZE * STORAGE_SIZE_SECTORS))

//===================================================================================

void flash_storage_init(void);
bool flash_storage_backup(uint8_t state_UpMotor, uint8_t state_DownMotor, uint8_t state_SideMotor, uint8_t gvar_nearby_panel_state);
bool flash_storage_restore(uint8_t* state_UpMotor, uint8_t* state_DownMotor, uint8_t* state_SideMotor, uint8_t* gvar_nearby_panel_state);

//===================================================================================

#endif /* _FLASH_STORAGE_H_ */
