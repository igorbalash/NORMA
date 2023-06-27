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

//===================================================================================

void flash_storage_init(void);
bool flash_storage_backup(uint8_t state_UpMotor, uint8_t state_DownMotor, uint8_t state_SideMotor, uint8_t gvar_nearby_panel_state);
bool flash_storage_restore(uint8_t* state_UpMotor, uint8_t* state_DownMotor, uint8_t* state_SideMotor, uint8_t* gvar_nearby_panel_state);

//===================================================================================

#endif /* _FLASH_STORAGE_H_ */
