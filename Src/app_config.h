/************************************************************************************
                  ** H **
Module:
Author: Unic-Lab <https://unic-lab.ru/>
************************************************************************************/

#ifndef _APP_CONFIH_H_
#define _APP_CONFIH_H_

//===================================================================================

#define APP_START_DELAY_MS				4000																			// задержка перед запуском программы, мСек

#define UP_MOTOR_CURR_THRESH_MA			2500																			// пороговое значения токопотребления верхнего актуатора, мА
#define DOWN_MOTOR_CURR_THRESH_MA		2500																			// пороговое значения токопотребления нижнего актуатора, мА
#define SIDE_MOTOR_CURR_THRESH_MA		2000																			// пороговое значения токопотребления одного из боковых актуаторов, мА

#define MOTOR_TIMEOUT_MS				20000																			// таймаут при движении актуаторов, мСек

#define MIN_EXT_VOLTAGE_MV				9000																			// мин. внешнее напряжение, мВ
#define MAX_EXT_VOLTAGE_MV				13000																			// макс. внешнее напряжение, мВ

#endif // _LEDS_H_