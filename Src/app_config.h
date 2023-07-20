/************************************************************************************
                  ** H **
Module:
Author: Unic-Lab <https://unic-lab.ru/>
************************************************************************************/

#ifndef _APP_CONFIH_H_
#define _APP_CONFIH_H_

//===================================================================================

#define APP_START_DELAY_MS				4000																			// задержка перед запуском программы, мСек

#define UP_MOTOR_CURR_THRESH_A			2.5f																			// пороговое значения токопотребления верхнего актуатора, А
#define DOWN_MOTOR_CURR_THRESH_A		2.5f																			// пороговое значения токопотребления нижнего актуатора, А
#define SIDE_MOTOR_CURR_THRESH_A		2.0f																			// пороговое значения токопотребления одного из боковых актуаторов, А

#define MOTOR_TIMEOUT_MS				20000																			// таймаут при движении актуаторов, мСек
#define MOTOR_DOWN_MOV_DELAY_MS			3000																			// задержка перед выдвижение нижнего актуатора после начала движения верхнего, мСек

#define MIN_EXT_VOLTAGE_V				9.0f																			// мин. внешнее напряжение, В
#define MAX_EXT_VOLTAGE_V				15.0f																			// макс. внешнее напряжение, В

#endif // _LEDS_H_
