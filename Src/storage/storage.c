/************************************************************************************
                  ** C **
Module:
Author: Unic-Lab <https://unic-lab.ru/>
************************************************************************************/

#include "storage.h"
#include "flash.h"
#include "crc16_ccit.h"

#ifdef ON_DEBUG_MESSAGE
	#include "debug.h"
	#include "string.h"
	#include <stdio.h>

	static char debug_buf[256] = {0};
#endif

//===================================================================================

#define STORAGE_REC_HEADER_LEN				4
#define STORAGE_MAX_BUFFER_LENGTH			(STORAGE_MAX_DATA_LENGTH + STORAGE_REC_HEADER_LEN)							// количество uint32_t элементов

//===================================================================================

static uint32_t buf[STORAGE_MAX_BUFFER_LENGTH];

//===================================================================================

static inline uint16_t rec_data_len( struct DataRecord *rec )
{
	return rec->data_size / sizeof(rec->data[0]);
}

static inline uint16_t rec_len( struct DataRecord *rec )
{
	return (rec_data_len(rec) + STORAGE_REC_HEADER_LEN);
}

static inline uint16_t rec_size( struct DataRecord *rec )
{
	return rec_len(rec) * sizeof(rec->data[0]);
}

static uint32_t crc_address(uint32_t addr, uint16_t data_size)
{
	uint8_t extra_bytes = data_size % 8;

	data_size /= 8;

	if ( extra_bytes > 0 ) {
		data_size++;
	}

	data_size *= 8;

	return addr + data_size;
}

//===================================================================================

enum storage_status storage_init( struct DataRecord *rec )
{
	uint32_t tail_addr = 0;
	uint32_t counter = 0;

	enum storage_status s = storage_get_tail(rec, &tail_addr, &counter);

	if ( s == STORAGE_STATUS_SUCCESS || s == STORAGE_STATUS_CRC_MISMATCH ) {
		// Проверка, если адрес >= адреса последней страницы, то устанавливаем его на адрес первой страницы
		if ( tail_addr + rec_size(rec) >= rec->section_end ) {
			rec->head_addr = rec->section_start;
		} else {
			rec->head_addr = tail_addr + rec_size(rec);
		}

		rec->counter = counter + 1;
	} else {
		rec->head_addr = rec->section_start;
		rec->counter = 0;
	}
	return s;
}

	/**
	Let's define some rules:
	1)   Addressing always occurs in a circular fashion: when @head pointer
		 reaches end of the last sector dedicated to the data section,
		 the pointer is returned back to the beginning
	2)   Number of sectors dedicated to a particular data section
		 equals at least ceil(2 * section_size), where @section_size is the number of
		 sectors required to store the data section. This is done for 2 reasons:
		 - no need to buffer sector contents prior to erasure
		 - wear-leveling
	3)   When sector boundary is being crossed, the next sector is always erased.
	*/
enum storage_status storage_write( struct DataRecord *rec )
{
	uint32_t head_addr = rec->head_addr;
	uint16_t crc = crc16_ccit((const uint8_t *) rec->data, rec->data_size, 0);
	const uint16_t size = rec_size(rec); // assume size < PAGE_SIZE
	const uint16_t len = rec_len(rec);

	uint32_t pos = head_addr % _FLASH_PAGE_SIZE;

	#ifdef ON_DEBUG_MESSAGE
		snprintf(debug_buf, sizeof(debug_buf), "Current offset in current page: %d\n", pos);
		HAL_UART_Transmit(&huart1, (uint8_t*)debug_buf, strlen(debug_buf), 500);
	#endif

	uint32_t next_sector = head_addr - (head_addr % _FLASH_SECTOR_SIZE) + _FLASH_SECTOR_SIZE;

	#ifdef ON_DEBUG_MESSAGE
		snprintf(debug_buf, sizeof(debug_buf), "Next free sector: %X\n", next_sector);
		HAL_UART_Transmit(&huart1, (uint8_t*)debug_buf, strlen(debug_buf), 500);
	#endif

	if ( head_addr % _FLASH_SECTOR_SIZE == 0 ) {
		// erase current sector
		flash_sector_erase(head_addr);

		#ifdef ON_DEBUG_MESSAGE
			snprintf(debug_buf, sizeof(debug_buf), "We need to erase current sector: %X\n", head_addr);
			HAL_UART_Transmit(&huart1, (uint8_t*)debug_buf, strlen(debug_buf), 500);
		#endif
	}

	if ( pos + size > _FLASH_PAGE_SIZE ) {
		// we shall be crossing page boundary - advance
		head_addr += size;

		#ifdef ON_DEBUG_MESSAGE
			HAL_UART_Transmit(&huart1, "Crossing page boundary..\n", strlen("Crossing page boundary..\n"), 500);
		#endif

		if ( head_addr >= next_sector ) {
			/* we shall be crossing sector boundary - erase next sector */
			if ( head_addr >= rec->section_end ) {
				head_addr = rec->section_start;
				next_sector = rec->section_start;

				#ifdef ON_DEBUG_MESSAGE
					HAL_UART_Transmit(&huart1, "End of section reached - rewinding\n", strlen("End of section reached - rewinding\n"), 500);
				#endif
			}

			flash_sector_erase(next_sector);

			#ifdef ON_DEBUG_MESSAGE
				HAL_UART_Transmit(&huart1, "We need to erase next sector\n\n", strlen("We need to erase next sector\n\n"), 500);
			#endif
		}
	}

	// check if next block is dirty
	flash_read_buf(head_addr, buf, rec_size(rec));
	for ( int i = 0; i < rec_len(rec); i++ )
	{
		if ( buf[i] != 0xffffffff ) {
			rec->head_addr = head_addr + size;

			#ifdef ON_DEBUG_MESSAGE
				HAL_UART_Transmit(&huart1, "Page is dirty - aborting\n", strlen("Page is dirty - aborting\n"), 500);
			#endif

			return STORAGE_STATUS_PAGE_DIRTY;
		}
	}

	// prepare buffer
	memcpy(buf, rec->data, rec->data_size);
	buf[len - 4] = rec->counter;
	buf[len - 3] = crc;
	buf[len - 2] = 0x4D524F4E;																							// free uint32_t to use (write "NORM" now)
	buf[len - 1] = STORAGE_MAGIC;

	// write as-is
	if ( flash_write(head_addr, buf, size) != true )
		return STORAGE_STATUS_IO_ERROR;

	head_addr += size;

	// Проверка, если адрес >= адреса последней страницы, то устанавливаем его на адрес первой страницы
	if ( head_addr >= rec->section_end ) {
		rec->head_addr = rec->section_start;
	} else {
		rec->head_addr = head_addr;
	}

	rec->counter++;

	return STORAGE_STATUS_SUCCESS;
}

enum storage_status storage_read( struct DataRecord *rec )
{
	uint32_t tail_addr = 0;
	uint32_t counter;
	enum storage_status s = storage_get_tail(rec, &tail_addr, &counter);
	if ( s == STORAGE_STATUS_SUCCESS || s == STORAGE_STATUS_CRC_MISMATCH ) {
		flash_read_buf(tail_addr, rec->data, rec->data_size);
	}
	return s;
}

enum storage_status storage_read_data( uint32_t addr, const void *data, uint16_t data_size )
{
	enum storage_status result = STORAGE_STATUS_CRC_MISMATCH;

	uint16_t crc = crc16_ccit((const uint8_t *)addr, data_size, 0);

	if ( crc == *((uint16_t*)(crc_address(addr, data_size))) ) {
		flash_read_buf(addr, (uint32_t*)data, data_size);
		result = STORAGE_STATUS_SUCCESS;
	}

	return result;
}

enum storage_status storage_get_tail( struct DataRecord *rec, uint32_t *tail_addr, uint32_t *counter )
{
	uint32_t max_ctr = 0;
	uint32_t max_ctr_addr = 0;
	uint8_t crc_is_ok = 1;
	uint8_t found = 0;

	for ( uint32_t addr = rec->section_start; addr + rec_size(rec) <= rec->section_end; addr += rec_size(rec) )
	{
		flash_read_buf(addr, buf, rec_size(rec));

		if ( buf[rec_len(rec) - 1] == STORAGE_MAGIC ) {
			uint32_t crc = buf[rec_len(rec) - 3];
			uint32_t ctr = buf[rec_len(rec) - 4];
			uint16_t crc_calc = crc16_ccit((const uint8_t *) buf, rec->data_size, 0);

			#ifdef ON_DEBUG_MESSAGE
				snprintf(debug_buf, sizeof(debug_buf), "Magic @ %X: crc = %X, crc_calc = %X, ctr = %d\n", addr, crc, crc_calc, ctr);
				HAL_UART_Transmit(&huart1, (uint8_t*)debug_buf, strlen(debug_buf), 500);
			#endif

			if ( crc == crc_calc ) {
				// WARNING: this will overflow someday..
				if ( ctr >= max_ctr ) {
					found = 1;
					max_ctr = ctr;
					max_ctr_addr = addr;
				}
			} else {
				crc_is_ok = 0;
				#ifdef ON_DEBUG_MESSAGE
					HAL_UART_Transmit(&huart1, "CRC mismatch - continuing\n", strlen("CRC mismatch - continuing\n"), 500);
				#endif
			}
		}
	}

	#ifdef ON_DEBUG_MESSAGE
	snprintf(debug_buf, sizeof(debug_buf), "Current MAX address: %X, MAX ctr: %d\n", max_ctr_addr, max_ctr);
		HAL_UART_Transmit(&huart1, (uint8_t*)debug_buf, strlen(debug_buf), 500);
	#endif

	if ( found ) {
		*tail_addr = max_ctr_addr;
		*counter = max_ctr;
		if ( !crc_is_ok )
			return STORAGE_STATUS_CRC_MISMATCH;
		return STORAGE_STATUS_SUCCESS;
	}

	return STORAGE_STATUS_NOT_FOUND;
}
