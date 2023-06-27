/************************************************************************************
                  ** C **
Module:
Author: Unic-Lab <https://unic-lab.ru/>
************************************************************************************/

#include "flash.h"
#include "stm32f1xx_hal.h"

//===================================================================================

// ATTENTION !
// size должен быть > 0 и кратен 2, т.к. в STM32F103 запись во флеш по 2 байта
bool flash_write(uint32_t addr, const uint32_t *buf, uint16_t size)
{
	bool status = true;

	// Проверка, что size > 0
	if (!(size > 0)) {
		return 0;
	}

	// Если размер не кратен 2 байтам - добиваем доп. байтами
	if ((size % 2) > 0) {
		size += 2 - (size % 2);
	}

	HAL_FLASH_Unlock();

	__IO uint16_t *dest_addr = (__IO uint16_t*)addr;
	__IO uint16_t *src_addr = (__IO uint16_t*)buf;

	for (uint16_t wordNum = 0; wordNum < (size/2); wordNum++)
	{
		if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, (uint32_t)dest_addr, *src_addr) != HAL_OK) {
			status = false;
			break;
		}

		dest_addr++;
		src_addr++;
	}

	HAL_FLASH_Lock();

	return status;
}

bool flash_sector_erase(uint32_t addr)
{
	bool status = true;

	uint32_t PageError = 0;
	FLASH_EraseInitTypeDef EraseInitStruct;

	HAL_FLASH_Unlock();

	EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
	EraseInitStruct.PageAddress = addr;
	EraseInitStruct.NbPages     = 1;

	if (HAL_FLASHEx_Erase(&EraseInitStruct, &PageError) != HAL_OK) {
		status = false;
	}

	HAL_FLASH_Lock();

	return status;
}
