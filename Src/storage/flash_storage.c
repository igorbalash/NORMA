/************************************************************************************
                  ** C **
Module:
Author: Unic-Lab <https://unic-lab.ru/>
************************************************************************************/

#include "flash_storage.h"
#include "version.h"
#include "storage.h"

//===================================================================================

#define STORAGE_SIZE_SECTORS			2																				// 2 страницы flash выделяем для хранения данных

#if (PRODUCT_HARD == 1)
	#define STORAGE_START_ADDRESS		0x0800F800
#elif (PRODUCT_HARD == 2)
	#define STORAGE_START_ADDRESS		0x0803F000
#else
	#error "UNKNOWN PRODUCT_HARD!"
#endif

#define STORAGE_END_ADDRESS				(STORAGE_START_ADDRESS + (_FLASH_SECTOR_SIZE * STORAGE_SIZE_SECTORS))

//===================================================================================

static uint32_t data[STORAGE_MAX_DATA_LENGTH];

static struct DataRecord record = {
	.head_addr = STORAGE_START_ADDRESS,
	.counter = 0,
	.data = data,
	.data_size = sizeof(data),
	.section_start = STORAGE_START_ADDRESS,
	.section_end = STORAGE_END_ADDRESS
};

//===================================================================================

bool flash_storage_backup(uint8_t state_UpMotor, uint8_t state_DownMotor, uint8_t state_SideMotor, uint8_t gvar_nearby_panel_state)
{
	bool ret_state = false;

	record.data[0] = ((state_UpMotor << 24) | (state_DownMotor << 16) | (state_SideMotor << 8) | gvar_nearby_panel_state) & 0xFFFFFFFF;
	record.data[1] = 0xFFFFFFFF;	// reserve
	record.data[2] = 0xFFFFFFFF;	// reserve
	record.data[3] = 0xFFFFFFFF;	// reserve

	// Write data record, retry in case of error
	for (int retries = 0; retries < 3; retries++) {
		uint32_t check_ctr = record.counter;
		if (storage_write(&record) == STORAGE_STATUS_SUCCESS) {
			if (storage_read(&record) == STORAGE_STATUS_SUCCESS) {
				if (check_ctr + 1 == record.counter) {
					ret_state = true;
					break;
				}
			}
		}
	}

	return ret_state;
}

bool flash_storage_restore(uint8_t* state_UpMotor, uint8_t* state_DownMotor, uint8_t* state_SideMotor, uint8_t* gvar_nearby_panel_state)
{
	bool ret_state = false;

	// Initialize flash and storage
	if ( STORAGE_STATUS_SUCCESS == storage_init(&record) ) {
		storage_read(&record);

		*state_UpMotor = (uint8_t)((record.data[0] & 0xFF000000) >> 24);
		*state_DownMotor = (uint8_t)((record.data[0] & 0xFF0000) >> 16);
		*state_SideMotor = (uint8_t)((record.data[0] & 0xFF00) >> 8);
		*gvar_nearby_panel_state = (uint8_t)(record.data[0] & 0xFF);

		ret_state = true;
	}

	return ret_state;
}

void flash_storage_init(void)
{
	// Initialize flash and storage
	storage_init(&record);
}
