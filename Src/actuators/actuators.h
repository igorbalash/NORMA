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

//===================================================================================

void actuators_init(void);
void actuators_start_move_up(ActuatorType_t act_type);
void actuators_start_move_down(ActuatorType_t act_type);
void actuators_stop_move(ActuatorType_t act_type);
void actuators_main_power_on(bool is_need_reverse_polarity);
void actuators_main_power_off(bool is_need_reverse_polarity);

#endif // _ACTUATORS_H_
