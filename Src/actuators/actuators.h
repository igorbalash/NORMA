/************************************************************************************
                  ** H **
Module:
Author: Unic-Lab <https://unic-lab.ru/>
************************************************************************************/

#ifndef _ACTUATORS_H_
#define _ACTUATORS_H_

//===================================================================================

#include <stdbool.h>

//===================================================================================

typedef enum {
	UP_ACTUATOR = 0,
	DOWN_ACTUATOR,
	SIDE_ACTUATOR
} ActuatorType_t;

typedef enum {
	UP_DIR = 0,
	DOWN_DIR
} Direction_t;

//===================================================================================

void actuators_init(void);
void actuators_main_power_off(bool is_need_reverse_polarity);
void actuators_prepare_move(Direction_t direction, bool is_need_reverse_polarity);
void actuators_power_on(ActuatorType_t act_type);
void actuators_power_off(ActuatorType_t act_type);

//===================================================================================

#endif // _ACTUATORS_H_
