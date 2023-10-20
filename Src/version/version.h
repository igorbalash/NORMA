/************************************************************************************
                  ** H **
Module: Модуль определяет дефайны для проекта исходя из версии железа и фуникцонала.
		Версия железа задается в plat.h.
		Функционал задается в defs.h.
Author: Unic-Lab <https://unic-lab.ru/>
************************************************************************************/

#include "defs.h"
#include "plat.h"

//===================================================================================

/**
	Поддерживаемый функционал:
	#define ON_MCO																										// включить MCO. MCO настраивается для вывода SYSCLK/16
	#define ON_DEBUG_MESSAGE																							// включить вывод отладочной информации
*/

//===================================================================================

/**
	В зависимости от типа сборки BUILD_*
	определяем функционал PRODUCT_VARIANT_FUNC.
*/
#if (BUILT_TYPE == BUILD_NO_LOG)
	#define PRODUCT_VARIANT_FUNC	1

#elif (BUILT_TYPE == BUILD_WITH_LOG)
	#define PRODUCT_VARIANT_FUNC	2
	#define ON_DEBUG_MESSAGE

#elif (BUILT_TYPE == BUILD_WITH_EMULATE_SIDE_PANEL)
	#define PRODUCT_VARIANT_FUNC	3
	#define SWAP_DOWN_SIDE_PINS
	#define ON_DEBUG_MESSAGE

#else
	#error "unknown BUILD type"
#endif

//===================================================================================

/**
	В зависимости от типа сборки BUILD_* и функционала PRODUCT_VARIANT_FUNC
	определяем вариант сборки PRODUCT_VARIANT.
*/
#if (BUILD_DEBUG_JLINK == 1) && (BUILD_RELEASE_JLINK == 0) && (BUILD_RELEASE_NO_JLINK == 0)
	#define PRODUCT_VARIANT	(PRODUCT_VARIANT_FUNC | 0x80)
	#define DEBUG_MODE
	#define MCU_DEBUG_PORT_ACTIVE
#elif (BUILD_DEBUG_JLINK == 0) && (BUILD_RELEASE_JLINK == 1) && (BUILD_RELEASE_NO_JLINK == 0)
	#define PRODUCT_VARIANT	(PRODUCT_VARIANT_FUNC)
	#define RELEASE_MODE
	#define ON_WDOG_IN_APP
	#define MCU_DEBUG_PORT_ACTIVE
#elif (BUILD_DEBUG_JLINK == 0) && (BUILD_RELEASE_JLINK == 0) && (BUILD_RELEASE_NO_JLINK == 1)
	#define PRODUCT_VARIANT	(PRODUCT_VARIANT_FUNC)
	#define RELEASE_MODE
	#define ON_WDOG_IN_APP
#else
	#error "unknown BUILD type"
#endif

//===================================================================================

/**
	Таблица соответствия (PLAT_SCHEME_VERSION_MAJOR.PLAT_SCHEME_VERSION_MINOR - PLAT_SCHEME_VARIANT) и PRODUCT_HARD
	( 0.0 - 1 ) - 1 - Фрезированный прототип
	( 0.0 - 0 ) - 2 - Опытные образцы для NORMA плата v.0.0
    ( 2.0 - 0 ) - 2 - Опытные образцы для NORMA плата v.2.0
*/
#if (PLAT_SCHEME_VERSION_MAJOR == 0) && (PLAT_SCHEME_VERSION_MINOR == 0) && (PLAT_SCHEME_VARIANT == 1)
	#include "hard_1.h"
	#if !defined(STM32F103xB)
		#error "You must define <STM32F103xB> in PRJ options"
	#endif
#elif ((PLAT_SCHEME_VERSION_MAJOR == 0) && (PLAT_SCHEME_VERSION_MINOR == 0) && (PLAT_SCHEME_VARIANT == 0)) || \
      ((PLAT_SCHEME_VERSION_MAJOR == 2) && (PLAT_SCHEME_VERSION_MINOR == 0) && (PLAT_SCHEME_VARIANT == 0))
	#include "hard_2.h"
	#if !defined(STM32F103xE)
		#error "You must define <STM32F103xE> in PRJ options"
	#endif
#else
	#error "UNSUPPORTED HARDWARE!"
#endif

//===================================================================================

// полная версия ПО, состоящая из 4 частей
#define FW_VERSION			((PRODUCT_ID 		<< 0)  | \
							(PRODUCT_VERSION	<< 8)  | \
							(PRODUCT_VARIANT 	<< 16) | \
							(PRODUCT_HARD 		<< 24))
