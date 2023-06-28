/************************************************************************************
                  ** H **
Module:
Author: Unic-Lab <https://unic-lab.ru/>
************************************************************************************/

#ifndef _FLASH_H_
#define _FLASH_H_

//===================================================================================

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

//===================================================================================

// ATTENTION !
// size должен быть > 0 и кратен 2, т.к. в STM32F103 запись во флеш по 2 байта
bool flash_write(uint32_t addr, const uint32_t *buf, uint16_t size);
bool flash_sector_erase(uint32_t addr);

static inline void flash_read_buf(uint32_t addr, uint32_t *buf, uint16_t size)
{
	memcpy(buf, (uint32_t *)(addr), size);
}

//===================================================================================

#endif // _FLASH_H_
