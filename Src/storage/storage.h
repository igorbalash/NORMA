/************************************************************************************
                  ** H **
Module:
Author: Unic-Lab <https://unic-lab.ru/>
************************************************************************************/

#ifndef _STORAGE_H_
#define _STORAGE_H_

//===================================================================================

#include "version.h"
#include <stdbool.h>
#include <stdint.h>
#include "stm32f1xx_hal.h"

//===================================================================================

#if (PRODUCT_HARD == 1)
	#define _FLASH_PAGE_SIZE			FLASH_PAGE_SIZE
	#define _FLASH_SECTOR_SIZE			_FLASH_PAGE_SIZE
#elif (PRODUCT_HARD == 2)
	#define _FLASH_PAGE_SIZE			FLASH_PAGE_SIZE
	#define _FLASH_SECTOR_SIZE			_FLASH_PAGE_SIZE
#else
	#error "UNKNOWN PRODUCT_HARD!"
#endif

#define STORAGE_MAX_DATA_LENGTH         4
#define STORAGE_MAGIC                   0xDEADBEEF

//===================================================================================

enum storage_status
{
	STORAGE_STATUS_IO_ERROR,
	STORAGE_STATUS_PAGE_DIRTY,
	STORAGE_STATUS_NOT_FOUND,
	STORAGE_STATUS_CRC_MISMATCH,
	STORAGE_STATUS_SUCCESS
};

struct DataRecord
{
	uint32_t head_addr;
	uint32_t counter;
	uint32_t *data;
	const uint16_t data_size;
	const uint32_t section_start;
	const uint32_t section_end;
};

//===================================================================================

	/**
	! Initialize data storage. Flash section is scanned for valid data
		and head_addr is restored if valid data is found.
	*/
enum storage_status storage_init( struct DataRecord *rec );

	/**
	! Append record to a circular storage. Page programming and sector erasure are
		performed automatically
	- rec DataRecord to append
	+ STORAGE_STATUS_SUCCESS - record written successfully
	+ STORAGE_STATUS_IO_ERROR - flash error occurred during write operation
	+ STORAGE_STATUS_PAGE_DIRTY - record was not written because the page is dirty;
		@head_addr will be incremented, so that subsequent function call will attempt
		a write at the next location.
	*/
enum storage_status storage_write( struct DataRecord *rec );

	/**
	! Read back a single record.
	- rec DataRecord to read. Data will be stored in *data buffer of the record.
	+ STORAGE_STATUS_SUCCESS - record found
	+ STORAGE_STATUS_CRC_MISMATCH - an older record was read, since newer record(s) are corrupt
	+ STORAGE_STATUS_NOT_FOUND - no entries found
	*/
enum storage_status storage_read( struct DataRecord *rec );

	/**
	! Find the last valid record and return it's start address.
	- rec DataRecord
	- tail_addr record address will be written here
	- counter record counter will be written here
	+ STORAGE_STATUS_SUCCESS - record found
	+ STORAGE_STATUS_CRC_MISMATCH - an older record was read, since newer record(s) are corrupt
	+ STORAGE_STATUS_NOT_FOUND - no entries found
	*/
enum storage_status storage_get_tail( struct DataRecord *rec, uint32_t *tail_addr, uint32_t *counter );

enum storage_status storage_read_data( uint32_t addr, const void *data, uint16_t data_size );

//===================================================================================

#endif // _STORAGE_H_
